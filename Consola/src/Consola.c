/*
 ============================================================================
 Name        : Consola.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "Consola.h"
//#include "../../shared/src/sockets.h" -> Esta forma es por si no toma el sockets.h

int main(void) {

	printf ("Hola soy consola y quiero conectarme con kernel \n ");

	loggerConsola = log_create("./consola.log","Consola", 1, LOG_LEVEL_INFO);

	log_info(loggerConsola, "---------------------------------------------------------------------------");
	//Esto es capricho perdooon, asi queda visualmente mas facil de identificar las ejecuciones
	log_info(loggerConsola, "Iniciando Consola...");

	int conexion = 0;

	configConsola = config_create("../Consola/consola.config");

	if (configConsola == NULL){
		log_error(loggerConsola,"Error al recuperar el config");
		terminarModulo(conexion,loggerConsola, configConsola);
		return EXIT_FAILURE;
	}

	char* ip = IpKernel();
	char* puerto = PuertoKernel();

	printf ("\nEl valor recuperado de la ip es %s con el puerto %s\n", ip, puerto);

	conexion = iniciarCliente(ip, puerto);

	if(conexion == -1){
		log_error(loggerConsola,"Error no se pudo establecer una conexion");
		terminarModulo(conexion,loggerConsola, configConsola);
		return EXIT_FAILURE;
	}

	log_info(loggerConsola, "Conexion exitosa");
	log_info(loggerConsola, "Enviando mensaje");
	enviar_mensaje("Hola kernel", conexion);

	log_info(loggerConsola, "Finalizando Consola...\n");

	terminarModulo(conexion,loggerConsola, configConsola);

	printf ("Finalizo Consola correctamente\n ");

	return EXIT_SUCCESS;
}
