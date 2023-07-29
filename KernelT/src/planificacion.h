/*
 * planificion.h
 *
 *  Created on: Apr 28, 2023
 *      Author: utnso
 */

#ifndef SRC_PLANIFICACION_H_
#define SRC_PLANIFICACION_H_
#include "utilsPlanificacion.h"

typedef struct{
	uint32_t PID;
	char* nombreArchivo;
}t_archivos;

extern t_list* archivos;
extern int operacionFS;

extern struct timespec begin, end;

//Nuevos//
void ordenarReady();
void instruccionAEjecutar(t_pcb* ultimoEjecutado);

//PARA HRRN
void finTiempoEnCPU(t_pcb* proceso);
void calcularRR(t_pcb* proceso);
bool comparadorRR(t_pcb* proceso1, t_pcb* proceso2);


void generarProceso(int* socket_cliente);
t_list*  obtenerInstrucciones(int socket_cliente);
void asignarMemoria(t_pcb* procesoNuevo, t_tabla* tablaDeSegmento);


void recibirYAsignarTablaDeSegmentos(t_pcb* proceso);


void implementacionWyS (char* nombreRecurso, int nombreInstruccion,  t_pcb* proceso);
void validarCS(int socketMemoria, t_instruccion* instruccion, t_pcb* ultimoEjecutado);
void implementacionF(t_instruccion* instruccion, t_pcb* ultimoEjecutado);


int archivoAbierto (char* nombre, int id);

#endif /* SRC_PLANIFICACION_H_ */
