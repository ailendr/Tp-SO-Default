/*
 * planificacion.c
 *
 *  Created on: Apr 28, 2023
 *      Author: utnso
 */

#include "planificacion.h"
t_queue* colaNew ;
t_queue* colaReady ;

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
