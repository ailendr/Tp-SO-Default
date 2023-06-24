/*
 * pcb.h
 *
 *  Created on: May 2, 2023
 *      Author: utnso
 */

#ifndef SRC_PCB_H_
#define SRC_PCB_H_
#include <utils/estructuras.h>
#include "kernelConexiones.h"

t_pcb* crearPcb (t_list* instrucciones, uint32_t pidNuevo, int conexionConsola);

t_contextoEjec* crearContexto(t_list* instrucciones, uint32_t pidDelContexto);



#endif /* SRC_PCB_H_ */
