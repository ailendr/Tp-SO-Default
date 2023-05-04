/*
 * KernelConexiones.h
 *
 *  Created on: Apr 23, 2023
 *      Author: utnso
 */

#ifndef SRC_KERNELCONEXIONES_H_
#define SRC_KERNELCONEXIONES_H_

#include <utils/sockets.h>
#include <pthread.h>

#include "kernelConfig.h"
#include "kernelGlobales.h"
#include "planificacion.h"


void iniciarConexionesDeKernel();
void atenderConsolas(int server_fd);


#endif /* SRC_KERNELCONEXIONES_H_ */
