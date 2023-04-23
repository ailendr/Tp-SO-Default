/*
 * KernelConexiones.h
 *
 *  Created on: Apr 23, 2023
 *      Author: utnso
 */

#ifndef SRC_KERNELCONEXIONES_H_
#define SRC_KERNELCONEXIONES_H_



void iniciarConexionesDeKernel();
void recibirProtocolo(int* socket_cliente);
void atenderConsolas(int server_fd);


#endif /* SRC_KERNELCONEXIONES_H_ */
