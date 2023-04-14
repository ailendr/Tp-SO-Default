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

t_log* loggerKernel;

int main(void) {

	printf ("Hola soy kernel y estoy queriendo recibir mensajes\n ");

	loggerKernel = log_create("servidor.log", "Servidor", 1, LOG_LEVEL_DEBUG);

	log_info(loggerKernel, "---------------------------------------------------------------------------");
	//Esto es capricho perdooon, asi queda visualmente mas facil de identificar las ejecuciones
	log_info(loggerKernel, "Iniciando Kernel...");

	int server_fd = iniciarServidor();

	if(server_fd == -1){
		log_error(loggerKernel,"Error al iniciar el servidor");
		terminarModulo(server_fd,loggerKernel/*, config*/);
		return EXIT_FAILURE;
		//Aca dejo tambien como podria ser el de consola
		//El return lo obliga a terminar
	}

	log_info(loggerKernel, "Servidor listo para recibir al cliente");

	int cliente_fd = esperar_cliente(server_fd);

	if(cliente_fd == -1){
		log_error(loggerKernel,"Error al conectar con el cliente");
		terminarModulo(server_fd,loggerKernel/*, config*/);
		return EXIT_FAILURE;
	}

	log_info(loggerKernel, "Se conecto un cliente");

	t_list* lista;

		while (1) {
			int cod_op = recibir_operacion(cliente_fd);

			switch (cod_op) {
				case MENSAJE:
					log_info(loggerKernel, "\nMe llego el mensaje: %s", recibir_mensaje(cliente_fd));
					//Lo pase a un log porque a la larga necesitamos recuperarlo
					break;

				case PAQUETE:
					lista = recibir_paquete(cliente_fd);
					log_info(loggerKernel, "Me llegaron los siguientes valores:\n");
					list_iterate(lista, (void*) iterator);
					break;

				case -1:
					log_error(loggerKernel, "el cliente se desconecto. Terminando servidor");
					//printf("el cliente se desconecto. Terminando servidor");
					//Aca tengo una duda, porque que el cliente se desconecte lo tomamos como error?
					//O sea si se que las commons dice que es un error porque se conecto
					//Pero como podemos diferenciar las desconexiones por error que las desconexiones esperadas??
					//Cuando terminamos toda la comunicacion con la consola, la consola finaliza, a esa me refiero

					//Consultaria con el profe
					return EXIT_FAILURE;

				default:
					log_warning(loggerKernel,"Operacion desconocida. No quieras meter la pata");
					break;
			}
		}

	log_info(loggerKernel, "Finalizando Kernel...\n");

	terminarModulo(cliente_fd,loggerKernel/*, config*/);

	printf ("Finalizo Kernel correctamente\n ");

	return EXIT_SUCCESS;
}

void iterator(char* value) {
	log_info(loggerKernel,"%s", value);
}

/*
NOTAS:
(Dany)
-> Agregue validaciones de errores
-> Modifique recibirMensaje para que me devuelva el mensaje
*/
