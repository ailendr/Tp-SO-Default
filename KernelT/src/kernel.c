#include <stdio.h>
#include <stdlib.h>
#include "kernel.h"

int main(void) {

	printf ("Hola soy kernel y estoy queriendo recibir mensajes\n ");

	loggerKernel = log_create("kernel.log", "Kernel", 1, LOG_LEVEL_DEBUG);

	log_info(loggerKernel, "---------------------------------------------------------------------------");
	//Esto es capricho perdooon, asi queda visualmente mas facil de identificar las ejecuciones
	log_info(loggerKernel, "Iniciando Kernel...");

	configKernel = config_create("../Kernel/kernel.config");

	if (configKernel == NULL){
		log_error(loggerKernel,"Error al recuperar el config");
		log_destroy(loggerKernel);
		config_destroy(configKernel);
		return EXIT_FAILURE;
	}

	char* ip = Ip();
	char* puerto = Puerto();

	printf ("\nEl valor recuperado de la ip es %s con el puerto %s\n", ip, puerto);


	int server_fd = iniciarServidor(ip, puerto);

	if(server_fd == -1){
		log_error(loggerKernel,"Error al iniciar el servidor");
		terminarModulo(server_fd,loggerKernel, configKernel);
		return EXIT_FAILURE;
		//Aca dejo tambien como podria ser el de consola
		//El return lo obliga a terminar
	}

	log_info(loggerKernel, "Servidor listo para recibir al cliente");

	int cliente_fd = esperar_cliente(server_fd);

	if(cliente_fd == -1){
		log_error(loggerKernel,"Error al conectar con el cliente");
		terminarModulo(server_fd,loggerKernel, configKernel);
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
					log_info(loggerKernel, "el cliente se desconecto.");

					return EXIT_FAILURE;

				default:
					log_warning(loggerKernel,"Operacion desconocida. No quieras meter la pata");
					break;
			}
		}

	log_info(loggerKernel, "Finalizando Kernel...\n");

	terminarModulo(cliente_fd,loggerKernel, configKernel);

	printf ("Finalizo Kernel correctamente\n ");

	return EXIT_SUCCESS;
}

void iterator(char* value) {
	log_info(loggerKernel,"%s", value);
}
