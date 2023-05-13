/*
 * conexionCpu.h
 *
 *  Created on: May 12, 2023
 *      Author: utnso
 */

#ifndef SRC_CONEXIONCPU_H_
#define SRC_CONEXIONCPU_H_

#include "./cpuConfig.h"
#include <utils/sockets.h>
#include <utils/estructuras.h>

extern int servidorCpu;
extern int socketMemoria;
extern int cliente;

int iniciarSocketsCpu();
t_paquete* serializarContexto(t_contextoEjec* procesoAEjecutar);
t_contextoEjec* deserializarContexto(void* buffer, int tamanio);
t_paquete* serializarInstrucciones(t_instruccion* instruccion);
t_list* deserializarInstrucciones(void*buffer, int desplazamiento,int tamanioBuffer);

#endif /* SRC_CONEXIONCPU_H_ */
