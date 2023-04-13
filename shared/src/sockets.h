/*
 * sockets.h
 *
 *  Created on: Apr 11, 2023
 *      Author: utnso
 */

#ifndef SRC_SOCKETS_H_
#define SRC_SOCKETS_H_

#include <stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<commons/log.h>
#include <commons/collections/list.h>

extern t_log* loggerServidor;

int recibir_operacion(int socket_cliente);
t_list* recibir_paquete(int socket_cliente);

typedef enum
{
	MENSAJE,
	PAQUETE
}op_code;



int iniciar_servidor(void);
int esperar_cliente(int socket_servidor);


void recibir_mensaje(int socket_cliente);
void* recibir_buffer(int* size, int socket_cliente);





#endif /* SRC_SOCKETS_H_ */
