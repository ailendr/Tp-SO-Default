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

extern int socketCPU;
extern int socketFs;
extern int socketMemoria;
extern uint32_t pid;
extern int procesosActivos;
extern t_pcb* ultimoEjecutado;
extern t_list* listaDeBloqueo;
extern t_list* listaDeInstancias;


#endif /* SRC_KERNELGLOBALES_H_ */
