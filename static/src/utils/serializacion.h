/*
 * serializacion.h
 *
 *  Created on: May 28, 2023
 *      Author: utnso
 */

#ifndef SRC_UTILS_SERIALIZACION_H_
#define SRC_UTILS_SERIALIZACION_H_

#include <string.h>
#include "sockets.h"

t_list* deserializarInstrucciones(void *buffer, int desplazamiento, int tamanioBuffer);
t_contextoEjec* deserializarContexto(void *buffer, int tamanio);
t_paquete* serializarContexto(t_contextoEjec *procesoAEjecutar);
t_paquete* serializarInstruccion(t_instruccion* instruccion);
t_paquete* serializacionParametros (t_paquete* pInstruc, int offset, t_instruccion* instruc, int param);
t_instruccion* deserializarInstruccionEstructura (void* buffer);


#endif /* SRC_UTILS_SERIALIZACION_H_ */
