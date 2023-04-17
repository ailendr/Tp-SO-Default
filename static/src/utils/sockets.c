/*
 * sockets.c
 *
 *  Created on: Apr 16, 2023
 *      Author: utnso
 */


#include "sockets.h"



///------FUNCIONES DEL CLIENTE------///

int iniciarCliente(char *ip, char* puerto)
{
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	// Ahora vamos a crear el socket.
	int socket_cliente = socket(server_info->ai_family,
			                     server_info->ai_socktype,
								 server_info->ai_protocol);


	// Ahora que tenemos el socket, vamos a conectarlo
    int conexion = connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen);

    freeaddrinfo(server_info);

	return ((conexion == -1) ? -1 : socket_cliente);
}
void enviar_mensaje(char* mensaje, int socket_cliente) //1)poner el mensaje en un paquee
{
	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = MENSAJE;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = strlen(mensaje) + 1;
	paquete->buffer->stream = malloc(paquete->buffer->size);
	memcpy(paquete->buffer->stream, mensaje, paquete->buffer->size);

	int bytes = paquete->buffer->size + 2*sizeof(int);

	void* a_enviar = serializar_paquete(paquete, bytes);//2)serializar el paquete

	send(socket_cliente, a_enviar, bytes, 0);//3) se envia lo serializado

	free(a_enviar); //libera puntero
	eliminar_paquete(paquete);//libera los punteros q componen la estructura paquete y luego la estructura en si
}

void* serializar_paquete(t_paquete* paquete, int bytes) // pone lo del paquete posta en ptra estructura magic para copiarlo basicamente
{
	void * magic = malloc(bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));//se pasa los bytes del tipodedato cod_op
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));//se pasa los bytes del tipo de dato q es size en bufffer, q es int
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;

	return magic;
}

void eliminar_paquete(t_paquete* paquete)
{
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}


////------FUNCIONES DEL SERVIDOR-----////

int iniciarServidor(char*  ip, char* puerto)
{
	// Quitar esta línea cuando hayamos terminado de implementar la funcion
	//assert(!"no implementado!");

	int socket_servidor;

	struct addrinfo hints, *servinfo;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &servinfo);//obtiene informacion de la ip y puerto y lo inyecta en &serverinfo

	// Creamos el socket de escucha del servidor
	socket_servidor = socket(servinfo->ai_family,
			                 servinfo->ai_socktype,
							 servinfo->ai_protocol);

	// Asociamos el socket a un puerto
	/* USAMOS BIND QUE ES EL PEGAMENTO A UN PUERTO*/
	bind(socket_servidor, servinfo-> ai_addr, servinfo->ai_addrlen);

	// Escuchamos las conexiones entrantes
	listen(socket_servidor, SOMAXCONN);

	freeaddrinfo(servinfo);

	return socket_servidor;
}



int esperar_cliente(int socket_servidor)
{

	// Aceptamos un nuevo cliente
	/*Accept retorna un nuevo socket, q ya sabemos q es un entero porq es un FileDescriptor*/
	int socket_cliente = accept(socket_servidor, NULL, NULL);

	//log_info(logger, "Se conecto un cliente!");
	printf("\nSe conecto un cliente");

	return socket_cliente;
}

int recibir_operacion(int socket_cliente)
{
	int cod_op;
	if(recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) > 0)
		return cod_op;
	else
	{
		close(socket_cliente);
		return -1;
	}
}

char* recibir_mensaje(int socket_cliente)
{
	int size;
	char* buffer = recibir_buffer(&size, socket_cliente);
	//printf("\n Me llego el mensaje: %s", buffer);
	return buffer;
}

void* recibir_buffer(int* size, int socket_cliente)
{
	void * buffer;

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}

t_list* recibir_paquete(int socket_cliente)
{
	int size;
	int desplazamiento = 0;
	void * buffer;
	t_list* valores = list_create();
	int tamanio;

	buffer = recibir_buffer(&size, socket_cliente);
	while(desplazamiento < size)
	{
		memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
		desplazamiento+=sizeof(int);
		char* valor = malloc(tamanio);
		memcpy(valor, buffer+desplazamiento, tamanio);
		desplazamiento+=tamanio;
		list_add(valores, valor);
	}
	free(buffer);
	return valores;
}




