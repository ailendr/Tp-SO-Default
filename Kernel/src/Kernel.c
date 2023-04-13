/*
 ============================================================================
 Name        : Kernel.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "Kernel.h"

t_log* loggerServidor;

int main(void) {
	loggerServidor = log_create("servidor.log", "Servidor", 1, LOG_LEVEL_DEBUG);
	int server_fd = iniciarServidor();
	log_info(loggerServidor, "Servidor listo para recibir al cliente");

	int cliente_fd = esperar_cliente(server_fd);
	t_list* lista;
		while (1) {
			int cod_op = recibir_operacion(cliente_fd);

			switch (cod_op) {
			case MENSAJE:
		     recibir_mensaje(cliente_fd);
             break;

			case PAQUETE:
				lista = recibir_paquete(cliente_fd);
				log_info(loggerServidor, "Me llegaron los siguientes valores:\n");
				list_iterate(lista, (void*) iterator);
				break;
			case -1:
				log_error(loggerServidor, "el cliente se desconecto. Terminando servidor");
				//printf("el cliente se desconecto. Terminando servidor");
				return EXIT_FAILURE;
			default:
				log_warning(loggerServidor,"Operacion desconocida. No quieras meter la pata");
				break;
			}
		}

		log_destroy(loggerServidor);//solo esta hasta tener el config para terminar modulo

	  //terminarModulo(cliente_fd,loggerServidor, config);

	return EXIT_SUCCESS;
}

void iterator(char* value) {
	log_info(loggerServidor,"%s", value);
}
