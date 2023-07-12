/*
 * recursos.c
 *
 *  Created on: May 25, 2023
 *      Author: utnso
 */


#include "utilsPlanificacion.h"

t_list* listaDeBloqueo;
t_list* listaDeInstancias;
t_queue *colaNew;
t_list *colaReady;
t_list* listaDeProcesos;//procesos admitidos en el sistema
t_pcb *ultimoEjecutado;
uint32_t pid = 0;

void crearEstados() {
	colaNew = queue_create();
	colaReady = list_create();
	listaDeProcesos = list_create();
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
	if(strcmp("HRRN", Algoritmo())==0){
		clock_gettime(CLOCK_REALTIME, &(procesoListo->llegadaAReady));}//Por HRRN
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

void procesoAEjecutar(t_contextoEjec *procesoAEjecutar) {

	t_paquete *paqueteDeContexto = serializarContexto(procesoAEjecutar); // "Pone el contexto en un paquete"
	log_info(loggerKernel, "Paquete de Contexto creado con exito");
	validarEnvioDePaquete(paqueteDeContexto, socketCPU, loggerKernel, configKernel, "Contexto");
}


void finalizarProceso(t_pcb *procesoAFinalizar, char* motivoDeFin) {
	mostrarListaDeProcesos(); //Solo para debugguear
	log_info(loggerKernel, "Finaliza el proceso %d - Motivo:%s",procesoAFinalizar->contexto->pid, motivoDeFin);
	//---Aviso a Consola que finalice---//
	int terminar = -1;
	send(procesoAFinalizar->socketConsola, &terminar, sizeof(int), 0);

	//--Aviso a Memoria que libere la tabla de Segmentos--//
	t_instruccion* instruc = malloc(sizeof(t_instruccion));
	instruc->nombre = EXIT;
	instruc->pid = procesoAFinalizar->contexto->pid;
	t_paquete* paqueteI = serializarInstruccion(instruc);
    validarEnvioDePaquete(paqueteI, socketMemoria, loggerKernel, configKernel, "Eliminar Tabla De Segmentos");
	free(instruc);

	int pidAEliminar = procesoAFinalizar->contexto->pid;
	int pos = posProcesoAEliminar(listaDeProcesos,pidAEliminar); //Esto lo hago porque list_remove_and.. remueve el elemento y corre la posicion para que no quede vacia, o sea , no queda una posicion sin nada adentro
    list_remove_and_destroy_element(listaDeProcesos, pos,(void *)destruirProceso);
	mostrarListaDeProcesos(); //solo para debugguear
    sem_post(&multiprogramacion);
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
				sem_post(&planiCortoPlazo);

				procesoAEjecutar(contextoActualizado);//Sigue en cpu
				break;
			 }
			}
		}

////---Funcion de IO---///
void bloquearHilo(t_parametroIO* parametro){
    t_pcb* procesoBloqueado = parametro->procesoABloquear;
    int tiempoDeBloqueo = parametro->tiempoDeBloqueo;
	tiempoEnCPU(procesoBloqueado);
	procesoBloqueado->estadoPcb= BLOCK;
	log_info(loggerKernel, "PID: %d - Bloqueado por: IO", procesoBloqueado->contexto->pid);
    logCambioDeEstado(procesoBloqueado, "EXEC", "BLOCK");
	usleep(tiempoDeBloqueo);
	agregarAEstadoReady(procesoBloqueado); //Agrega a la cola y cambia el estado del pcb
	logCambioDeEstado(procesoBloqueado, "BLOCK", "READY");
	free(parametro);
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
			break;
		case ERROR:
			finalizarProceso(ultimoEjecutado, "OUT OF MEMORY");
			break;
		case OK: //hasta que le encontremos un uso a la base
			log_info(loggerKernel, "Segmento creado con Exito en Memoria");
			procesoAEjecutar(contexto);
			break;
	}
}
//Actualizar la tabla de segmentos de todos los procesos//
void actualizarTablaEnProcesos(t_list* listaDeTablas){
	int tamanio = list_size(listaDeProcesos);
	for(int i=0; i<tamanio; i++){
		t_pcb* proceso =list_get(listaDeProcesos,i);
		int posDeTabla = posTablaEnLista(listaDeTablas, proceso->contexto->pid);
		  t_list* tablaDeSegmentos = list_get(listaDeTablas,posDeTabla);
		  proceso->tablaSegmentos = tablaDeSegmentos;
		}
	}


t_list* crearListaDeBloqueo(){
	char** vectorDeRecursos = Recursos(); //la funcion devuelve una array de strings con un null al final

	//---Creo las colas y voy llenando la lista---//
	t_list* listColasPorRecurso = list_create(); //creo una lista que contiene en cada posicion una cola de bloqueados para el recurso
	int i = 0;
	while(vectorDeRecursos[i] != NULL){
		t_queue* colaDeBloqueo = queue_create();
		list_add(listColasPorRecurso, colaDeBloqueo);
		i++;
	}

	return listColasPorRecurso;
	}


t_list* crearListaDeInstancias(){

	//--Casteo el vector de strings de instancias a una lista de enteros, lo pense como un vector pero alta paja el retorno--//
	char** instanciasDeRecursos = Instancias();

	t_list* listaDeInstancias = list_create();
	int i =0;
	while(instanciasDeRecursos[i] != NULL){
		char* stringCantidad = instanciasDeRecursos[i];
		int cantidad = atoi(stringCantidad); // se puede usar tambien strtol(stringCantidad, NULL, 10)
		int* cant = malloc(sizeof(int));
		*cant = cantidad;
		list_add(listaDeInstancias, (void*)cant);

		i++;
	}
	return listaDeInstancias;
}

void crearEstructurasDeRecursos(){ //Son globales
	listaDeBloqueo = crearListaDeBloqueo();
	listaDeInstancias = crearListaDeInstancias();
}

int recursoDisponible(char* nombre){
	char** recursos = Recursos();
	int i = 0;
	while(recursos[i] != NULL){
		char* recurso = recursos[i];
		if(strcmp(recurso,nombre)==0){
		 return i;
		}
		i++;
	}
	return -1;
}

//Logueo de procesos en sistema //
//Logueo de Lista de Procesos
void mostrarListaDeProcesos(){
	int tamanio = list_size(listaDeProcesos);
	for(int i=0; i<tamanio; i++){
		t_pcb* proceso = list_get(listaDeProcesos,i);
		log_info(loggerKernel, "Posicion %d de la Lista con Proceso de id <%d>", i, proceso->contexto->pid);
	}
}


int posProcesoAEliminar(t_list* listaDeProcesos, int pidAEliminar ){
	int tamanio = list_size(listaDeProcesos);
	int i = 0;
	while(i<tamanio){
			t_pcb* proceso = list_get(listaDeProcesos, i);
			if(proceso->contexto->pid == pidAEliminar ){
			 return i;
			}
			i++;
		}
		return -1;

}


