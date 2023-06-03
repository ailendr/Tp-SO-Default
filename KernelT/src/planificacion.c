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
	sem_post(&productorColaNew);
}

void agregarAEstadoReady(t_pcb *procesoListo) {
	pthread_mutex_lock(&mutexReady);
	list_add(colaReady, procesoListo);
	procesoListo->estadoPcb = READY;
	clock_gettime(CLOCK_REALTIME, &(procesoListo->llegadaAReady));//Por HRRN
	pthread_mutex_lock(&mutexReady);
	sem_post(&productorListaReady);
	log_info(loggerKernel,
			"Cola Ready con algoritmo %s .Ingresa el proceso con id %d:",
			Algoritmo(), procesoListo->contexto->pid);
}

t_pcb* extraerDeNew() {
	sem_wait(&productorColaNew);
	pthread_mutex_lock(&mutexNew);
	t_pcb *proceso = queue_pop(colaNew);
	pthread_mutex_lock(&mutexNew);
	return proceso;
}

t_pcb* extraerDeReady() {
	sem_wait(&productorListaReady);
	pthread_mutex_lock(&mutexReady);
	t_pcb *proceso = list_remove(colaReady, 0); //Obtiene el 1er elem de la lista y lo elimina de la lista
	pthread_mutex_lock(&mutexReady);
	return proceso;
}

void cambioDeEstado(t_pcb* proceso, char* estadoAnterior, char* nuevoEstado){
	log_info(loggerKernel, "PID:%d  - Estado Anterior: %s - Estado Actual: %s", proceso->contexto->pid, estadoAnterior, nuevoEstado);
}


void procesoAEjecutar(t_contextoEjec *procesoAEjecutar) {

	t_paquete *paqueteDeContexto = serializarContexto(procesoAEjecutar); // "Pone el contexto en un paquete"
	enviar_paquete(paqueteDeContexto, socketCPU); //Acá se serializa el paquete y se envia
}

void largoPlazo() {
	while (1) {

		sem_wait(&planiLargoPlazo);
		t_pcb *proceso;
		//Pasaje de New a Ready//

		sem_wait(&multiprogramacion); //Siempre que entra aca se descuenta el gr de multiprogramacion en el sistema
		proceso = extraerDeNew(colaNew);
		enviarProtocolo(socketMemoria, HANDSHAKE_PedirMemoria,loggerKernel); //El handshake seria el pedido de memoria
		//asignarMemoria(proceso, tabla); //PCB creado
		//log_info(loggerKernel, "Tabla de segmentos inicial ya asignada a proceso PID: %d, proceso->contexto->pid);
		agregarAEstadoReady(proceso);
		cambioDeEstado(proceso, "NEW", "READY");
		sem_post(&planiCortoPlazo);
	}
}

void cortoPlazo() {
	while (1) {
		log_info(loggerKernel, "El corto plazo esta trabajando");
		sem_wait(&planiCortoPlazo);
		sem_wait(&cpuOcupada);
		log_info(loggerKernel, "El corto plazo esta trabajando");
		char *algoritmo = Algoritmo();
		if (strcmp(algoritmo, "FIFO") == 0) {
			algoritmoFIFO();
		}
		if (strcmp(algoritmo, "HRRN")==0){
			algoritmoHRRN();
		}else{
			log_info(loggerKernel, "El algoritmo obtenido por archivo de config no es válido");
			exit(1);
		}
		instruccionAEjecutar();
	}
}

