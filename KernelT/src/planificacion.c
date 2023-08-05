/*
 * planificacion.c
 *
 *  Created on: Apr 28, 2023
 *      Author: utnso
 */

#include "planificacion.h"

struct timespec begin, end;
int operacionFS = 0;


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
				//mostrarColaReady();
				ordenarReady();
				t_pcb* procesoAEjec=list_remove(colaReady,0);
				log_info(loggerKernel, "%s: Obtengo el proceso %d de Ready", Algoritmo(), procesoAEjec->contexto->pid);
				t_contextoEjec * contextoAEjec = procesoAEjec->contexto;
				procesoAEjecutar(contextoAEjec);
				clock_gettime(CLOCK_REALTIME, &begin);//Tomo tiempo en que ingresó el proceso en CPU
				procesoAEjec->estadoPcb = EXEC;
				logCambioDeEstado(procesoAEjec, "READY", "EXEC");
				//pthread_mutex_lock(&mutexUltimoEjecutado);
				//pthread_mutex_unlock(&mutexUltimoEjecutado);
				instruccionAEjecutar(procesoAEjec);

			}
			else{log_info(loggerKernel, "No hay procesos en Ready para extraer");}
	}
}

void instruccionAEjecutar(t_pcb* ultimoEjecutado) {
		int tamanioBuffer = 0;
		int desplazamiento = 0;
		//Recepcion del contexto//
		int codContexto = recibir_operacion(socketCPU); //Se recibe un codigo :PAQUETE
		void *buffer = recibir_buffer(&tamanioBuffer, socketCPU); //retorna el buffer y almacena el tamanio total del buffer
		if(codContexto == PAQUETE){
			log_info(loggerKernel, "Se recibio el buffer del Contexto e Instruccion");}
		int tamContexto = recibirTamContexto(buffer, &desplazamiento);
		t_contextoEjec* contextoActualizado = deserializarContexto(buffer, tamContexto, &desplazamiento);
		log_info(loggerKernel, "Contexto recibido con pid : %d", contextoActualizado->pid);
		ultimoEjecutado->contexto = contextoActualizado;
		//Recepcion de una instruccion//
		int tipoInstruccion = recibirTipoInstruccion(buffer, &desplazamiento);
		t_instruccion* instruccion;
		switch(tipoInstruccion){
			case EXIT:
				log_info(loggerKernel, "Instruccion EXIT");
				 instruccion = deserializarInstruccionEstructura(buffer, 0, &desplazamiento);
				free(instruccion);
				finalizarProceso(ultimoEjecutado, "SUCCESS");
				//sem_post(&planiCortoPlazo);
				break;
			case YIELD:
				log_info(loggerKernel, "Instruccion YIELD");
				 instruccion = deserializarInstruccionEstructura(buffer, 0, &desplazamiento);
				free(instruccion);
				finTiempoEnCPU(ultimoEjecutado);
				agregarAEstadoReady(ultimoEjecutado);
				//sem_post(&cpuLibre);
				sem_post(&planiCortoPlazo);
				break;

			case WAIT:
				log_info(loggerKernel, "Instruccion WAIT");
				instruccion = deserializarInstruccionEstructura(buffer, 1, &desplazamiento);;
				log_info(loggerKernel, "Recurso a consumir : %s", instruccion->param1);
				char* recursoAConsumir = instruccion->param1;
				implementacionWyS(recursoAConsumir, 1, ultimoEjecutado);
				free(instruccion);
				//sem_post(&planiCortoPlazo);
				break;
			case SIGNAL:
				log_info(loggerKernel, "Instruccion SIGNAL");
				instruccion = deserializarInstruccionEstructura(buffer, 1, &desplazamiento);;
				char* recursoALiberar = instruccion->param1;
				free(instruccion); //Para mi hay q liberar el puntero a la instruccion, una vez q obtenemos el parametro
				implementacionWyS(recursoALiberar, 2, ultimoEjecutado);

				break;
			case IO:
				log_info(loggerKernel, "Instruccion IO");
				finTiempoEnCPU(ultimoEjecutado);
				instruccion = deserializarInstruccionEstructura(buffer, 1, &desplazamiento);
				finTiempoEnCPU(ultimoEjecutado); // VEEEEER ESTOOOOOOOO
				char* tiempo = instruccion->param1;
				free(instruccion);//Hay q liberar puntero
				log_info(loggerKernel, "PID: %d - Ejecuta IO: %d", ultimoEjecutado->contexto->pid, atoi(tiempo));
				t_parametroIO* parametro = malloc(sizeof(t_parametroIO)) ;
				parametro->tiempoDeBloqueo = atoi(tiempo);
				//pthread_mutex_lock(&mutexUltimoEjecutado);
				parametro->procesoABloquear = ultimoEjecutado;
				//pthread_mutex_lock(&mutexUltimoEjecutado);
				pthread_t hiloDeBloqueo; //crear hilo
				pthread_create(&hiloDeBloqueo, NULL, (void*)bloquearHilo, (void*)parametro);
				pthread_detach(hiloDeBloqueo);
				break;
			case MOV_IN:
				log_info(loggerKernel, "Instruccion MOV_IN Fallida");
				instruccion = deserializarInstruccionEstructura(buffer, 2, &desplazamiento);
				free(instruccion);
				finalizarProceso(ultimoEjecutado, "SEG_FAULT");
				//sem_post(&planiCortoPlazo);

				break;
			case MOV_OUT:
				log_info(loggerKernel, "Instruccion MOV_OUT Fallida");
				instruccion= deserializarInstruccionEstructura(buffer, 2, &desplazamiento);
				free(instruccion);
				finalizarProceso(ultimoEjecutado, "SEG_FAULT");
				//sem_post(&planiCortoPlazo);
				break;
			case CREATE_SEGMENT://El proceso sigue en cpu
				log_info(loggerKernel, "Instruccion Create Segment");
				instruccion = deserializarInstruccionEstructura(buffer, 2, &desplazamiento);
				log_info (loggerKernel, "Codigo de operacion de instruc: %d", instruccion->nombre);
				int idSegmentoCS = atoi(instruccion->param1);
				int tamanioSegmento = atoi(instruccion->param2);
				log_info(loggerKernel,"PID: %d - Crear Segmento - Id: %d - Tamaño: %d", contextoActualizado->pid,idSegmentoCS,tamanioSegmento);
				//Serializamos y enviamos a memoria//
				t_paquete* paqueteCS = serializarInstruccion(instruccion);
				validarEnvioDePaquete(paqueteCS, socketMemoria, loggerKernel, configKernel, "Instruccion Create Segment");
                //Funcion que recibe un sed y valida si Memoria pudo crear un segmento//
				validarCS(socketMemoria,instruccion, ultimoEjecutado);
				free(instruccion);

				break;
			case DELETE_SEGMENT: //El proceso sigue en cpu
				log_info(loggerKernel, "Instruccion Delete Segmente");
				instruccion = deserializarInstruccionEstructura(buffer, 1, &desplazamiento);
				int idSegmentoDS = atoi(instruccion->param1);
				log_info(loggerKernel, "PID: %d - Eliminar Segmento - Id Segmento: %d",contextoActualizado->pid,idSegmentoDS);
				//Serializamos y enviamos a memoria//
				t_paquete* paqueteDS = serializarInstruccion(instruccion);
				validarEnvioDePaquete(paqueteDS, socketMemoria, loggerKernel, configKernel, "Instruccion Delete Segment");
				free(instruccion);
				//Recibimos la tabla de segmentos actualizada//
				recibirYAsignarTablaDeSegmentos(ultimoEjecutado);
				//El proceso sigue en ejecucion asi que lo volvemos a enviar a cpu
				procesoAEjecutar(contextoActualizado);
				instruccionAEjecutar(ultimoEjecutado);

				break;
			case F_OPEN:
				log_info(loggerKernel, "Instruccion F Open ");
				instruccion = deserializarInstruccionEstructura(buffer, 1, &desplazamiento);
				log_info(loggerKernel,"PID: <%d> - Abrir Archivo: <%s>", instruccion->pid, instruccion->param1);
                int posArchivo = buscarArchivoEnTGAA(instruccion->param1);
                t_paquete* paquete;
                int recepcion;
				if (posArchivo != -1){
					log_info(loggerKernel, "Archivo: <%s> ya existente", instruccion->param1);
					t_archivo* archivo = list_get(tablaGlobalDeArchivos, posArchivo);
					archivo->contador += 1; //incremento el contador, es la cantidad de procesos esperando
					agregarEntradaATablaxProceso(instruccion->param1, ultimoEjecutado, 0); //Agrega la entrada y posiciona el puntero en cero
					bloquearProcesoPorArchivo(instruccion->param1, ultimoEjecutado);
					/*paquete = serializarInstruccion(instruccion);
					validarEnvioDePaquete(paquete, socketFs, loggerKernel, configKernel, "Instruccion F OPEN");
					free(instruccion);
				     recepcion = recibir_operacion(socketFs);*/
					}
				else { //si no existe en la TGAA
					//1) Consulta si existe ese archivo
					paquete = serializarInstruccion(instruccion);
					validarEnvioDePaquete(paquete, socketFs, loggerKernel, configKernel, "Instruccion F OPEN");
					recepcion = recibir_operacion(socketFs);
					t_instruccion* instrucFCreate;
						if(recepcion == ERROR ){ // Solo si no existe envia un F Create
							instrucFCreate = malloc(sizeof(t_instruccion));
							instrucFCreate->nombre = F_CREATE;
							instrucFCreate->pid = contextoActualizado->pid;
							instrucFCreate->param1 = instruccion->param1;
							t_paquete* paq = serializarInstruccion(instrucFCreate);
							validarEnvioDePaquete(paq, socketFs, loggerKernel, configKernel, "Instruccion F CREATE");
							//Si es la primera vez que crea un archivo-> Debe iniciar una cola de block para ese archivo
							crearColaBlockDeArchivo(instruccion->param1);

						}
						//Exista o no exista debe hacer esto//
						//Agrego una entrada a la Tabla Global De Archivos Abiertos//
						t_archivo* archivo = malloc (sizeof(t_archivo));
						archivo->nombreArchivo = instruccion->param1;
						archivo->contador = 0; //Es el contador de cuantos procesos esperan al archivo
						list_add(tablaGlobalDeArchivos, archivo);
						//Agrego entrada a la Tabla de Archivos para este proceso//
						agregarEntradaATablaxProceso(instruccion->param1, ultimoEjecutado, 0);

						free(instruccion); //ver si luego no genera inconsistencias
						free( instrucFCreate);
						procesoAEjecutar(ultimoEjecutado->contexto);
						instruccionAEjecutar(ultimoEjecutado);
						}


				break;
			case F_CLOSE:
				log_info(loggerKernel, "Instruccion F CLOSE");
				instruccion=deserializarInstruccionEstructura(buffer, 1, &desplazamiento);
				//Cierro el arch en la tabla del proceso
				int posArchProceso =buscarArchivoEnProceso(instruccion->param1, ultimoEjecutado);
				t_list* listaDeArchs = ultimoEjecutado->archAbiertos;
				list_remove_and_destroy_element(listaDeArchs, posArchProceso, (void*) cerrarArchivoDeProceso); //Se borra el archivoPorProceso q es distinto al archivo del TGA
				t_pcb* proceso;
				//Modifico el contador en la TGAA
				int pos = buscarArchivoEnTGAA(instruccion->param1);
				t_archivo* archivo = list_get(tablaGlobalDeArchivos, pos);
				//int instancias = archivo->contador; //Ya se que esto parece  re para nada, pero sino creo que me tira cualquier numero

				if(archivo->contador > 0){
					t_colaDeArchivo* colaArchivo = buscarColaDeArchivo(archivo->nombreArchivo);
					proceso = queue_pop(colaArchivo->colaBlock);
					archivo->contador -= 1;
					agregarAEstadoReady(proceso);
					sem_post(&planiCortoPlazo);


				}else{
					int posColaDeBlock = posColaDeArchivo(archivo->nombreArchivo);
					if(posColaDeBlock!=-1){
					list_remove_and_destroy_element(listaDeColasPorArchivo, posColaDeBlock , (void*) queue_destroy);
					}
					list_remove_and_destroy_element(tablaGlobalDeArchivos, pos, (void*)cerrarArchivoEnTGAA);
					/*archivoProceso->nombreArchivo=NULL;
					archivoProceso->puntero=0;
					free(archivoProceso);*/
				}
				free(instruccion);
				procesoAEjecutar(ultimoEjecutado->contexto);
				instruccionAEjecutar(ultimoEjecutado);
				break;

			case F_SEEK:
				log_info(loggerKernel, "Instruccion F SEEK");
				instruccion = deserializarInstruccionEstructura(buffer, 2, &desplazamiento);
				posicionarPuntero(instruccion->param1,ultimoEjecutado, instruccion->param2);
				t_paquete* paqueteFS = serializarInstruccion(instruccion);
				validarEnvioDePaquete(paqueteFS, socketFs, loggerKernel, configKernel, "Instruccion F SEEK");
				free(instruccion);
				procesoAEjecutar(ultimoEjecutado->contexto);
				instruccionAEjecutar(ultimoEjecutado);

				break;
			case F_READ:
				log_info(loggerKernel, "Instruccion F READ");
				instruccion = deserializarInstruccionEstructura(buffer, 3, &desplazamiento);
                validarRyW(instruccion->param2, ultimoEjecutado);
                //Serializa la instruccion ,la manda a FS y bloquea al proceso //
                t_parametroFS* paramR = malloc(sizeof(t_parametroFS));
                paramR->proceso = ultimoEjecutado;
                paramR->instruccion=instruccion;
                pthread_t hiloRead;
        		pthread_create(&hiloRead, NULL ,(void*) implementacionF,(void*) paramR);
                pthread_detach(hiloRead);

				break;
			case F_WRITE:
				log_info(loggerKernel, "Instruccion F WRITE");
				instruccion = deserializarInstruccionEstructura(buffer, 3, &desplazamiento);
                validarRyW(instruccion->param2, ultimoEjecutado);
                //Serializa la instruccion ,la manda a FS y bloquea al proceso //
				t_parametroFS* paramW = malloc(sizeof(t_parametroFS));
				paramW->proceso = ultimoEjecutado;
				paramW->instruccion=instruccion;
				pthread_t hiloWrite;
				pthread_create(&hiloWrite, NULL ,(void*) implementacionF,(void*) paramW);
				pthread_detach(hiloWrite);
				break;
			case F_TRUNCATE:
				log_info(loggerKernel, "Instruccion F TRUNCATE");
				instruccion = deserializarInstruccionEstructura(buffer, 2, &desplazamiento);
				//Serializa la instruccion ,la manda a FS y bloquea al proceso //
				t_parametroFS* paramT = malloc(sizeof(t_parametroFS));
				paramT->proceso = ultimoEjecutado;
				paramT->instruccion=instruccion;
				pthread_t hiloTruncate;
				pthread_create(&hiloTruncate, NULL ,(void*) implementacionF,(void*) paramT);
				pthread_detach(hiloTruncate);

				break;
			/*case(-1):
				log_info(loggerKernel, "Error al recibir el codigo de operacion. Hemos finalizado la Conexion "); //Esto es porque el recibir_op retorna un -1 si hubo error y nunca lo consideramos
				//ver que hacer cuando pasa esto*/
			break;
		}
		free(buffer);
	}

