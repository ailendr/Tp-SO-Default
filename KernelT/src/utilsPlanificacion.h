/*
 * recursos.h
 *
 *  Created on: May 25, 2023
 *      Author: utnso
 */

#ifndef SRC_UTILSPLANIFICACION_H_
#define SRC_UTILSPLANIFICACION_H_
#include "kernelGlobales.h"
#include "semaforos.h"

typedef struct{
	int tiempoDeBloqueo;
	t_pcb* procesoABloquear;

} t_parametroIO;

void crearEstados();
void eliminarEstados();
void agregarAEstadoNew(t_pcb* procesoNuevo);
void agregarAEstadoReady(t_pcb* procesoListo);
t_pcb* extraerDeNew();
t_pcb* extraerDeReady();
void logCambioDeEstado(t_pcb* proceso, char* estadoAnterior, char* nuevoEstado);

//lo pongo aca porq largo plazo los usa//
void largoPlazo();
void cortoPlazo();

void procesoAEjecutar(t_contextoEjec* procesoAEjecutar);
void tiempoEnCPU(t_pcb* proceso);
void finalizarProceso(t_pcb* procesoAFinalizar, char* motivoDeFin);


void bloquearHilo(t_parametroIO* parametro);
void validarRyW(char* direccion);
void implementacionF(t_instruccion* instruccion);
void actualizarTablaEnProcesos(t_list* listaDeTablas);

t_list* crearListaDeBloqueo();
t_list* crearListaDeInstancias();
void crearEstructurasDeRecursos();
int recursoDisponible(char* nombre);

//Logueo de las instrucciones para verificar que esta todo ok//
void mostrarListaDeProcesos();

int posProcesoAEliminar(t_list* listaDeProcesos, int pid);


#endif /* SRC_UTILSPLANIFICACION_H_ */