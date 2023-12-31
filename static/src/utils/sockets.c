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
	    	log_info(logger, "Socket creado con exito en la ip %s y puerto %s ", ip, puerto);
	    }


	// Ahora que tenemos el socket, vamos a conectarlo
    int conexion = connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen);
      if(conexion  == -1){
    	  	log_info(logger, " Error : Fallo la conexion");
        	freeaddrinfo(server_info);
        	return -1;
        }

      else {
    	  	log_info(logger, "La conexion es exitosa");
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

int enviarMensaje(char* mensaje, int socket_cliente) //1)poner el mensaje en un paquee
{
	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = MENSAJE;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = strlen(mensaje) + 1;
	paquete->buffer->stream = malloc(paquete->buffer->size);
	memcpy(paquete->buffer->stream, mensaje, paquete->buffer->size);

	int bytes = paquete->buffer->size + 2*sizeof(int);

	void* a_enviar = serializar_paquete(paquete, bytes);//2)serializar el paquete

	int returnSend = send(socket_cliente, a_enviar, bytes, 0);//3) se envia lo serializado

	free(a_enviar); //libera puntero
	eliminar_paquete(paquete);//libera los punteros q componen la estructura paquete y luego la estructura en si
  return returnSend;
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

int enviarPaquete(t_paquete* paquete, int socket_cliente, t_log* logger,char* nombrePaq)
{
	int bytes = paquete->buffer->size + 2*sizeof(int);
	void* a_enviar = serializar_paquete(paquete, bytes);

	int returnSend = send(socket_cliente, a_enviar, bytes, 0);
	free(a_enviar);
	eliminar_paquete(paquete); //En caso de error o q salga bien que tambien libere el paquete

	if(returnSend == -1){
		log_info(logger, "Error al enviar el Paquete de %s:", nombrePaq);
		return -1;
	}else{
		log_info(logger, "He podido enviar el Paquete de %s", nombrePaq );
	}

	return 0;
}

void validarEnvioDePaquete(t_paquete* paquete, int socket_cliente, t_log* logger,t_config* config, char* nombrePaq){
	if(enviarPaquete(paquete, socket_cliente, logger, nombrePaq) == -1){
		log_info(logger, "Fallo la conexion. Terminando Modulo");
		terminarModulo(socket_cliente, logger, config);
		exit(1);
	}

}



void eliminar_paquete(t_paquete* paquete)
{
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

///Mensaje de protocolo//
int enviarProtocolo(int conexion,t_handshake handshake,t_log* logger){
	uint32_t protocolo = handshake;
	uint32_t resultado = 0;//Lo inicialice asi para verificar que funcione el recv en los hilos
	int returnSend = send(conexion, &protocolo, sizeof(uint32_t), 0);

	if(returnSend == -1){
		log_info(logger, "Error al enviar el mensaje de protocolo ");
		return -1;
	}
	else{
		log_info(logger, "He podido enviar un mensaje de protocolo");
	}

	recv(conexion, &resultado, sizeof(uint32_t), MSG_WAITALL);

	if(resultado == -1){
		log_info(logger,"Fallo el Protocolo de Comunicacion.Terminando la conexion");
		return -1;
	}
	else if(resultado == HANDSHAKE_Ok){
		log_info(logger, "Protocolo de Comunicacion exitoso");
	}

	return 0;

}
//Para enviar solo el buffer de la tabla de segmentos: Cuando llegue del lado del cliente hacer un voi* buffer = recibirBuffer -> lo q me importa esta en el stream/
  int enviarBuffer(t_buffer* buffer, int socket, char* nombreBuffer, t_log* logger){
	 /*Hacer 2 sends o todo lo otro ajajaja
	  * int returnSize, returnStream;
		if( (returnSize = send(socket, &(buffer->size),sizeof(int),0)) == -1){
		log_info(logger, "Error al enviar el size del buffer");}
		if((returnStream = send(socket, buffer->stream, buffer->size,0)) == -1){
		log_info(logger, "Error al enviar el contenido de :%s", nombreBuffer);
		free(buffer->stream);
		free(buffer);

		}*/

		int bytes = buffer->size + sizeof(int);
		void * magic = malloc(bytes);
		int desplazamiento = 0;
		memcpy(magic + desplazamiento, &(buffer->size), sizeof(int));
		desplazamiento+= sizeof(int);
		memcpy(magic + desplazamiento, buffer->stream, buffer->size);
		desplazamiento+= buffer->size;
		int returnSend = send(socket, magic, bytes, 0);
		free(magic);
		free(buffer->stream);
		free(buffer);

		if(returnSend == -1){
			log_info(logger, "Error al enviar : %s", nombreBuffer);
			return -1;
		}else{
			log_info(logger, "He podido enviar : %s", nombreBuffer);
		}

		return 0;
}

  void validarEnvioBuffer(t_buffer* buffer, int socket, char* nombreBuffer, t_log* logger, t_config* config){
	  if(enviarBuffer(buffer, socket, nombreBuffer, logger) == -1){
	  		log_info(logger, "Fallo la conexion. Terminando Modulo");
	  		terminarModulo(socket, logger, config);
	  		exit(1);
	  	}
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

	//Agrego configuracion para no esperar al matar y levantar los servidores//

	if (setsockopt(socket_servidor, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0)
	    perror("setsockopt(SO_REUSEADDR) failed");
	/*int reuse = 1;
	    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) < 0)
	        perror("setsockopt(SO_REUSEADDR) failed");*/
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
	int recvNum = recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL);
	if(recvNum > 0)
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

//Usando solo para kernel///
void recibirProtocolo (int* socket_cliente){
	int conexionNueva = *socket_cliente;
	//printf("Hilo en curso: Esperando mensaje del socket con file descriptor %d", conexionNueva);

	uint32_t handshake;
	uint32_t resultado_ok = HANDSHAKE_Ok;
	uint32_t resultado_error = - 1;

	recv(conexionNueva, &handshake, sizeof(uint32_t), MSG_WAITALL);
     if(handshake == HANDSHAKE_Consola)
	   send(conexionNueva, &resultado_ok, sizeof(uint32_t), 0);
	 else
	   send(conexionNueva, &resultado_error, sizeof(uint32_t), 0);

	//close(conexionNueva);
	//free(socket_cliente);
}

void recibirHandshake(int socket_cliente,t_handshake handshake,t_log* logger){
	log_info(logger, "Esperando mensaje del cliente ");

		uint32_t protocolo;
		uint32_t resultado_ok = HANDSHAKE_Ok;
		uint32_t resultado_error = - 1;

        int returnRecv = recv(socket_cliente, &protocolo, sizeof(uint32_t), MSG_WAITALL);
    	 if(returnRecv == -1){
    		 log_info(logger, "Error al recibir el mensaje \n");
    	 	 }
    	 else if(returnRecv==0){
    		 log_info(logger, "El cliente se ha desconectado \n");
    	 	 }


		if(protocolo == handshake){
			   send(socket_cliente, &resultado_ok, sizeof(uint32_t), 0);
				log_info(logger, "El cliente cumple con el Protocolo de Comunicacion");
			}
		else{
			   send(socket_cliente, &resultado_error, sizeof(uint32_t), 0);
			}
}

//---------Terminar Modulo------///
void terminarModulo(int conexion, t_log* logger, t_config* config)
{

	if(logger != NULL){
		log_destroy(logger);
	}

	if(config != NULL){
		config_destroy(config);
	}

	close (conexion);
}