//---ALGORITMOS---///

void finTiempoEnCPU(t_pcb* proceso){
 if(strcmp("HRRN", Algoritmo())==0){

	clock_gettime(CLOCK_REALTIME, &end);
	double elapsed = ((end.tv_sec - begin.tv_sec)*1000) + ((end.tv_nsec - begin.tv_nsec)*1e-6);

	proceso->ultimaRafagaEjecutada = elapsed;

	calcularNuevaEstimacion(proceso);
 }

}

void ordenarReady(){
	if (strcmp(Algoritmo(), "HRRN")==0){
		log_info(loggerKernel, "Cola Ready ordenada por HRRN");
		list_iterate(colaReady, (void*) calcularRR);
		list_sort(colaReady, (void*)comparadorRR);
		mostrarColaReady();
	}
	else {
		log_info(loggerKernel, "Cola Ready ordenada por FIFO");} //se sabe q no ordena nada solo es un log
}



void calcularRR(t_pcb* proceso) {

	struct timespec endd;
	clock_gettime(CLOCK_REALTIME, &endd);

	double elapsed = ((endd.tv_sec - (proceso->llegadaAReady.tv_sec))*1000) + ((endd.tv_nsec - (proceso->llegadaAReady.tv_nsec))*1e-6);

    proceso->tiempoDeEspera = elapsed;
	proceso->RR = 1 + ((proceso->tiempoDeEspera )/ (proceso->estimadoRafaga));

}

