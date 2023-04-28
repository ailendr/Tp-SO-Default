/*
 * KernelConexiones.h
 *
 *  Created on: Apr 23, 2023
 *      Author: utnso
 */

#ifndef SRC_KERNELCONEXIONES_H_
#define SRC_KERNELCONEXIONES_H_

#include <utils/sockets.h>
#include "KernelConfig.h"
#include "KernelGlobales.h"
#include <pthread.h>


void iniciarConexionesDeKernel();
void atenderConsolas(int server_fd);


#endif /* SRC_KERNELCONEXIONES_H_ */
