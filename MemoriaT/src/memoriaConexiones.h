/*
 * memoriaConexiones.h
 *
 *  Created on: Apr 24, 2023
 *      Author: utnso
 */

#ifndef SRC_MEMORIACONEXIONES_H_
#define SRC_MEMORIACONEXIONES_H_

void atenderModulos(int socket_servidor);
void atenderPeticionesCpu(int socket);
void atenderPeticionesFs(int socket);
void atenderPeticionesKernel(int socket);

#endif /* SRC_MEMORIACONEXIONES_H_ */
