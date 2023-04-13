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

	t_log* loggerConsola = log_create("./consola.log","Consola", 1, LOG_LEVEL_INFO);


	char* ip = "127.0.0.1";

	char* puerto = "8000";

	int conexion = iniciarCliente(ip, puerto);

	if(conexion == -1){
		log_warning(loggerConsola,"Error no se pudo establecer una conexion");
	}
	else {
		log_info(loggerConsola, "Conexion exitosa ");
	}

	enviar_mensaje("Hola kernel", conexion);

	//estas funciones van hasta implementar terminarModulo
    log_destroy(loggerConsola);

	close (conexion);
	//terminarModulo(conexion,loggerConsola, unconfig);

	return EXIT_SUCCESS;
}