bool comparadorRR(t_pcb* proceso1, t_pcb* proceso2) {

    return (proceso1->RR) >= (proceso2->RR);

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
	proceso->tablaSegmentos = tablaDeSegmentos;
	loggearTablaDeSegmentos(tablaDeSegmentos, loggerKernel);

}

///UTILS DE INSTRUCCIONES///
///---------RECURSOS COMPARTIDOS PARA WAIT Y SIGNAL----///
void implementacionWyS (char* nombreRecurso, int nombreInstruccion, t_pcb* ultimoEjecutado){
	/*1ero buscar el recurso: si no está finaliza el proceso
	 -----------                         si esta decrementa o incrementa la instancia*/
	int posicionRecurso = recursoDisponible(nombreRecurso);
		if(posicionRecurso ==-1){
			finalizarProceso(ultimoEjecutado, "Recurso No Existente");
		}
		else{
			int* valor = list_get(listaDeInstancias,posicionRecurso);
			switch (nombreInstruccion){
			case 1: //1=WAIT
				*valor -= 1;
				log_info(loggerKernel, "PID: %d - WAIT: %s - Instancias: %d", ultimoEjecutado->contexto->pid, nombreRecurso, *valor);
				 if(*valor<0){
				t_queue* colaDeBloqueo = (t_queue*)list_get(listaDeBloqueo, posicionRecurso);
				queue_push(colaDeBloqueo, ultimoEjecutado);
				finTiempoEnCPU(ultimoEjecutado);
				ultimoEjecutado->estadoPcb = BLOCK;
				logCambioDeEstado(ultimoEjecutado, "EXEC", "BLOCK");
				log_info(loggerKernel, "PID: %d -Bloqueado por %s:", ultimoEjecutado->contexto->pid, nombreRecurso);
				 }
				 else{
					 procesoAEjecutar(ultimoEjecutado->contexto); //Sigue en cpu
					 instruccionAEjecutar(ultimoEjecutado);
					 }
				break;
			case 2://2=SIGNAL
				*valor += 1;
				log_info(loggerKernel, "PID: %d - SIGNAL: %s - Instancias: %d", ultimoEjecutado->contexto->pid, nombreRecurso, *valor);
				t_queue* colaDeBloqueo = (t_queue*)list_get(listaDeBloqueo, posicionRecurso);
				if(!queue_is_empty(colaDeBloqueo)){
				t_pcb* procesoDesbloqueado = queue_pop(colaDeBloqueo);
				agregarAEstadoReady(procesoDesbloqueado);
				logCambioDeEstado(procesoDesbloqueado,"BLOCK" ,"READY");
				sem_post(&planiCortoPlazo);
				}
				procesoAEjecutar(ultimoEjecutado->contexto);//Sigue en cpu
				instruccionAEjecutar(ultimoEjecutado);
				break;
			 }
			}
		}

