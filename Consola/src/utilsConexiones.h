/*
 * utilsConexiones.h
 *
 *  Created on: Apr 10, 2023
 *      Author: utnso
 */

#ifndef UTILSCONEXIONES_H_
#define UTILSCONEXIONES_H_

#include <stdio.h>
#include <stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<commons/log.h>

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

int crear_conexion(char *ip, char* puerto);
void enviar_mensaje(char* mensaje, int socket_cliente);
void* serializar_paquete(t_paquete* paquete, int bytes);
void eliminar_paquete(t_paquete* paquete);
void liberar_conexion(int socket_cliente);

#endif /* UTILSCONEXIONES_H_ */
