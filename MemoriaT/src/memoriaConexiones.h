<<<<<<< HEAD
=======
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

void atenderModulos(int socket_servidor);
void atenderPeticionesCpu(int socket);
void atenderPeticionesFs(int socket);
void atenderPeticionesKernel(int socket);

#endif /* SRC_MEMORIACONEXIONES_H_ */
>>>>>>> 711da0eda9f8edead47d77266c5e456b6cd76a6e
