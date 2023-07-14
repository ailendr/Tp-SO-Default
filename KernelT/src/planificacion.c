/*
 * planificacion.c
 *
 *  Created on: Apr 28, 2023
 *      Author: utnso
 */

#include "planificacion.h"



void largoPlazo() {
	while (1) {

		sem_wait(&planiLargoPlazo);
		log_info(loggerKernel, "Pase el semaforo de largo plazo");
		t_pcb *proceso;
		//Pasaje de New a Ready//
		sem_wait(&multiprogramacion);
		log_info(loggerKernel, "Pase el gr de multiprogramacion");//Siempre que entra aca se descuenta el gr de multiprogramacion en el sistema
		proceso = extraerDeNew(colaNew);
		pthread_mutex_lock(&mutexListaDeProcesos);
        list_add_in_index(listaDeProcesos,proceso->contexto->pid,proceso);//Agregamos a la lista de procesos globales
        mostrarListaDeProcesos();
        pthread_mutex_unlock(&mutexListaDeProcesos);
        log_info(loggerKernel, "Solicitando Tabla de Segmentos a Memoria");
        t_instruccion* instruc = malloc(sizeof(t_instruccion));
        instruc->nombre = CREAR_TABLA;
        instruc->pid = proceso->contexto->pid;
        t_paquete* paqueteI = serializarInstruccion(instruc);
        free(instruc);
        validarEnvioDePaquete(paqueteI, socketMemoria, loggerKernel, configKernel, "Crear Tabla de Segmentos");
		recibirYAsignarTablaDeSegmentos(proceso);
		log_info(loggerKernel, "Tabla de segmentos inicial ya asignada a proceso PID: %d", proceso->contexto->pid);
		agregarAEstadoReady(proceso);
		logCambioDeEstado(proceso, "NEW", "READY");
		sem_post(&planiCortoPlazo);
	}
}



