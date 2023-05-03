/*
 * planificion.h
 *
 *  Created on: Apr 28, 2023
 *      Author: utnso
 */

#ifndef SRC_PLANIFICACION_H_
#define SRC_PLANIFICACION_H_
#include <commons/collections/queue.h>
#include "KernelConexiones.h"
#include "pcb.h"

extern t_queue* colaNew;
extern t_list* colaReady;


void crearEstados();
void agregarAEstadoNew(t_pcb* procesoNuevo);
void agregarAEstadoReady(t_pcb* procesoListo);
t_pcb* extraerDeNew();
t_pcb* extraerDeReady();

void largoPlazo();
void cortoPlazo();
void algoritmoFIFO();
void procesoAEjecutar(t_contextoEjec* procesoAEjecutar);

void generarProceso(int* socket_cliente);
t_list*  obtenerInstrucciones(int socket_cliente);
void asignarMemoria(t_pcb* procesoNuevo, t_list* tablaDeSegmento);


#endif /* SRC_PLANIFICACION_H_ */
