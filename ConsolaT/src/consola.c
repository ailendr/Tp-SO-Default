/*
 * consola.c
 *
 *  Created on: Apr 16, 2023
 *      Author: utnso
 */

#include "consola.h"

int main(int argc, char** argv) {
	/*NOTA: argv[0]: almacena el nombre del programa.
			argv[1]: almacena el primer argumento.
			argv[2]: almacena el segundo argumento. */

//validacion de cantidad de argumentos
	 if (argc < 3) {
	        return EXIT_FAILURE;
	    }

	printf ("Hola soy consola y quiero conectarme con kernel \n ");

	loggerConsola = log_create("./consola.log","Consola", 1, LOG_LEVEL_INFO);

	log_info(loggerConsola, "---------------------------------------------------------------------------");

	log_info(loggerConsola, "Iniciando Consola...");

	int conexion = 0;
    char* pathConfig = argv[1];
    char* pathPseudo = argv[2];

	configConsola = config_create(pathConfig);

	if( verificarConfig (conexion, loggerConsola, configConsola) == 1 ) return EXIT_FAILURE;

	char* ip = IpKernel();
	char* puerto = PuertoKernel();

	printf ("El valor recuperado de la ip es %s con el puerto %s\n", ip, puerto);

	t_list* listaInstrucciones = crearLista(pathPseudo);


	//conexion//
	log_info(loggerConsola, "Iniciando como Cliente ... \n");
	conexion = iniciarCliente(ip, puerto, loggerConsola);
	if( verificarSocket (conexion, loggerConsola, configConsola) == 1 ) return EXIT_FAILURE;

	log_info(loggerConsola, "Enviando mensaje de protocolo");

	if(enviarProtocolo(conexion, loggerConsola) == -1){
	        terminarModulo(conexion,loggerConsola, configConsola);
	        return EXIT_FAILURE;
	}

	log_info(loggerConsola, "Enviando instrucciones");
	enviarListaDeIntrucciones(listaInstrucciones, conexion);

	log_info(loggerConsola, "Finalizando Consola...\n");

	terminarModulo(conexion, loggerConsola, configConsola);

	printf ("Finalizo Consola correctamente\n ");

	return EXIT_SUCCESS;
}
