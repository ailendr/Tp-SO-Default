/*
 * planificacion.c
 *
 *  Created on: Apr 28, 2023
 *      Author: utnso
 */

#include "planificacion.h"

t_queue *colaNew;
t_list *colaReady;
t_pcb *ultimoEjecutado;
uint32_t pid = 0;

void crearEstados() {
	colaNew = queue_create();
	colaReady = list_create();
}

void eliminarEstados() {
	list_destroy(colaReady);
	queue_clean(colaNew);
	queue_destroy(colaNew);
}

void agregarAEstadoNew(t_pcb *procesoNuevo) {
	pthread_mutex_lock(&mutexNew);
	queue_push(colaNew, procesoNuevo);
	procesoNuevo->estadoPcb = NEW;
	pthread_mutex_unlock(&mutexNew);
}

void agregarAEstadoReady(t_pcb *procesoListo) {
	pthread_mutex_lock(&mutexReady);
	list_add(colaReady, procesoListo);
	procesoListo->estadoPcb = READY;
	clock_gettime(CLOCK_REALTIME, &(procesoListo->llegadaAReady));//Por HRRN
	pthread_mutex_unlock(&mutexReady);
	log_info(loggerKernel,
			"Cola Ready con algoritmo %s .Ingresa el proceso con id %d:",
			Algoritmo(), procesoListo->contexto->pid);
}

t_pcb* extraerDeNew() {
	pthread_mutex_lock(&mutexNew);
	t_pcb *proceso = queue_pop(colaNew);
	pthread_mutex_unlock(&mutexNew);
	return proceso;
}

t_pcb* extraerDeReady() {
	pthread_mutex_lock(&mutexReady);
	t_pcb *proceso = list_remove(colaReady, 0); //Obtiene el 1er elem de la lista y lo elimina de la lista
	pthread_mutex_unlock(&mutexReady);
	return proceso;
}

void logCambioDeEstado(t_pcb* proceso, char* estadoAnterior, char* nuevoEstado){
	log_info(loggerKernel, "PID:%d  - Estado Anterior: %s - Estado Actual: %s", proceso->contexto->pid, estadoAnterior, nuevoEstado);
}


void procesoAEjecutar(t_contextoEjec *procesoAEjecutar) {

	t_paquete *paqueteDeContexto = serializarContexto(procesoAEjecutar); // "Pone el contexto en un paquete"
	log_info(loggerKernel, "Paquete de Contexto creado con exito");
	validarEnvioDePaquete(paqueteDeContexto, socketCPU, loggerKernel, configKernel, "Contexto");
}

void largoPlazo() {
	while (1) {

		sem_wait(&planiLargoPlazo);
		log_info(loggerKernel, "Pase el semaforo de largo plazo");
		t_pcb *proceso;
		//Pasaje de New a Ready//

		sem_wait(&multiprogramacion);
		log_info(loggerKernel, "Pase el gr de multiprogramacion");//Siempre que entra aca se descuenta el gr de multiprogramacion en el sistema
		proceso = extraerDeNew(colaNew);
		//enviarProtocolo(socketMemoria, HANDSHAKE_PedirMemoria,loggerKernel); //El handshake seria el pedido de memoria
		//asignarMemoria(proceso, tabla); //PCB creado
		//log_info(loggerKernel, "Tabla de segmentos inicial ya asignada a proceso PID: %d, proceso->contexto->pid);
		agregarAEstadoReady(proceso);
		logCambioDeEstado(proceso, "NEW", "READY");
		sem_post(&planiCortoPlazo);
	}
}
void ordenarReady(){
	if (strcmp(Algoritmo(), "HRRN")==0){
		log_info(loggerKernel, "Cola Ready ordenada por HRRN");
		list_iterate(colaReady, (void*) calcularNuevaEstimacion);
		list_iterate(colaReady, (void*) calcularRR);
		list_sort(colaReady, (void*)comparadorRR);
	}
	else {
		log_info(loggerKernel, "Cola Ready ordenada por FIFO");} //se sabe q no ordena nada solo es un log
}

