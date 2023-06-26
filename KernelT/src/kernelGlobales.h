/*
 * globales.h
 *
 *  Created on: Apr 23, 2023
 *      Author: utnso
 */

#ifndef SRC_KERNELGLOBALES_H_
#define SRC_KERNELGLOBALES_H_

#include <stdint.h>//BORRAR SI NO SIRVE
#include"pcb.h"
#include <commons/collections/queue.h>


extern int socketCPU;
extern int socketFs;
extern int socketMemoria;

extern t_queue* colaNew;
extern t_list* colaReady;
extern t_list* listaDeProcesos;
extern uint32_t pid;

extern t_pcb* ultimoEjecutado;
extern t_list* listaDeBloqueo;
extern t_list* listaDeInstancias;


#endif /* SRC_KERNELGLOBALES_H_ */
