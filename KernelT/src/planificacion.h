/*
 * planificion.h
 *
 *  Created on: Apr 28, 2023
 *      Author: utnso
 */

#ifndef SRC_PLANIFICACION_H_
#define SRC_PLANIFICACION_H_
#include <commons/collections/queue.h>
#include <utils/estructuras.h>
#include "KernelConexiones.h"
#include <string.h>

extern t_queue* colaNew;
extern t_queue* colaReady;

void crearColas();
void agregarAColaNew(t_pcb* procesoNuevo);
void agregarAColaReady(t_pcb* procesoListo);
t_pcb* extraerDeCola(t_queue* cola);
void largoPlazo();
void cortoPlazo();
void algoritmoFIFO();
void procesoAEjecutar(t_pcb* procesoAEjecutar);


#endif /* SRC_PLANIFICACION_H_ */
