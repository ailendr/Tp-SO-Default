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

	log_info(loggerMemoria, "Esperando Clientes ... \n");

	atenderModulos(servidorMemoria);


	log_info(loggerMemoria, "Finalizando Memoria...\n");

	//terminarModulo(cliente, loggerMemoria, configMemoria); No lo vamos a usar porq la funcion del hilo ya cierra las conexion del cliente ahi
	log_destroy(loggerMemoria);
	config_destroy(configMemoria);
	close (servidorMemoria);

	printf ("Finalizo Memoria  correctamente\n ");

	return EXIT_SUCCESS;
}