void enviarContextoACpu(){
		t_pcb* procesoAEjec=extraerDeReady();
		log_info(loggerKernel, "%s: Obtengo el proceso %d de Ready", Algoritmo(), procesoAEjec->contexto->pid);
		t_contextoEjec * contextoAEjec = procesoAEjec->contexto;
		procesoAEjecutar(contextoAEjec);
		procesoAEjec->estadoPcb = EXEC;
		logCambioDeEstado(procesoAEjec, "READY", "EXEC");
		if(strcmp(Algoritmo(), "HRRN")==0){
		clock_gettime(CLOCK_REALTIME, &(procesoAEjec->llegadaACPU));
		}
		ultimoEjecutado = procesoAEjec;
}

void cortoPlazo() {
	while (1) {

		sem_wait(&planiCortoPlazo);
		log_info(loggerKernel, "Corto Plazo habilitado");
		ordenarReady();
        enviarContextoACpu();
		instruccionAEjecutar();
	}
}

void instruccionAEjecutar() {
		int tamanio = 0;
//Recepcion del contexto//
		int codContexto = recibir_operacion(socketCPU);
		t_contextoEjec *contextoActualizado; //PROBADO: TIENE QUE IR SI O SI POR EL ORDEN EN EL QUE SE ENVIAN LAS COSAS
		void *buffer = recibir_buffer(&tamanio, socketCPU);
		log_info(loggerKernel, "Se recibio el buffer del Contexto");
		contextoActualizado = deserializarContexto(buffer, tamanio);
		log_info(loggerKernel, "Contexto recibido con pid : %d", contextoActualizado->pid);
		ultimoEjecutado->contexto = contextoActualizado;
		free(buffer);
//Recepcion de una instruccion//
		int codigo = recibir_operacion(socketCPU);
		switch(codigo){
			case EXIT:
				log_info(loggerKernel, "Intruccion EXIT");
				t_instruccion* instruccionExit= obtenerInstruccion(socketCPU,0);
				free(instruccionExit);
				finalizarProceso(ultimoEjecutado, "SUCCESS");
				break;
			case YIELD:
				log_info(loggerKernel, "Intruccion YIELD");
				t_instruccion* instruccionYield = obtenerInstruccion(socketCPU,0);
				free(instruccionYield);
				tiempoEnCPU(ultimoEjecutado);
				agregarAEstadoReady(ultimoEjecutado);
				//sem_post(&cpuLibre);
				sem_post(&planiCortoPlazo);

				break;

			case WAIT:
				log_info(loggerKernel, "Intruccion WAIT");
				t_instruccion* instruccionWait = obtenerInstruccion(socketCPU,1);
				log_info(loggerKernel, "Recurso a consumir : %s", instruccionWait->param1);
				char* recursoAConsumir = instruccionWait->param1;
				implementacionWyS(recursoAConsumir, 1, contextoActualizado);
				free(instruccionWait);
				sem_post(&planiCortoPlazo);
				break;
			case SIGNAL:
				log_info(loggerKernel, "Intruccion SIGNAL");
				t_instruccion* instruccionSignal = obtenerInstruccion(socketCPU,1);
				char* recursoALiberar = instruccionSignal->param1;
				free(instruccionSignal); //Para mi hay q liberar el puntero a la instruccion, una vez q obtenemos el parametro
				implementacionWyS(recursoALiberar, 2, contextoActualizado);

				break;
			case IO:
				log_info(loggerKernel, "Intruccion IO");
				tiempoEnCPU(ultimoEjecutado); //no sé si ponerlo aca o donde tomarle el tiempo
				t_instruccion* instruccionIO = obtenerInstruccion(socketCPU,1);
				char* tiempo = instruccionIO->param1;
				int* tiempoDeIO = malloc(sizeof(int));
				*tiempoDeIO = atoi(tiempo);//en microsegundos
				free(instruccionIO);//Hay q liberar puntero
				pthread_t hiloDeBloqueo; //crear hilo
				pthread_create(&hiloDeBloqueo, NULL, (void*)bloquearHilo, (void*)tiempoDeIO);
				pthread_detach(hiloDeBloqueo);
				break;
			case MOV_IN:
				log_info(loggerKernel, "Intruccion MOV_IN Fallida");
				t_instruccion* instruccionMI = obtenerInstruccion(socketCPU,2);
				free(instruccionMI);
				finalizarProceso(ultimoEjecutado, "SEG_FAULT");
				break;
			case MOV_OUT:
				log_info(loggerKernel, "Intruccion MOV_OUT Fallida");
				t_instruccion* instruccionMO = obtenerInstruccion(socketCPU,2);
				free(instruccionMO);
				finalizarProceso(ultimoEjecutado, "SEG_FAULT");
				break;
			case CREATE_SEGMENT:
				break;
			case DELETE_SEGMENT:
				break;
			case F_OPEN:
				break;
			case F_CLOSE:
				break;
			case F_SEEK:
				break;
			case F_READ:
				log_info(loggerKernel, "Intruccion F READ");
				t_instruccion* instruccionFR = obtenerInstruccion(socketCPU,3);
                validarRyW(instruccionFR->param2);
                //Serializa la instruccion ,la manda a FS y bloquea al proceso //
                implementacionF(instruccionFR);
				break;
			case F_WRITE:
				log_info(loggerKernel, "Intruccion F WRITE");
				t_instruccion* instruccionFW = obtenerInstruccion(socketCPU,3);
                validarRyW(instruccionFW->param2);
                implementacionF(instruccionFW);

				break;
			case F_TRUNCATE:
				break;

			default:
				break;
		}
	}


