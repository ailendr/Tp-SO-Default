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
uint32_t pid=1;

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
	 procesosActivos++;//Esto no estoy segura de que deberia incrementar aca
 }

 t_pcb* extraerDeNew(){
 	 t_pcb* proceso = queue_pop(colaNew);

 	 return proceso;
  }

 t_pcb* extraerDeReady(){
	 t_pcb* proceso = list_remove(colaReady,0);//Iba a usar el list_get pero tiene sentido sacar el elemento de la cola y eliminarlo de ahi
	 return proceso;
 }

/*
 int cantProcesosEnMemoria(){
	 int cantidad=0;
	 int procesosEnReady = list_size(colaReady);
	 //int procesosEnNew = queue_size(colaNew);
	 cantidad = procesosEnReady + procesosEnNew;
	 return cantidad;
 }
*/

//Creo que deber{ia estar en KernelConexiones o en otro lado
t_contextoEjec* crearContexto(t_list* instrucciones) {
 	t_contextoEjec* contexto;
 	contexto->instrucciones=instrucciones;
 	contexto->PC=0;
 	return contexto;
 }

 t_pcb* crearPcbNuevo (t_list* instrucciones, uint32_t pid, estadoPcb estado){
	    t_pcb* pcb = malloc(sizeof(t_pcb*));
	    t_contextoEjec* contexto = malloc(sizeof(t_contextoEjec*));
	    contexto=crearContexto(instrucciones);

	 	pcb->PID = pid;
	 	pcb->contexto;
	 	pcb->tablaSegmentos = list_create();//recibir por parametro
	 	pcb->estadoPcb = estado;
	 	pcb->estimadoReady = Estimacion();
	 	pcb->llegadaAReady = 0;
	 	pcb->archAbiertos = list_create();//recibir por parametro
	 	return pcb;
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
	 int maxGradoMultiprogram = Multiprogramacion();
	 if(procesosActivos <= maxGradoMultiprogram){
		 proceso = extraerDeNew(colaNew);
		 agregarAEstadoReady(proceso);
	 }
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