void cortoPlazo() {
	while (1) {
		sem_wait(&planiCortoPlazo);
		log_info(loggerKernel, "Corto Plazo habilitado");
		if(!list_is_empty(colaReady)){
				ordenarReady();
				t_pcb* procesoAEjec=extraerDeReady();
				log_info(loggerKernel, "%s: Obtengo el proceso %d de Ready", Algoritmo(), procesoAEjec->contexto->pid);
				t_contextoEjec * contextoAEjec = procesoAEjec->contexto;
				procesoAEjecutar(contextoAEjec);
				procesoAEjec->estadoPcb = EXEC;
				logCambioDeEstado(procesoAEjec, "READY", "EXEC");
				if(strcmp(Algoritmo(), "HRRN")==0){
				clock_gettime(CLOCK_REALTIME, &(procesoAEjec->llegadaACPU));
				}
				//pthread_mutex_lock(&mutexUltimoEjecutado);
				ultimoEjecutado = procesoAEjec;
				//pthread_mutex_unlock(&mutexUltimoEjecutado);
				instruccionAEjecutar();

			}
			else{log_info(loggerKernel, "No hay procesos en Ready para extraer");}
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
//Recepcion de una instruccion//
		int codigo = recibir_operacion(socketCPU);
		switch(codigo){
			case EXIT:
				log_info(loggerKernel, "Intruccion EXIT");
				t_instruccion* instruccionExit= obtenerInstruccion(socketCPU,0);
				free(instruccionExit);
				finalizarProceso(ultimoEjecutado, "SUCCESS");
				//sem_post(&planiCortoPlazo);
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
				//sem_post(&planiCortoPlazo);
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
				free(instruccionIO);//Hay q liberar puntero
				log_info(loggerKernel, "PID: %d - Ejecuta IO: %d", ultimoEjecutado->contexto->pid, atoi(tiempo));
				t_parametroIO* parametro = malloc(sizeof(t_parametroIO)) ;
				parametro->tiempoDeBloqueo = atoi(tiempo);
				parametro->procesoABloquear = ultimoEjecutado;
				pthread_t hiloDeBloqueo; //crear hilo
				pthread_create(&hiloDeBloqueo, NULL, (void*)bloquearHilo, (void*)parametro);
				pthread_detach(hiloDeBloqueo);
				break;
			case MOV_IN:
				log_info(loggerKernel, "Intruccion MOV_IN Fallida");
				t_instruccion* instruccionMI = obtenerInstruccion(socketCPU,2);
				free(instruccionMI);
				finalizarProceso(ultimoEjecutado, "SEG_FAULT");
				//sem_post(&planiCortoPlazo);

				break;
			case MOV_OUT:
				log_info(loggerKernel, "Intruccion MOV_OUT Fallida");
				t_instruccion* instruccionMO = obtenerInstruccion(socketCPU,2);
				free(instruccionMO);
				finalizarProceso(ultimoEjecutado, "SEG_FAULT");
				//sem_post(&planiCortoPlazo);
				break;
			case CREATE_SEGMENT://El proceso sigue en cpu
				log_info(loggerKernel, "Intruccion Create Segment");
				t_instruccion* instruccionCS = obtenerInstruccion(socketCPU,2);// No se estaba recibiendo bien el pid porq en Cpu no se ponia el pid: ARREGLADO
				log_info (loggerKernel, "Codigo de operacion de instruc: %d", instruccionCS->nombre);
				int idSegmentoCS = atoi(instruccionCS->param1);
				int tamanioSegmento = atoi(instruccionCS->param2);
				log_info(loggerKernel,"PID: %d - Crear Segmento - Id: %d - Tamaño: %d", contextoActualizado->pid,idSegmentoCS,tamanioSegmento);
				//Serializamos y enviamos a memoria//
				t_paquete* paqueteCS = serializarInstruccion(instruccionCS);
				validarEnvioDePaquete(paqueteCS, socketMemoria, loggerKernel, configKernel, "Instruccion Create Segment");
				free(instruccionCS);
                //Funcion que recibe un sed y valida si Memoria pudo crear un segmento//
				validarCS(socketMemoria, contextoActualizado);

				break;
			case DELETE_SEGMENT: //El proceso sigue en cpu
				log_info(loggerKernel, "Intruccion Delete Segmente");
				t_instruccion* instruccionDS = obtenerInstruccion(socketCPU,1);
				int idSegmentoDS = atoi(instruccionDS->param1);
				log_info(loggerKernel, "PID: %d - Eliminar Segmento - Id Segmento: %d",contextoActualizado->pid,idSegmentoDS);
				//Serializamos y enviamos a memoria//
				t_paquete* paqueteDS = serializarInstruccion(instruccionDS);
				validarEnvioDePaquete(paqueteDS, socketMemoria, loggerKernel, configKernel, "Instruccion Delete Segment");
				free(instruccionDS);
				//Recibimos la tabla de segmentos actualizada//
				recibirYAsignarTablaDeSegmentos(ultimoEjecutado);
				//El proceso sigue en ejecucion asi que lo volvemos a enviar a cpu
				procesoAEjecutar(contextoActualizado);
				instruccionAEjecutar();

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

//---ALGORITMOS---///
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

void calcularNuevaEstimacion(t_pcb* proceso) {
	double alfa = Alfa();
    double nuevaEstimacion = (alfa * proceso->ultimaRafagaEjecutada)+ (proceso->estimadoReady *(1 - alfa));
    proceso->estimadoReady = nuevaEstimacion;
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
	free(socket_cliente); //duda de si esta bien el free o puede romper en la conexion aunque no lo creemos
	sem_post(&planiLargoPlazo);
	}
}

void asignarMemoria(t_pcb *procesoNuevo, t_tabla *tablaDeSegmento) {
	procesoNuevo->tablaSegmentos = tablaDeSegmento;

}

//Recibir Tabla de segmentos//
void recibirYAsignarTablaDeSegmentos(t_pcb* proceso){
	//----Todo lo que implica deserializar una tabla de segmentos----//
	 int size = 0;
	 int desplazamiento = 0;
	 void* bufferTabla = recibir_buffer(&size,socketMemoria);
	t_tabla* tablaDeSegmentos = deserializarTablaDeSegmentos(bufferTabla,&desplazamiento,size);
	free(bufferTabla);
	//---------------//
	loggearTablaDeSegmentos(tablaDeSegmentos);
	asignarMemoria(proceso, tablaDeSegmentos);

}

///UTILS DE INSTRUCCIONES///
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
					 instruccionAEjecutar();
					 }
				break;
			case 2://2=SIGNAL
				valor ++;
				log_info(loggerKernel, "PID: %d - SIGNAL: %s - Instancias: %d", ultimoEjecutado->contexto->pid, nombreRecurso, valor);
				t_queue* colaDeBloqueo = (t_queue*)list_get(listaDeBloqueo, posicionRecurso);
				t_pcb* procesoDesbloqueado = queue_pop(colaDeBloqueo);
				agregarAEstadoReady(procesoDesbloqueado);
				logCambioDeEstado(procesoDesbloqueado,"BLOCK" ,"READY");
				sem_post(&planiCortoPlazo);

				procesoAEjecutar(contextoActualizado);//Sigue en cpu
				instruccionAEjecutar();
				break;
			 }
			}
		}

//Validacion para CreateSegment//
void validarCS(int socketMemoria, t_contextoEjec* contexto){
	uint32_t mensaje = 0;
	recv(socketMemoria, &mensaje, sizeof(uint32_t),0);
	switch (mensaje) {
		case COMPACTAR:
			//TODO Validariamos que no haya operaciones esntre Fs y Memoria
			int habilitado = COMPACTAR;//solicitariamos a la memoria que compacte enviandole un send de OK
			send(socketMemoria, &habilitado, sizeof(int),0);
			t_list* listaDeTablas = deserializarListaDeTablas(socketMemoria);//recibe lista de tablas actualizada y deserializa
			actualizarTablaEnProcesos(listaDeTablas);//funcion que tome cada pcb y setee la nueva tabla correspondiente con su posicion
			procesoAEjecutar(contexto);
			instruccionAEjecutar();
			break;
		case ERROR:
			finalizarProceso(ultimoEjecutado, "OUT OF MEMORY");
			break;
		case OK: //hasta que le encontremos un uso a la base
			log_info(loggerKernel, "Segmento creado con Exito en Memoria");
			procesoAEjecutar(contexto);
			instruccionAEjecutar();
			break;
	}
}