void algoritmoFIFO() {
	log_info(loggerKernel, "Empieza algoritmo FIFO");
	t_pcb *procesoAEjec = extraerDeReady();
	log_info(loggerKernel, "FIFO: Obtengo un proceso de ready");
	t_contextoEjec *contextoAEjec = procesoAEjec->contexto;
	procesoAEjecutar(contextoAEjec);
	procesoAEjec->estadoPcb = EXEC;
	logCambioDeEstado(procesoAEjec, "READY", "EXEC");
	ultimoEjecutado = procesoAEjec;
}


void algoritmoHRRN(){
	log_info(loggerKernel, "Empieza algoritmo HRRN");
	t_pcb* procesoAEjec;
	list_iterate(colaReady, (void*) calcularNuevaEstimacion);
	list_iterate(colaReady, (void*) calcularRR);
	list_sort(colaReady, (void*)comparadorRR);//Aca me ordena la lista por el comparador de RR
	procesoAEjec=extraerDeReady();
	log_info(loggerKernel, "HRRN: Obtengo un proceso de ready");

	t_contextoEjec * contextoAEjec = procesoAEjec->contexto;
	procesoAEjecutar(contextoAEjec);
	procesoAEjec->estadoPcb=EXEC;
	logCambioDeEstado(procesoAEjec, "READY", "EXEC");
	clock_gettime(CLOCK_REALTIME, &(procesoAEjec->llegadaACPU));//Por HRRN
	ultimoEjecutado = procesoAEjec;
}

void calcularNuevaEstimacion(t_pcb* proceso) {
	double alfa = Alfa();
    double nuevaEstimacion = (alfa * proceso->ultimaRafagaEjecutada)+ (proceso->estimadoReady *(1 - alfa));
    proceso->estimadoReady = nuevaEstimacion;
}

void tiempoEnCPU(t_pcb* proceso){
	if(strcmp("HRRN", Algoritmo())==0){
    struct timespec end;
    clock_gettime(CLOCK_REALTIME, &end);

    long seconds = end.tv_sec - proceso->llegadaACPU.tv_sec;
    long nanoseconds = end.tv_nsec - proceso->llegadaACPU.tv_nsec;
    double elapsed = seconds + nanoseconds*1e-9;

    proceso->ultimaRafagaEjecutada=elapsed;
	}
}

void calcularRR(t_pcb* proceso) {

    struct timespec end;
    clock_gettime(CLOCK_REALTIME, &end);

    long seconds = end.tv_sec - proceso->llegadaAReady.tv_sec;
    long nanoseconds = end.tv_nsec - proceso->llegadaAReady.tv_nsec;
    double elapsed = seconds + nanoseconds*1e-9;

    proceso->tiempoDeEspera = elapsed;
	proceso->RR = 1 + (proceso->tiempoDeEspera / proceso->estimadoReady);

}