void instruccionAEjecutar() {
	int tamanio = 0;

	//A futuro agregar los de FS
	t_contextoEjec *contextoActualizado;
	while (1) {
		int codigo = recibir_operacion(socketCPU);

		switch (codigo) {
		case CONTEXTO:
			void *buffer = recibir_buffer(&tamanio, socketCPU);
			contextoActualizado = deserializarContexto(buffer, tamanio);
			ultimoEjecutado->contexto = contextoActualizado;
			log_info(loggerKernel, "PID:%d  - PC:%d", contextoActualizado->pid, contextoActualizado->PC);
			//TODO Eliminar, linea para visualizar el mandado de info
			free(buffer);
			break;
		case EXIT:
			finalizarProceso(ultimoEjecutado, "SUCCESS");
			break;
		case YIELD:
			if(strcmp(Algoritmo(),"HRRN")==0){
				tiempoEnCPU(ultimoEjecutado);
			}
			agregarAEstadoReady(ultimoEjecutado);
			sem_post(&cpuOcupada);
			break;

		case WAIT:
			char* recursoAConsumir = "recurso"; //= deserializarInstruccion(); Supongamos que recibimos el parametro. Hay q ver como serializa dany
			implementacionWyS(recursoAConsumir, 1);
			break;
		case SIGNAL:
			char* recursoALiberar = "recurso"; // = deserializarInstruccion();
			implementacionWyS(recursoALiberar, 2);
            procesoAEjecutar(contextoActualizado); //vuelve a enviar el contexto a ejecucion
			break;
		case CREATE_SEGMENT:
			break;
		case DELETE_SEGMENT:
			break;

		case IO:
			int tiempoDeIO = 1000; //en microsegundos tendriamos q deserializarInstruccion y seria el primer parametro
			pthread_t hiloDeBloqueo; //crear hilo
			pthread_create(&hiloDeBloqueo, NULL, (void*)bloquearHilo, (void*) &tiempoDeIO);
			pthread_detach(hiloDeBloqueo);
			break;

		default:
			break;
		}
	}
}

void algoritmoFIFO() {
	t_pcb *procesoAEjec = extraerDeReady();
	t_contextoEjec *contextoAEjec = procesoAEjec->contexto;
	procesoAEjecutar(contextoAEjec);
	procesoAEjec->estadoPcb = EXEC;
	cambioDeEstado(procesoAEjec, "READY", "EXEC");
	ultimoEjecutado = procesoAEjec;
}


void algoritmoHRRN(){
	t_pcb* procesoAEjec;
	list_iterate(colaReady, (void*) calcularNuevaEstimacion);
	list_iterate(colaReady, (void*) calcularRR);
	list_sort(colaReady, (void*)comparadorRR);//Aca me ordena la lista por el comparador de RR
	procesoAEjec=extraerDeReady();
	t_contextoEjec * contextoAEjec = procesoAEjec->contexto;
	procesoAEjecutar(contextoAEjec);
	procesoAEjec->estadoPcb=EXEC;
	cambioDeEstado(procesoAEjec, "READY", "EXEC");
	clock_gettime(CLOCK_REALTIME, &(procesoAEjec->llegadaACPU));//Por HRRN
	ultimoEjecutado = procesoAEjec;
}

void calcularNuevaEstimacion(t_pcb* proceso) {
	double alfa = Alfa();
    double nuevaEstimacion = (alfa * proceso->ultimaRafagaEjecutada)+ (proceso->estimadoReady *(1 - alfa));
    proceso->estimadoReady = nuevaEstimacion;
}

void tiempoEnCPU(t_pcb* proceso){
    struct timespec end;
    clock_gettime(CLOCK_REALTIME, &end);

    long seconds = end.tv_sec - proceso->llegadaACPU.tv_sec;
    long nanoseconds = end.tv_nsec - proceso->llegadaACPU.tv_nsec;
    double elapsed = seconds + nanoseconds*1e-9;

    proceso->ultimaRafagaEjecutada=elapsed;

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

//////////////////////INSTRUCCIONES Y PROCESOS////////////////////////////

t_list* obtenerInstrucciones(int socket_cliente) {
	int codigoOp = recibir_operacion(socket_cliente);
	t_list *listaDeInstrucciones; //Recordar liberar esto cuando terminemos->se libera cuando termina el pcb
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
	t_pcb *procesoNuevo = crearPcb(instrucciones, pid, consolaNueva);
	pid++;
	pthread_mutex_unlock(&mutexPID);
	agregarAEstadoNew(procesoNuevo);
	log_info(loggerKernel, "Se crea el proceso %d en New",
			procesoNuevo->contexto->pid);
	log_info(loggerKernel,"%s",list_get(instrucciones,0));
	//Cerrando recursos
	//close(consolaNueva);//Ver si esta demas esto o nos romperia
	free(socket_cliente); //duda de si esta bien el free o puede romper en la conexion aunque no lo creemos
	sem_post(&planiLargoPlazo);
	}
}

