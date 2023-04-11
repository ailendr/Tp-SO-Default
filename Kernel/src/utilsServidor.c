#include "utilsServidor.h"

int iniciar_servidor(void)
{
	// Quitar esta línea cuando hayamos terminado de implementar la funcion
	//assert(!"no implementado!");

	int socket_servidor;
	char*  ip= "127.0.0.1";
	char* puerto="8000";


	struct addrinfo hints, *servinfo, *p;

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
	//log_trace(logger, "Listo para escuchar a mi cliente");
	printf("Listo para escuchar un cliente");

	return socket_servidor;
}


int esperar_cliente(int socket_servidor)
{
	// Quitar esta línea cuando hayamos terminado de implementar la funcion
	//assert(!"no implementado!");

	// Aceptamos un nuevo cliente
	/*Accept retorna un nuevo socket, q ya sabemos q es un entero porq es un FileDescriptor*/
	int socket_cliente = accept(socket_servidor, NULL, NULL);

	//log_info(logger, "Se conecto un cliente!");
	printf("se conecto un cliente");

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

void recibir_mensaje(int socket_cliente)
{
	int size;
	char* buffer = recibir_buffer(&size, socket_cliente);
	//log_info(logger, "Me llego el mensaje : %s", buffer);
	printf("Me llego el mensaje: %s", buffer);
	free(buffer);
}
void* recibir_buffer(int* size, int socket_cliente)
{
	void * buffer;

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}