//Validacion para CreateSegment//
void validarCS(int socketMemoria, t_instruccion* instruccion, t_pcb* ultimoEjecutado){

	int mensaje = recibir_operacion(socketMemoria);
	switch (mensaje) {
		case COMPACTAR:
			log_info(loggerKernel, "Respuesta de Create Segment: COMPACTAR");
			//TODO Validariamos que no haya operaciones esntre Fs y Memoria
			if(operacionFS==1){
			log_info(loggerKernel, "Compactacion: Esperando Fin de Operaciones de FS");
					}
			pthread_mutex_lock(&mutexOperacionFS);
			log_info(loggerKernel, "Compactacion: Se solicito compactacion ");
			int habilitado = COMPACTAR;//solicitariamos a la memoria que compacte enviandole un send de O
			send(socketMemoria, &habilitado, sizeof(int),0);
			t_list* listaDeTablas = deserializarListaDeTablas(socketMemoria);//recibe lista de tablas actualizada y deserializa
			actualizarTablaEnProcesos(listaDeTablas);//funcion que tome cada pcb y setee la nueva tabla correspondiente con su posicion
			pthread_mutex_unlock(&mutexOperacionFS);
			log_info(loggerKernel, "Se Finalizo el proceso de COMPACTACION");
			t_paquete* paqueteCS = serializarInstruccion(instruccion);
			validarEnvioDePaquete(paqueteCS, socketMemoria, loggerKernel, configKernel, "Instruccion Create Segment");
			validarCS(socketMemoria,instruccion,ultimoEjecutado);

			break;
		case ERROR:
			log_info(loggerKernel, "Respuesta de Create Segment: NO HAY MEMORIA SUFICIENTE");
			finalizarProceso(ultimoEjecutado, "OUT OF MEMORY");
			break;
		case OK: //hasta que le encontremos un uso a la base
			log_info(loggerKernel, "Respuesta de Create Segment: SEGMENTO CREADO CON EXITO");
			procesoAEjecutar(ultimoEjecutado->contexto);
			instruccionAEjecutar(ultimoEjecutado);
			break;
		case(-1):
			log_info(loggerKernel, "Error al recibir el mensaje de Memoria. Hemos finalizado la Conexion "); //Esto es porque el recibir_op retorna un -1 si hubo error y nunca lo consideramos
			//ver que hacer cuando pasa esto
			break;

	}
}

