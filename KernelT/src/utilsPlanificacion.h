/*
 * recursos.h
 *
 *  Created on: May 25, 2023
 *      Author: utnso
 */

#ifndef SRC_UTILSPLANIFICACION_H_
#define SRC_UTILSPLANIFICACION_H_
#include "semaforos.h"

typedef struct{
	int tiempoDeBloqueo;
	t_pcb* procesoABloquear;

} t_parametroIO;

typedef struct{
	char* nombreArchivo;
	int contador;
}t_archivo;

typedef struct{
	char* nombreArchivo;
	int puntero;
}t_archivoPorProceso;

/*typedef struct{
	uint32_t PID;
	t_list* archivos; //Aca almacenamos un archivo por proceso
}t_tablaDeAxP; //Tabla de archivos por proceso
*/
typedef struct{
	char* nombreArchivo;
	t_queue* colaBlock;
} t_colaDeArchivo;

typedef struct{
	t_instruccion* instruccion;
	t_pcb* proceso;
}t_parametroFS;

void crearEstados();
void eliminarEstados();
void agregarAEstadoNew(t_pcb* procesoNuevo);
void agregarAEstadoReady(t_pcb* procesoListo);
t_pcb* extraerDeNew();
t_pcb* extraerDeReady();
void logCambioDeEstado(t_pcb* proceso, char* estadoAnterior, char* nuevoEstado);
void mostrarColaReady();

//lo pongo aca porq largo plazo los usa//
void largoPlazo();
void cortoPlazo();

void procesoAEjecutar(t_contextoEjec* procesoAEjecutar);
void tiempoEnCPU(t_pcb* proceso);
void calcularNuevaEstimacion(t_pcb* proceso);

void finalizarProceso(t_pcb* procesoAFinalizar, char* motivoDeFin);


void bloquearHilo(t_parametroIO* parametro);
void validarRyW(char* direccion,t_pcb* ultimoEjecutado);
void actualizarTablaEnProcesos(t_list* listaDeTablas);

t_list* crearListaDeBloqueo();
t_list* crearListaDeInstancias();
void crearEstructurasDeRecursos();
void crearEstructurasDeArchivos();
int recursoDisponible(char* nombre);

//Logueo de las instrucciones para verificar que esta todo ok//
void mostrarListaDeProcesos();
void loggearListaDeIntrucciones(t_list* instrucciones);
int posProcesoAEliminar(t_list* listaDeProcesos, int pid);

float tiempoActualEnMiliseg();
 //Funciones para Archivos//
void crearColaBlockDeArchivo(char* archivo);
void bloquearProcesoPorArchivo (char* archivo, t_pcb* proceso);
t_colaDeArchivo* buscarColaDeArchivo(char* archivo);
int posColaDeArchivo(char* archivo);
int buscarArchivoEnTGAA(char* nombreArchivo);
void agregarEntradaATablaxProceso(char* nombreArchivo, t_pcb* proceso, int posPuntero);
//int buscarTAxP(uint32_t pid);




#endif /* SRC_UTILSPLANIFICACION_H_ */
