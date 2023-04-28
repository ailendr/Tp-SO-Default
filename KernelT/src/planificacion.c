/*
 * planificacion.c
 *
 *  Created on: Apr 28, 2023
 *      Author: utnso
 */

#include "planificacion.h"


t_queue* colaNew ;
t_queue* colaReady ;//Esto debería ser una lista por HRRN

void crearColas(){
  colaNew = queue_create();
  colaReady = queue_create();
}

 void agregarAColaNew(t_pcb* procesoNuevo){
 queue_push(colaNew, procesoNuevo);
 procesoNuevo->estadoPcb = NEW;
 }

 void agregarAColaReady(t_pcb* procesoListo){
	 queue_push(colaReady, procesoListo);
	 procesoListo->estadoPcb = READY;
 }

 t_pcb* extraerDeCola(t_queue* cola){
 	 t_pcb* proceso = queue_pop(cola);

 	 return proceso;
  }

 //El grado de multiprogram es todo lo que esté en memoria, los de new cuentan
 /*
 int cantProcesosEnMemoria(){
	 int cantidad=0;
	 int procesosEnReady= queu_size(colaReady);
	 int procesosEnNew=queu_size(colaNew);
	 cantidad = procesosEnReady + procesosEnNew;
	 return cantidad;
 }*/


void procesoAEjecutar(t_pcb* procesoAEjecutar){
	//Aca tendría que llamar a lo de serializar, dejo un tipo provisional asi no rompe
	uint32_t paquete;
	uint32_t paqueteRecibido;
	send(socketCPU,&paquete, sizeof(uint32_t), MSG_WAITALL);
	recv(socketCPU, &paqueteRecibido,sizeof(uint32_t),MSG_WAITALL);
	//Aca habria que deserializar y actualizar el proceso
	t_pcb* procesoDeserializado;
	agregarAColaReady(procesoDeserializado);
}

 void largoPlazo(){
	 t_pcb* proceso;
	 //Con un semaforo tendriamos que ver lo de la multiprogramacion, esto lo dejo hasta que los implementemos
	 //Lo de la multiprogramacion tendría que ser para crear los pcb realmente, no?
	 int procesosActivos=queue_size(colaReady);
	 int maxGradoMultiprogram=Multiprogramacion();
	 if(procesosActivos<=maxGradoMultiprogram){
		 proceso=extraerDeCola(colaNew);
		 agregarAColaReady(proceso);
	 }
 }

 void cortoPlazo(){
	 char* algoritmo = Algoritmo();
	 if(strcmp(algoritmo,"FIFO")){
		 algoritmoFIFO();
	 }
 }

 void algoritmoFIFO(){
	 t_pcb* procesoAEjec=extraerDeCola(colaReady);
	 procesoAEjecutar(procesoAEjec);

 }
