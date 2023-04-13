/*
 * sockets.h
 *
 *  Created on: Apr 11, 2023
 *      Author: utnso
 */

#ifndef SRC_SOCKETS_H_
#define SRC_SOCKETS_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <signal.h>
#include <netdb.h>
#include <commons/collections/list.h>


typedef enum
{
	MENSAJE,
	PAQUETE
}op_code;

typedef struct
{
	int size;
	void* stream;
} t_buffer;

typedef struct
{
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;

int iniciarServidor(void);
int iniciarCliente(char *ip, char* puerto);

int esperar_cliente(int socket_servidor);
void recibir_mensaje(int socket_cliente);
void* recibir_buffer(int* size, int socket_cliente);
int recibir_operacion(int socket_cliente);
t_list* recibir_paquete(int socket_cliente);

void enviar_mensaje(char* mensaje, int socket_cliente);
void* serializar_paquete(t_paquete* paquete, int bytes);
void eliminar_paquete(t_paquete* paquete);

void liberar_conexion(int socket);

#endif /* SRC_SOCKETS_H_ */
