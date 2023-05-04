/*
 * pcb.h
 *
 *  Created on: May 2, 2023
 *      Author: utnso
 */

#ifndef SRC_PCB_H_
#define SRC_PCB_H_
#include <utils/estructuras.h>
#include "KernelConexiones.h"

t_pcb* crearPcb(t_list* instrucciones);

t_contextoEjec* crearContexto(t_list* instrucciones);


#endif /* SRC_PCB_H_ */
