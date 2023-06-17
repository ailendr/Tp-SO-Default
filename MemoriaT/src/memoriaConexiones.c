/*
 * memoriaConexiones.c
 *
 *  Created on: Apr 24, 2023
 *      Author: utnso
 */


#include <utils/sockets.h>
#include <pthread.h>
#include "memoriaConfig.h"


void atenderModulos(int socket_servidor){
	while (1) {
		  pthread_t hiloServidor;

		   int *socket_cliente = malloc(sizeof(int));
		   *socket_cliente = esperar_cliente(socket_servidor, loggerMemoria);
		   if(verificarSocket(*socket_cliente, loggerMemoria, configMemoria) == 1) {
			   free(socket_cliente);
			   close(socket_servidor);
			   exit(1);
		   }

		   pthread_create(&hiloServidor,
		                    NULL,
		                   (void*) recibirProtocolo,
		                   (void*)socket_cliente);

		   pthread_detach(hiloServidor);

   }
}