bool comparadorRR(t_pcb* proceso1, t_pcb* proceso2) {

    return proceso1->RR >= proceso2->RR;

}

//////////////////////LISTA DE INSTRUCCIONES ,PROCESOS, E INSTRUCCION BY CPU////////////////////////////
t_instruccion* obtenerInstruccion(int socket, int cantParam){
	int tamanio = 0;
	void *buffer = recibir_buffer(&tamanio, socketCPU);
	t_instruccion* instruccionNueva = deserializarInstruccionEstructura(buffer, cantParam);
	return instruccionNueva;
}

t_list* obtenerInstrucciones(int socket_cliente) {
	int codigoOp = recibir_operacion(socket_cliente);
	t_list *listaDeInstrucciones; //Se libera cuando termina el pcb
	uint32_t recepcion = 1;
	if (codigoOp == PAQUETE) { //Ver si lo aplicamos o sacamos
		listaDeInstrucciones = recibir_paquete(socket_cliente);
		if(!list_is_empty(listaDeInstrucciones)){
			send(socket_cliente, &recepcion, sizeof(uint32_t), 0);//Consola deberia verificar que si es 1-> kernel recibio todo OK
		}
	else{
		recepcion = 0;
		send(socket_cliente, &recepcion, sizeof(uint32_t), 0);
		log_info(loggerKernel, "Error: La lista de instrucciones esta vacia");} //Consola deberia verificar que si es 0-> Kernel recibio algo vacio
	}
	return listaDeInstrucciones;
}

void generarProceso(int *socket_cliente) {
	int consolaNueva = *socket_cliente;
	recibirProtocolo(socket_cliente); //Handashake No lo incluimos por el momento porq la func cierra las conexiones-> podemos modificarla dsps
	t_list *instrucciones = obtenerInstrucciones(consolaNueva);
	if(list_is_empty(instrucciones)){
		log_info(loggerKernel, "Error: La lista de instrucciones esta vacia");
		close(consolaNueva);//cierro la conexion con esa consola
	}
	else{
	pthread_mutex_lock(&mutexPID);
	loggearListaDeIntrucciones(instrucciones);//Lo puse adentro porq muestra el pid
	t_pcb *procesoNuevo = crearPcb(instrucciones, pid, consolaNueva);
	pid++;
	pthread_mutex_unlock(&mutexPID);
	agregarAEstadoNew(procesoNuevo);
	log_info(loggerKernel, "Se crea el proceso %d en New",
			procesoNuevo->contexto->pid);

	//Cerrando recursos
	//close(consolaNueva);//Ver si esta demas esto o nos romperia
	//free(socket_cliente); //duda de si esta bien el free o puede romper en la conexion aunque no lo creemos
	sem_post(&planiLargoPlazo);
	}
}

void asignarMemoria(t_pcb *procesoNuevo, t_list *tablaDeSegmento) {
	procesoNuevo->tablaSegmentos = tablaDeSegmento;

}

void finalizarProceso(t_pcb *procesoAFinalizar, char* motivoDeFin) {

	log_info(loggerKernel, "Finaliza el proceso %d - Motivo:%s",
			procesoAFinalizar->contexto->pid, motivoDeFin);
	int terminar = -1;
	send(procesoAFinalizar->socketConsola, &terminar, sizeof(int), 0); //Avisa a consola que finalice
	free(procesoAFinalizar->contexto->instrucciones);
	free(procesoAFinalizar->contexto);
	// send(socketMemoria,&motivo, sizeof(uint32_t)); //Solicita a memoria que elimine la tabla de segmentos
	free(procesoAFinalizar);
	sem_post(&multiprogramacion);
	//sem_post(&cpuLibre);
	sem_post(&planiCortoPlazo);
}

