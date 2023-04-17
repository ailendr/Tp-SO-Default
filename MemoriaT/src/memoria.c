#include <stdlib.h>
#include <stdio.h>
#include "memoria.h"

int main(void) {
	printf("Hola soy Memoria y soy servidor de Kernel,Cpu y Fs \n ");
	loggerMemoria = log_create("memoria.log", "Memoria",1,LOG_LEVEL_DEBUG);
	log_info(loggerMemoria, "---------------------------------------------------------------------------");
	log_info(loggerMemoria, "Iniciando Memoria...");

	configMemoria = config_create("../memoria.config"); //puse asi el path por ../MemoriaT/memoria me rompe
	if (configMemoria == NULL){
			log_error(loggerMemoria,"Error al recuperar el config");
			log_destroy(loggerMemoria);
			config_destroy(configMemoria);
			return EXIT_FAILURE;
		}

	char* puerto = puertoEscucha();
	printf ("\n El valor recuperado del puerto es %s ", puerto);


	int servidorMemoria = iniciarServidor(NULL, puerto);
	//Habria q ver de valirdar esto en sockets sino lo vamos a repetir en todos los modulos"
	if(servidorMemoria == -1){
		log_error(loggerMemoria, "Error al iniciar el servidor");

		terminarModulo(servidorMemoria, loggerMemoria, configMemoria);

		return EXIT_FAILURE;

	}
	log_info(loggerMemoria, "Servidor listo para recibir al cliente");

	int cliente = esperar_cliente(servidorMemoria);

	///Habria q ver de validar esto en sockets ///
	if(cliente ==-1){
		log_error(loggerMemoria, "Error al conectar con el cliente");
		terminarModulo(servidorMemoria, loggerMemoria, configMemoria);
        return EXIT_FAILURE;
	}

	log_info(loggerMemoria, "Se conecto un cliente");

	///Copio esto comentado de kernel porque hay q ver de hacerla una funcion generica//
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

		log_info(loggerKernel, "Finalizando Kernel...\n");
		*/

		terminarModulo(cliente,loggerMemoria, configMemoria);


		printf ("Finalizo Memoria  correctamente\n ");



	return EXIT_SUCCESS;
}
