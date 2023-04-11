#ifndef UTILSSERVIDOR_H_
#define UTILSSERVIDOR_H_

#include <stdio.h>
#include <stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<commons/log.h>
#include<commons/collections/list.h>

#define PUERTO 8000

typedef enum
{
	MENSAJE,
	PAQUETE
}op_code;

int iniciar_servidor(void);
int esperar_cliente(int socket_servidor);
int recibir_operacion(int socket_cliente);

void recibir_mensaje(int socket_cliente);
void* recibir_buffer(int* size, int socket_cliente);




#endif /* CONSOLA_H_ */
