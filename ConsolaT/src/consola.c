/*
 * consola.c
 *
 *  Created on: Apr 16, 2023
 *      Author: utnso
 */

#include "consola.h"
//#include "parseoInstrucciones.h"


void enviarInstruccionesAKernel(const char* pathInstrucciones, int conexionConKernel){
	//t_paquete *paqueteConInstrucciones = crear_paquete();
	//parseoDeInstrucciones(pathInstrucciones);
	//enviarPaquete(paqueteConInstrucciones, conexionConKernel);
	//log_info(loggerConsola,"Envio paquete con las instrucciones a kernel");

}

int main(int argc, char *argv[]) {

	//printf ("Hola soy consola y quiero conectarme con kernel \n ");

	if(argc < 3){
		return EXIT_FAILURE;
	}
	loggerConsola = log_create("./consola.log","ConsolaT", 1, LOG_LEVEL_INFO);
	log_info(loggerConsola, "---------------------------------------------------------------------------");
	log_info(loggerConsola, "Iniciando Consola...");
	int conexionConKernel = 0;
	char* pathConfig = argv[1];
	configConsola = config_create("../ConsolaT/consola.config");

	if( verificarConfig (conexionConKernel, loggerConsola, configConsola) == 1 ) return EXIT_FAILURE;

	char* ip = IpKernel();
	char* puerto = PuertoKernel();

	//printf ("El valor recuperado de la ip es %s con el puerto %s\n", ip, puerto);
	log_info(loggerConsola, "Iniciando como Cliente ... \n");
	conexionConKernel = iniciarCliente(ip, puerto, loggerConsola);
	if( verificarSocket (conexionConKernel, loggerConsola, configConsola) == 1 ) return EXIT_FAILURE;

	char *pathInstrucciones = argv[2]; //o *archivoInstruciones
	enviarInstruccionesAKernel(pathInstrucciones, conexionConKernel);
	//log_info(loggerConsola, "Enviando mensaje");

	//if(enviarProtocolo(conexion, loggerConsola) == -1){
	  //      terminarModulo(conexion,loggerConsola, configConsola);
	  //      return EXIT_FAILURE;
	//}

	log_info(loggerConsola, "Finalizando Consola...\n");

	terminarModulo(conexionConKernel, loggerConsola, configConsola);

	printf ("Finalizo Consola correctamente\n ");

	return EXIT_SUCCESS;
}
