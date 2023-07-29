/*
 * globales.h
 *
 *  Created on: Apr 23, 2023
 *      Author: utnso
 */

#ifndef SRC_KERNELGLOBALES_H_
#define SRC_KERNELGLOBALES_H_

#include"pcb.h"
#include <commons/collections/queue.h>


extern int socketCPU;
extern int socketFs;
extern int socketMemoria;

extern t_queue* colaNew;
extern t_list* colaReady;
extern t_list* listaDeProcesos;
extern uint32_t pid;

extern t_list* listaDeBloqueo;
extern t_list* listaDeInstancias;
extern t_list* listaDeColasPorArchivo;

extern t_list* archivos;
extern int operacionFS;

#endif /* SRC_KERNELGLOBALES_H_ */

