/*
 * memoriaConexiones.h
 *
 *  Created on: Apr 24, 2023
 *      Author: utnso
 */

#ifndef SRC_MEMORIACONEXIONES_H_
#define SRC_MEMORIACONEXIONES_H_

#include "algoritmosDeAsignacion.h"


void atenderModulos(int socket_servidor);
void atenderPeticionesCpu(int* socket);
void atenderPeticionesFs(int* socket);
void atenderPeticionesKernel(int* socketKernel);
void enviarTablaDeSegmentos(t_tabla* tablaDeSegmentos, int socket);
void enviarListaDeTablas(t_list* listaDeTablas, int socket);

#endif /* SRC_MEMORIACONEXIONES_H_ */