//----Funcion General para el hilo en Instruccion Fwrite, FRead, FTruncate---//
void implementacionF(t_parametroFS* parametro){
	t_pcb* proceso = parametro->proceso;
	t_instruccion* instruccion = parametro->instruccion;
	pthread_mutex_lock(&mutexOperacionFS);
	operacionFS = 1;
	t_paquete* paqueteF = serializarInstruccion(instruccion);
	validarEnvioDePaquete(paqueteF, socketFs, loggerKernel, configKernel, "Instruccion a File System");
	proceso->estadoPcb= BLOCK;
	log_info(loggerKernel, "PID: <%d> - Bloqueado por operar sobre el archivo: <%s>", proceso->contexto->pid, instruccion->param1);
	logCambioDeEstado(proceso, "EXEC", "BLOCK");
	finTiempoEnCPU(proceso);

	int finOperacion = recibir_operacion(socketFs);
	if(finOperacion == -1){
		log_info(loggerKernel, "Error al recibir de FS el resultado de la operacion");
	}
	else if( finOperacion == OK){
		log_info(loggerKernel, "Finalizo la Operacion en File System");
		logCambioDeEstado(proceso, "BLOCK", "READY");

		pthread_mutex_unlock(&mutexOperacionFS);
		agregarAEstadoReady(proceso);
		sem_post(&planiCortoPlazo);

		operacionFS = 0;

		free(parametro->instruccion);
		free(parametro->proceso);
		free(parametro);
	}
}

// UTILS FILE SYSTEM
/*
int archivoAbierto (char* nombre, int id){
    t_archivos* archivo;

	int tamanioLista = list_size(archivos);

	for(int j = 0; j < tamanioLista; j++){
		archivo = list_get(archivos, j);

		if(strcmp(archivo->nombreArchivo, nombre) == 0 && archivo->PID == id){
			return j;
		}

	}
    return -1;
}
*/

