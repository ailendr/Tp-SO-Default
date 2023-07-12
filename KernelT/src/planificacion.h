/*
 * planificion.h
 *
 *  Created on: Apr 28, 2023
 *      Author: utnso
 */

#ifndef SRC_PLANIFICACION_H_
#define SRC_PLANIFICACION_H_
#include "utilsPlanificacion.h"






//Nuevos//
void ordenarReady();

void algoritmoFIFO();
void instruccionAEjecutar();
//PARA HRRN
void algoritmoHRRN();
void calcularNuevaEstimacion(t_pcb* proceso);
void calcularRR(t_pcb* proceso);
bool comparadorRR(t_pcb* proceso1, t_pcb* proceso2);

void generarProceso(int* socket_cliente);
t_list*  obtenerInstrucciones(int socket_cliente);
void asignarMemoria(t_pcb* procesoNuevo, t_list* tablaDeSegmento);


void recibirYAsignarTablaDeSegmentos(t_pcb* proceso);


void loggearListaDeIntrucciones(t_list* instrucciones);




#endif /* SRC_PLANIFICACION_H_ */
