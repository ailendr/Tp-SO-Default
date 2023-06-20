/*
 * memoriaConexiones.h
 *
 *  Created on: Apr 24, 2023
 *      Author: utnso
 */

#ifndef SRC_MEMORIACONEXIONES_H_
#define SRC_MEMORIACONEXIONES_H_

#include <utils/sockets.h>
#include <pthread.h>
#include "memoriaConfig.h"
#include "algoritmosDeAsignacion.h"

void atenderModulos(int socket_servidor);
void atenderPeticionesCpu(int socket);
void atenderPeticionesFs(int socket);
void atenderPeticionesKernel(int socket);

#endif /* SRC_MEMORIACONEXIONES_H_ */
