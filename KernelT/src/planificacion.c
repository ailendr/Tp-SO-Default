/*
 * planificacion.c
 *
 *  Created on: Apr 28, 2023
 *      Author: utnso
 */

#include "planificacion.h"


t_queue* colaNew ;
t_list* colaReady ;
int procesosActivos=0;


void crearEstados(){
  colaNew = queue_create();
  colaReady = list_create();
}

 void agregarAEstadoNew(t_pcb* procesoNuevo){
 queue_push(colaNew, procesoNuevo);
 procesoNuevo->estadoPcb = NEW;
 }

 void agregarAEstadoReady(t_pcb* procesoListo){
	 list_add(colaReady, procesoListo);
	 procesoListo->estadoPcb = READY;

	 log_info(loggerKernel, "Cola Ready con algoritmo %s .Ingresa el proceso con id %d:", Algoritmo(), procesoListo->PID);
 }

 t_pcb* extraerDeNew(){
 	 t_pcb* proceso = queue_pop(colaNew);

 	 return proceso;
  }

 t_pcb* extraerDeReady(){
	 t_pcb* proceso = list_remove(colaReady,0);//Iba a usar el list_get pero tiene sentido sacar el elemento de la cola y eliminarlo de ahi
	 return proceso;
 }



void procesoAEjecutar(t_contextoEjec* procesoAEjecutar){ //TODO
	//Aca tendría que llamar a lo de serializar, dejo un tipo provisional asi no rompe
	uint32_t paquete = 0;
	uint32_t paqueteRecibido;
	 send(socketCPU,&paquete, sizeof(uint32_t), 0);
     recv(socketCPU, &paqueteRecibido,sizeof(uint32_t),MSG_WAITALL);

	//Aca habria que deserializar y actualizar el proceso
	//t_contextoEjec* procesoDeserializado;
	//funcion que coloque ese nuevo contexto recibido al pcb enviado
}

 void largoPlazo(){
	 t_pcb* proceso;
	 //Con un semaforo tendriamos que ver lo de la multiprogramacion, esto lo dejo hasta que los implementemos
	 //int procesosActivos = cantProcesosEnMemoria();

	 //Pasaje de New a Ready//
	 int maxGradoMultiprogram = Multiprogramacion(); //Tiene que ser un Globals
	 if(procesosActivos <= maxGradoMultiprogram){
		 proceso = extraerDeNew(colaNew);
		 enviarProtocolo(socketMemoria, HANDSHAKE_PedirMemoria,loggerKernel);//El handshake seria el pedido de memoria
		 //asignarMemoria(proceso, tabla); //PCB creado
		 agregarAEstadoReady(proceso);
	     procesosActivos ++;
	     log_info(loggerKernel, "PID: %d - Estado Anterior: NEW - Estado Actual: READY", proceso->PID);
	 }

	 }
 void finalizarProceso(t_pcb* procesoAFinalizar){
	 free(procesoAFinalizar->contexto);
	// send(socketMemoria,&motivo, sizeof(uint32_t)); //Solicita a memoria que elimine la tabla de segmentos
	 free(procesoAFinalizar);
	 log_info(loggerKernel,"Finaliza el proceso %d - Motivo: SUCCESS", procesoAFinalizar->PID);


 }


 void cortoPlazo(){
	 char* algoritmo = Algoritmo();
	 if(strcmp(algoritmo,"FIFO")){
		 algoritmoFIFO();
	 }
 }

 void algoritmoFIFO(){
	 t_pcb* procesoAEjec = extraerDeReady();
	 t_contextoEjec* contextoAEjec = procesoAEjec->contexto; //podria usar el concepto de encapsulamiento y q pcb sepa retornarme su contexto pero wee
	 procesoAEjecutar(contextoAEjec);

 }
/// ---------TODO LO QUE TIENE QUE VER CON INSTRUCCIONES---------///

 t_list* obtenerInstrucciones(int socket_cliente){
	 int codigoOp = recibir_operacion(socket_cliente);
	 t_list* listaDeInstrucciones; //Recordar liberar esto cuando terminemos
	 uint32_t recepcion = 1; //Consola deberia verificar que si es 1 -> kernel recibio todo OK
	 if(codigoOp == PAQUETE){
	 listaDeInstrucciones = recibir_paquete(socket_cliente);
	 send(socket_cliente, &recepcion, sizeof(uint32_t),0);
	 return listaDeInstrucciones;
   }
 }

 void generarProceso(int* socket_cliente){
	 int consolaNueva = *socket_cliente;
	   recibirProtocolo(socket_cliente); //Handashake
 	  t_list* instrucciones = obtenerInstrucciones(consolaNueva);
 	  t_pcb* procesoNuevo = crearPcb(instrucciones);
 	  agregarAEstadoNew(procesoNuevo);
 	  log_info(loggerKernel, "Se crea el proceso %d en New", procesoNuevo->PID);
   }

   void asignarMemoria(t_pcb* procesoNuevo, t_list* tablaDeSegmento){
 	  procesoNuevo->tablaSegmentos = tablaDeSegmento;
   }




