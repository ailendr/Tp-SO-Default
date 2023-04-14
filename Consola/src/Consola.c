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

	log_info(loggerConsola, "---------------------------------------------------------------------------");
	//Esto es capricho perdooon, asi queda visualmente mas facil de identificar las ejecuciones
	log_info(loggerConsola, "Iniciando Consola...");

	char* ip = "127.0.0.1";

	char* puerto = "8000";

	int conexion = iniciarCliente(ip, puerto);

	if(conexion == -1){
		log_warning(loggerConsola,"Error no se pudo establecer una conexion");
		//Esto hay que verificarlo, porque si el kernel no esta prendido o funcionando
		//igual va por el camino del exito, lo cual es rarisimo
		//lo consultaria con el profe
		//TODO

		//En kernel deje como podria tambien ser esta parte
	}
	else {
		log_info(loggerConsola, "Conexion exitosa");
		log_info(loggerConsola, "Enviando mensaje");
		enviar_mensaje("Hola kernel", conexion);
		//Lo movi aca porque esto solo pasa si la conexion esta bien
		//Si lo de arriba no funciona, directamente cierra y termina
	}

	//estas funciones van hasta implementar terminarModulo
	log_info(loggerConsola, "Finalizando Consola...\n");

	terminarModulo(conexion,loggerConsola/*, unconfig*/);

	printf ("Finalizo Consola correctamente\n ");

	return EXIT_SUCCESS;
}

/*
NOTAS:
(Dany)
-> Verificar la validacion de la conexion
-> El enviar mensaje lo puse adentro del if para que solo se mande si es exitoso
-> Agregue un par de logs info para ver bien si va todo bien en la terminal
-> Agregue utilsConsola
-> Comente lo de configs asi probamos de paso la nueva funcion, no olvidemos de comentar
*/