void asignarMemoria(t_pcb *procesoNuevo, t_list *tablaDeSegmento) {
	procesoNuevo->tablaSegmentos = tablaDeSegmento;

}

void finalizarProceso(t_pcb *procesoAFinalizar, char* motivoDeFin) {
	free(procesoAFinalizar->contexto->instrucciones);
	free(procesoAFinalizar->contexto);
	// send(socketMemoria,&motivo, sizeof(uint32_t)); //Solicita a memoria que elimine la tabla de segmentos
	free(procesoAFinalizar);
	sem_post(&multiprogramacion);
	int terminar = -1;
	send(procesoAFinalizar->socketConsola, &terminar, sizeof(int), 0); //Avisa a consola que finalice
	log_info(loggerKernel, "Finaliza el proceso %d - Motivo:%s",
			procesoAFinalizar->contexto->pid, motivoDeFin);
	sem_post(&cpuOcupada);

}

///---------RECURSOS COMPARTIDOS PARA WAIT Y SIGNAL----///
void implementacionWyS (char* nombreRecurso, int nombreInstruccion){
	/*1ero buscar el recurso: si no está finaliza el proceso
	 -----------                         si esta decrementa o incrementa la instancia*/
	int posicionRecurso = recursoDisponible(nombreRecurso);
		if(posicionRecurso ==-1){
			finalizarProceso(ultimoEjecutado, "SEG_FAULT");
		}
		else{
			int valor = *(int*)list_get(listaDeInstancias,posicionRecurso);
			switch (nombreInstruccion){
			case 1: //1=WAIT
				valor --;
				log_info(loggerKernel, "PID: %d - WAIT: %s - Instancias: %d", ultimoEjecutado->contexto->pid, nombreRecurso, valor);
				 if(valor<0){
				t_queue* colaDeBloqueo = (t_queue*)list_get(listaDeBloqueo, posicionRecurso);
				queue_push(colaDeBloqueo, ultimoEjecutado);
				tiempoEnCPU(ultimoEjecutado);
				ultimoEjecutado->estadoPcb = BLOCK;
				cambioDeEstado(ultimoEjecutado, "EXEC", "BLOCK");
				log_info(loggerKernel, "PID: %d -Bloqueado por %s:", ultimoEjecutado->contexto->pid, nombreRecurso);
				sem_post(&cpuOcupada);
				 }
				break;
			case 2://2=SIGNAL
				valor ++;
				log_info(loggerKernel, "PID: %d - SIGNAL: %s - Instancias: %d", ultimoEjecutado->contexto->pid, nombreRecurso, valor);
				t_queue* colaDeBloqueo = (t_queue*)list_get(listaDeBloqueo, posicionRecurso);
				t_pcb* procesoDesbloqueado = queue_pop(colaDeBloqueo);
				agregarAEstadoReady(procesoDesbloqueado);
				cambioDeEstado(procesoDesbloqueado,"BLOCK" ,"READY");
				break;
			 }
			}
		}

////---Funcion de IO---///
void bloquearHilo(int* tiempo){
    sem_post(&cpuOcupada);//Lo primero es liberar la cpu
	int tiempoDeBloqueo = *tiempo;
	ultimoEjecutado->estadoPcb= BLOCK;
	log_info(loggerKernel, "PID: %d - Bloqueado por: IO", ultimoEjecutado->contexto->pid);
    cambioDeEstado(ultimoEjecutado, "EXEC", "BLOCK");
	usleep(tiempoDeBloqueo);
	agregarAEstadoReady(ultimoEjecutado); //Agrega a la cola y cambia el estado del pcb
	cambioDeEstado(ultimoEjecutado, "BLOCK", "READY");
}
