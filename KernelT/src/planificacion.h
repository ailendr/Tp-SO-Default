/*
 * planificion.h
 *
 *  Created on: Apr 28, 2023
 *      Author: utnso
 */

#ifndef SRC_PLANIFICACION_H_
#define SRC_PLANIFICACION_H_
#include <commons/collections/queue.h>
#include <time.h>

#include "kernelGlobales.h"
//#include "kernelConfig.h" lo incluimos dentro de semaforos
//#include "KernelConexiones.h"
#include "pcb.h"
#include "semaforos.h"

extern t_queue* colaNew;
extern t_list* colaReady;


void crearEstados();
void eliminarEstados();
void agregarAEstadoNew(t_pcb* procesoNuevo);
void agregarAEstadoReady(t_pcb* procesoListo);
t_pcb* extraerDeNew();
t_pcb* extraerDeReady();

//lo pongo aca porq largo plazo los usa//
t_paquete* serializarContexto(t_contextoEjec* procesoAEjecutar);
t_list* deserializarInstrucciones(void*buffer, int desplazamiento,int tamanioBuffer);
t_contextoEjec* deserializarContexto(void* buffer, int tamanio);
void largoPlazo();
void cortoPlazo();
void algoritmoFIFO();
void procesoAEjecutar(t_contextoEjec* procesoAEjecutar);
void instruccionAEjecutar();
//PARA HRRN
void algoritmoHRRN();
void calcularNuevaRafaga();
void calcularRR(t_pcb* proceso);
bool comparadorRR(t_pcb* proceso1, t_pcb* proceso2);

void generarProceso(int* socket_cliente);
void finalizarProceso(t_pcb* procesoAFinalizar);
t_list*  obtenerInstrucciones(int socket_cliente);
void asignarMemoria(t_pcb* procesoNuevo, t_list* tablaDeSegmento);



#endif /* SRC_PLANIFICACION_H_ */
