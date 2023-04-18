#include <stdlib.h>
#include <stdio.h>
#include "memoria.h"

int main(void) {

	printf("Hola soy Memoria y soy servidor de Kernel,Cpu y Fs \n ");

	loggerMemoria = log_create("memoria.log", "Memoria",1,LOG_LEVEL_DEBUG);

	log_info(loggerMemoria, "---------------------------------------------------------------------------");
	log_info(loggerMemoria, "Iniciando Memoria...");

	int servidorMemoria = 0;

	configMemoria = config_create("../MemoriaT/memoria.config");
	if(verificarConfig (servidorMemoria, loggerMemoria, configMemoria) == 1 ) return EXIT_FAILURE;

	char* puerto = puertoEscucha();
	char* ip = ipEscucha();

	printf ("El valor recuperado de la ip es %s con el puerto %s\n", ip, puerto);

	log_info(loggerMemoria, "Iniciando Servidor ... \n");
	servidorMemoria = iniciarServidor(ip, puerto);
	if( verificarSocket (servidorMemoria, loggerMemoria, configMemoria) == 1 ) return EXIT_FAILURE;
	log_info(loggerMemoria, "Servidor listo para recibir al cliente");

	log_info(loggerMemoria, "Iniciando Cliente ... \n");
	int cliente = esperar_cliente(servidorMemoria);
	if( verificarSocket (cliente, loggerMemoria, configMemoria) == 1 ){
		close(servidorMemoria);
		return EXIT_FAILURE;
	}
	log_info(loggerMemoria, "Se conecto un cliente");

	//t_list* lista;Dejo comentado esto porq me molesta el warning cuando se construye el proyec

	while (1) {
		int cod_op = recibir_operacion(cliente);

		switch (cod_op) {
			case MENSAJE:
				log_info(loggerMemoria, "\nMe llego el mensaje: %s", recibir_mensaje(cliente));
				break;
			/*
			case PAQUETE:
				lista = recibir_paquete(cliente);
				log_info(loggerMemoria, "Me llegaron los siguientes valores:\n");
				list_iterate(lista, (void*) iterator);
				break;
			*/
			case -1:
				log_info(loggerMemoria, "el cliente se desconecto.");
				break;

			default:
				log_warning(loggerMemoria,"Operacion desconocida. No quieras meter la pata");
				break;
		}

		if ( cod_op == -1 ) break;

	}


	log_info(loggerMemoria, "Finalizando Memoria...\n");

	terminarModulo(cliente, loggerMemoria, configMemoria);
	close (servidorMemoria);

	printf ("Finalizo Memoria  correctamente\n ");

	return EXIT_SUCCESS;
}
