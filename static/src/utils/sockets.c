/*
 * sockets.c
 *
 *  Created on: Apr 16, 2023
 *      Author: utnso
 */


#include "sockets.h"



///------FUNCIONES DEL CLIENTE------///
int iniciarCliente(char *ip, char* puerto, t_log* logger)
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
	
	 if(socket_cliente == -1) {
	        log_error(logger, "Error al crear el socket \n ");
	        return -1;
	    }
	    else{
	    	log_info(logger, "\n Socket creado con exito en la ip %s y puerto %s ", ip, puerto);
	    }


	// Ahora que tenemos el socket, vamos a conectarlo
    int conexion = connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen);
      if(conexion  == -1){
        	log_info(logger, "\n Error : fallo la conexion");
        	freeaddrinfo(server_info);
        	return -1;
        }

      else {
        	log_info(logger, "\n La conexion es exitosa");
        }

    freeaddrinfo(server_info);
    return socket_cliente;
    
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

void crear_buffer(t_paquete* paquete)
{
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = 0;
	paquete->buffer->stream = NULL;
}

t_paquete* crear_paquete(void)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = PAQUETE;
	crear_buffer(paquete);
	return paquete;
}

void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio)
{
	paquete->buffer->stream = realloc(paquete->buffer->stream, paquete->buffer->size + tamanio + sizeof(int));

	memcpy(paquete->buffer->stream + paquete->buffer->size, &tamanio, sizeof(int));
	memcpy(paquete->buffer->stream + paquete->buffer->size + sizeof(int), valor, tamanio);

	paquete->buffer->size += tamanio + sizeof(int);
}

void enviar_paquete(t_paquete* paquete, int socket_cliente)
{
	int bytes = paquete->buffer->size + 2*sizeof(int);
	void* a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
}


void eliminar_paquete(t_paquete* paquete)
{
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

///Mensaje de protocolo//
int enviarProtocolo(int conexion, t_log* logger){
	uint32_t handshake = 1;
	uint32_t resultado = 2 ;//Lo inicialice asi para verificar que funcione el recv en los hilos
	int returnSend = send(conexion, &handshake, sizeof(uint32_t), 0);

	if(returnSend == -1){
		log_info(logger, "Error al enviar el mensaje de protocolo");
		return -1;
	}
	else{
		log_info(logger, "He podido enviar un mensaje de protocolo");
	}

	recv(conexion, &resultado, sizeof(uint32_t), MSG_WAITALL);

	if(resultado == -1){
		log_info(logger,"No cumple el protocolo.Terminando la conexion");
		return -1;
	}
	else if(resultado == 0){
		log_info(logger, "El valor devuelto cumple con el protocolo");
	}

	return 0;

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



int esperar_cliente(int socket_servidor, t_log* logger)
{

	// Aceptamos un nuevo cliente
	/*Accept retorna un nuevo socket, q ya sabemos q es un entero porq es un FileDescriptor*/
	int socket_cliente = accept(socket_servidor, NULL, NULL);

    if(socket_cliente < 0){
    	log_error(logger, "No se ha podido conectar el cliente");
    }else{
    	log_info(logger, "Se ha aceptado un cliente nuevo");
    }

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
//Mensaje de protocolo: no tiene el log por parametro porq habria que pasarselo por el hilo y no puede pasarse multiples parametros al menos que sea un puntero a un struct pero paja//

void recibirProtocolo (int* socket_cliente){
	int conexionNueva = *socket_cliente;
	//printf("Hilo en curso: Esperando mensaje del socket con file descriptor %d", conexionNueva);

	uint32_t handshake;
	uint32_t resultado_ok = 0;
	uint32_t resultado_error = - 1;

	recv(conexionNueva, &handshake, sizeof(uint32_t), MSG_WAITALL);
     if(handshake == 1)
	   send(conexionNueva, &resultado_ok, sizeof(uint32_t), 0);
	 else
	   send(conexionNueva, &resultado_error, sizeof(uint32_t), 0);

	close(conexionNueva);
	free(socket_cliente);
}

void recibirHandshake(int socket_cliente){
	printf("\n Esperando mensaje del socket con file descriptor %d", socket_cliente);

		uint32_t handshake;
		uint32_t resultado_ok = 0;
		uint32_t resultado_error = - 1;

     recv(socket_cliente, &handshake, sizeof(uint32_t), MSG_WAITALL);
		if(handshake == 1)
			   send(socket_cliente, &resultado_ok, sizeof(uint32_t), 0);
			else
			   send(socket_cliente, &resultado_error, sizeof(uint32_t), 0);
}


