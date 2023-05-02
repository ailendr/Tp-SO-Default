/*
 * consola.c
 *
 *  Created on: Apr 16, 2023
 *      Author: utnso
 */

#include "consola.h"

int main(void) {

	printf ("Hola soy consola y quiero conectarme con kernel \n ");

	loggerConsola = log_create("./consola.log","Consola", 1, LOG_LEVEL_INFO);

	log_info(loggerConsola, "---------------------------------------------------------------------------");

	log_info(loggerConsola, "Iniciando Consola...");

	int conexion = 0;

	configConsola = config_create("../ConsolaT/consola.config");

	if( verificarConfig (conexion, loggerConsola, configConsola) == 1 ) return EXIT_FAILURE;

	char* ip = IpKernel();
	char* puerto = PuertoKernel();

	printf ("El valor recuperado de la ip es %s con el puerto %s\n", ip, puerto);

	log_info(loggerConsola, "Iniciando como Cliente ... \n");
	conexion = iniciarCliente(ip, puerto, loggerConsola);
	if( verificarSocket (conexion, loggerConsola, configConsola) == 1 ) return EXIT_FAILURE;

	log_info(loggerConsola, "Enviando mensaje");

	if(enviarProtocolo(conexion, loggerConsola) == -1){
	        terminarModulo(conexion,loggerConsola, configConsola);
	        return EXIT_FAILURE;
	}

	log_info(loggerConsola, "Finalizando Consola...\n");

	terminarModulo(conexion, loggerConsola, configConsola);

	printf ("Finalizo Consola correctamente\n ");

	return EXIT_SUCCESS;
}
