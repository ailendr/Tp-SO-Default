#include <stdlib.h>
#include <stdio.h>
#include "FileSystem.h"

int main(void) {

	printf("Hola soy FileSystem y soy servidor de Kernel y me conecto a Memoria \n ");

	loggerFS = log_create("memoria.log", "Memoria",1,LOG_LEVEL_DEBUG);

	log_info(loggerFS, "---------------------------------------------------------------------------");
	log_info(loggerFS, "Iniciando FileSystem...");

	int servidorFS = 0;

	configFS = config_create("../FileSystem/filesystem.config");
	if(verificarConfig (servidorFS, loggerFS, configFS) == 1 ) return EXIT_FAILURE;

	char* puerto = puertoEscucha();
	char* ip = IP_Escucha();

	printf ("El valor recuperado de la ip es %s con el puerto %s\n", ip, puerto);

	log_info(loggerFS, "Iniciando Servidor ... \n");
	servidorFS = iniciarServidor(ip, puerto);
	if( verificarSocket (servidorFS, loggerFS, configFS) == 1 ) return EXIT_FAILURE;
	log_info(loggerFS, "Servidor listo para recibir al cliente");

	log_info(loggerFS, "Iniciando Cliente ... \n");
	int cliente = esperar_cliente(servidorFS);
	if( verificarSocket (cliente, loggerFS, configFS) == 1 ){
		close(servidorFS);
		return EXIT_FAILURE;
	}
	log_info(loggerFS, "Se conecto un cliente");

		/*
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
	*/


	log_info(loggerFS, "Finalizando Memoria...\n");

	terminarModulo(cliente, loggerFS, configFS);
	close (servidorFS);

	printf ("Finalizo Memoria  correctamente\n ");

	return EXIT_SUCCESS;
}

