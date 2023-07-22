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
void instruccionAEjecutar(t_pcb* ultimoEjecutado);
//PARA HRRN
//void algoritmoHRRN();
void calcularRR(t_pcb* proceso);
bool comparadorRR(t_pcb* proceso1, t_pcb* proceso2);
uint32_t tiempo_actual();
t_pcb* pcb_elegido_HRRN();
void estimar_rafaga(t_pcb* pcb);
t_pcb* obtenerProceso();

void generarProceso(int* socket_cliente);
t_list*  obtenerInstrucciones(int socket_cliente);
void asignarMemoria(t_pcb* procesoNuevo, t_tabla* tablaDeSegmento);


void recibirYAsignarTablaDeSegmentos(t_pcb* proceso);


void implementacionWyS (char* nombreRecurso, int nombreInstruccion,  t_pcb* proceso);
void validarCS(int socketMemoria, t_instruccion* instruccion, t_pcb* ultimoEjecutado);



#endif /* SRC_PLANIFICACION_H_ */
