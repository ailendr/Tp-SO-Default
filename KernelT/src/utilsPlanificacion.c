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

    //float seconds = end.tv_sec - proceso->llegadaACPU.tv_sec;
    //float nanoseconds = end.tv_nsec - proceso->llegadaACPU.tv_nsec;
    double elapsed = (end.tv_sec - proceso->llegadaACPU.tv_sec) + ((end.tv_nsec - proceso->llegadaACPU.tv_nsec)*1e-9);

    proceso->ultimaRafagaEjecutada=elapsed;
    calcularNuevaEstimacion(proceso);
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
   if(pos != -1){
       destruirTabla(procesoAFinalizar->tablaSegmentos);
       pthread_mutex_lock(&mutexListaDeProcesos);
       list_remove_and_destroy_element(listaDeProcesos, pos, (void*) destruirProceso);
       mostrarListaDeProcesos(); //solo para debugguear
       pthread_mutex_unlock(&mutexListaDeProcesos);

    sem_post(&multiprogramacion);
   }
   else {
	   log_info(loggerKernel, "ERROR: No se encuentra el proceso a eliminar");
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

//Actualizar la tabla de segmentos de todos los procesos//
void actualizarTablaEnProcesos(t_list* listaDeTablas){
	int tamanio = list_size(listaDeProcesos);
	for(int i=0; i<tamanio; i++){
		t_pcb* proceso =list_get(listaDeProcesos,i);
		int posDeTabla = posTablaEnLista(listaDeTablas, proceso->contexto->pid);
		  t_tabla* tablaDeSegmentos = list_get(listaDeTablas,posDeTabla);
		  proceso->tablaSegmentos = tablaDeSegmentos;
          loggearTablaDeSegmentos(tablaDeSegmentos, loggerKernel);

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

//Logueo de procesos en sistema //
//Logueo de Lista de Procesos
void mostrarListaDeProcesos(){
	int tamanio = list_size(listaDeProcesos);
	for(int i=0; i<tamanio; i++){
		t_pcb* proceso = list_get(listaDeProcesos,i);
		log_info(loggerKernel, "Posicion %d de la Lista con Proceso de id <%d>", i, proceso->contexto->pid);
	}
}

void mostrarColaReady(){
	int tamanio = list_size(colaReady);
	for(int i=0; i<tamanio; i++){
		t_pcb* proceso = list_get(colaReady,i);
		log_info(loggerKernel, "Posicion %d de la Lista con Proceso de id <%d>", i, proceso->contexto->pid);
	}
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


