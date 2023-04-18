#include <stdlib.h>
#include <stdio.h>
#include "FileSystem.h"

int main(void) {

	printf("Hola soy FileSystem y soy servidor de Kernel y me conecto a Memoria \n ");

	loggerFS = log_create("FileSystem.log", "FS",1,LOG_LEVEL_DEBUG);

	log_info(loggerFS, "---------------------------------------------------------------------------");
	log_info(loggerFS, "Iniciando FileSystem...");

	int servidorFS = 0;

	configFS = config_create("../FileSystemT/filesystem.config");
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

	t_list* lista;

	while (1) {
		int cod_op = recibir_operacion(cliente);

		switch (cod_op) {
			case MENSAJE:
				log_info(loggerFS, "\nMe llego el mensaje: %s", recibir_mensaje(cliente));
				break;

			/*
			case PAQUETE:
				lista = recibir_paquete(cliente);
				log_info(loggerFS, "Me llegaron los siguientes valores:\n");
				list_iterate(lista, (void*) iterator);
				break;
			*/

			case -1:
				log_info(loggerFS, "el cliente se desconecto.");
				break;

			default:
				log_warning(loggerFS,"Operacion desconocida. No quieras meter la pata");
				break;
		}

		if ( cod_op == -1 ) break;
	}

	log_info(loggerFS, "Iniciando conexion con Memoria ... \n");

	char* ipM = IP_Memoria();
	char* puertoM = puertoMemoria();

	int socketMemoria = iniciarCliente(ipM, puertoM);
	if( verificarSocket (socketMemoria, loggerFS, configFS) == 1 ) return EXIT_FAILURE;

	log_info(loggerFS, "Conexion exitosa");
	log_info(loggerFS, "Enviando mensaje");
	enviar_mensaje("Hola Memoria soy FS", socketMemoria);

	log_info(loggerFS, "Finalizando File System...\n");

	terminarModulo(cliente, loggerFS, configFS);
	close (socketMemoria);
	close (servidorFS);

	printf ("Finalizo File System correctamente\n ");

	return EXIT_SUCCESS;
}

