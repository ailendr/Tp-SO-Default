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

	configConsola = config_create("../Consola/consola.config");

	if (configConsola == NULL){
		log_error(loggerConsola,"Error al recuperar el config");
		log_destroy(loggerConsola);
		config_destroy(configConsola);
		return EXIT_FAILURE;
	}

	char* ip = IpKernel();
	char* puerto = PuertoKernel();

	printf ("\nEl valor recuperado de la ip es %s con el puerto %s\n", ip, puerto);

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
	config_destroy(configConsola);
	//Lo deje aca xq Kernel aun no lo tiene

	printf ("Finalizo Consola correctamente\n ");

	return EXIT_SUCCESS;
}

char* IpKernel(){
	return config_get_string_value(configConsola, "IP_KERNEL");
}

char* PuertoKernel(){
	return config_get_string_value(configConsola, "PUERTO_KERNEL");
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
