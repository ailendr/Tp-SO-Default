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
//#include "kernelConfig.h" //lo incluimos dentro de semaforos.h
//#include "KernelConexiones.h"
#include "pcb.h"
#include "semaforos.h"
#include "recursos.h"
#include <utils/serializacion.h>

typedef struct{
	int tiempoDeBloqueo;
	t_pcb* procesoABloquear;

} t_parametroIO;

extern t_queue* colaNew;
extern t_list* colaReady;


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
//Nuevos//
void ordenarReady();
void enviarContextoACpu();

void algoritmoFIFO();
void procesoAEjecutar(t_contextoEjec* procesoAEjecutar);
void instruccionAEjecutar();
//PARA HRRN
void algoritmoHRRN();
void calcularNuevaEstimacion(t_pcb* proceso);
void calcularRR(t_pcb* proceso);
bool comparadorRR(t_pcb* proceso1, t_pcb* proceso2);
void tiempoEnCPU(t_pcb* proceso);

void generarProceso(int* socket_cliente);
void finalizarProceso(t_pcb* procesoAFinalizar, char* motivoDeFin);
t_list*  obtenerInstrucciones(int socket_cliente);
void asignarMemoria(t_pcb* procesoNuevo, t_list* tablaDeSegmento);

void implementacionWyS (char* nombreRecurso, int nombreInstruccion, t_contextoEjec* contextoActualizado);
void bloquearHilo(t_parametroIO* parametro);
void validarRyW(char* direccion);
void implementacionF(t_instruccion* instruccion);

void validarCS(int socket);

void recibirYAsignarTablaDeSegmentos(t_pcb* proceso);

void loggearListaDeIntrucciones(t_list* instrucciones);




#endif /* SRC_PLANIFICACION_H_ */