///---------RECURSOS COMPARTIDOS PARA WAIT Y SIGNAL----///
void implementacionWyS (char* nombreRecurso, int nombreInstruccion, t_contextoEjec* contextoActualizado){
	/*1ero buscar el recurso: si no está finaliza el proceso
	 -----------                         si esta decrementa o incrementa la instancia*/
	int posicionRecurso = recursoDisponible(nombreRecurso);
		if(posicionRecurso ==-1){
			finalizarProceso(ultimoEjecutado, "Recurso No Existente");
		}
		else{
			int* pvalor = list_get(listaDeInstancias,posicionRecurso);
			int valor = *pvalor;
			//free(pvalor); Preguntar como liberar una lista de punteross
			switch (nombreInstruccion){
			case 1: //1=WAIT
				valor --;
				log_info(loggerKernel, "PID: %d - WAIT: %s - Instancias: %d", ultimoEjecutado->contexto->pid, nombreRecurso, valor);
				 if(valor<0){
				t_queue* colaDeBloqueo = (t_queue*)list_get(listaDeBloqueo, posicionRecurso);
				queue_push(colaDeBloqueo, ultimoEjecutado);
				tiempoEnCPU(ultimoEjecutado);
				ultimoEjecutado->estadoPcb = BLOCK;
				logCambioDeEstado(ultimoEjecutado, "EXEC", "BLOCK");
				log_info(loggerKernel, "PID: %d -Bloqueado por %s:", ultimoEjecutado->contexto->pid, nombreRecurso);
				 }
				 else{
					 procesoAEjecutar(contextoActualizado); //Sigue en cpu
					 }
				break;
			case 2://2=SIGNAL
				valor ++;
				log_info(loggerKernel, "PID: %d - SIGNAL: %s - Instancias: %d", ultimoEjecutado->contexto->pid, nombreRecurso, valor);
				t_queue* colaDeBloqueo = (t_queue*)list_get(listaDeBloqueo, posicionRecurso);
				t_pcb* procesoDesbloqueado = queue_pop(colaDeBloqueo);
				agregarAEstadoReady(procesoDesbloqueado);
				logCambioDeEstado(procesoDesbloqueado,"BLOCK" ,"READY");
				procesoAEjecutar(contextoActualizado);//Sigue en cpu
				break;
			 }
			}
		}

////---Funcion de IO---///
void bloquearHilo(int* tiempo){
	int tiempoDeBloqueo = *tiempo;
	t_pcb* procesoBloqueado = ultimoEjecutado;
	tiempoEnCPU(procesoBloqueado);
	procesoBloqueado->estadoPcb= BLOCK;
	log_info(loggerKernel, "PID: %d - Bloqueado por: IO", procesoBloqueado->contexto->pid);
    logCambioDeEstado(procesoBloqueado, "EXEC", "BLOCK");
	usleep(tiempoDeBloqueo);
	agregarAEstadoReady(procesoBloqueado); //Agrega a la cola y cambia el estado del pcb
	logCambioDeEstado(procesoBloqueado, "BLOCK", "READY");
	sem_post(&planiCortoPlazo);
}
//Validacion para F Read y FWrite//
void validarRyW(char* direccion){
	int direc = atoi(direccion);
	if(direc == -1){
		finalizarProceso(ultimoEjecutado, "SEG_FAULT");
	}
}
//Funcion General para la mayoria de isntrucciones q empiezan con F//
void implementacionF(t_instruccion* instruccion){
	t_paquete* paqueteF = serializarInstruccion(instruccion);
	validarEnvioDePaquete(paqueteF, socketFs, loggerKernel, configKernel, "Instruccion");
	ultimoEjecutado->estadoPcb= BLOCK;
	log_info(loggerKernel, "PID: %d - Bloqueado por operar sobre el archivo: %s", ultimoEjecutado->contexto->pid, instruccion->param1);
	logCambioDeEstado(ultimoEjecutado, "EXEC", "BLOCK");
	tiempoEnCPU(ultimoEjecutado);
	free(instruccion);
}

//Logueo de las instrucciones para verificar que esta todo ok//
void loggearListaDeIntrucciones(t_list* instrucciones){
	int tamanioListaInstrucciones = list_size(instrucciones);
	log_info(loggerKernel, "La lista de instrucciones del proceso %d es:", pid);
		for (int i = 0; i < tamanioListaInstrucciones; i++){
			 char* instruccion= list_get(instrucciones,i);
			 log_info(loggerKernel, "%s", instruccion);
		}
}
