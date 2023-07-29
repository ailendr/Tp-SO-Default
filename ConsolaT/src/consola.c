/*
 * consola.c
 *
 *  Created on: Apr 16, 2023
 *      Author: utnso
 */

#include "consola.h"

static void enviarInstruccionesAKernel(char* pathInstrucciones, int conexionConKernel){
	t_paquete *paqueteConInstrucciones = parseoDeInstrucciones(pathInstrucciones);
	enviar_paquete(paqueteConInstrucciones, conexionConKernel);
	log_info(loggerConsola,"Envio paquete con las instrucciones a kernel");
	eliminar_paquete(paqueteConInstrucciones);
}

int main(int argc, char** argv) {

	//printf ("Hola soy consola y quiero conectarme con kernel \n ");
	if(argc < 3){
		return EXIT_FAILURE;
	}

  loggerConsola = log_create("./consola.log","ConsolaT", 1, LOG_LEVEL_INFO);
	log_info(loggerConsola, "---------------------------------------------------------------------------");
	log_info(loggerConsola, "Iniciando Consola...");
	int conexionConKernel = 0;
	char* pathConfig = argv[1];
	char* pathInstrucciones = argv[2];

	configConsola = config_create(pathConfig);

	if( verificarConfig (conexionConKernel, loggerConsola, configConsola) == 1 ) return EXIT_FAILURE;

	char* ip = IpKernel();
	char* puerto = PuertoKernel();

	//printf ("El valor recuperado de la ip es %s con el puerto %s\n", ip, puerto);

	log_info(loggerConsola, "Iniciando como Cliente ... ");
	conexionConKernel = iniciarCliente(ip, puerto, loggerConsola);
	if( verificarSocket (conexionConKernel, loggerConsola, configConsola) == 1 ) return EXIT_FAILURE;

	log_info(loggerConsola, "Enviando mensaje de protocolo");

	if(enviarProtocolo(conexionConKernel, HANDSHAKE_Consola,loggerConsola) == -1){
	        terminarModulo(conexionConKernel,loggerConsola, configConsola);
	        return EXIT_FAILURE;
	}
//--Envia instrucciones y espera el mensaje de confirmacion de que llego OK--//
  enviarInstruccionesAKernel(pathInstrucciones, conexionConKernel);
  int recepcionInstrucciones;
  recv(conexionConKernel, &recepcionInstrucciones, sizeof(int), MSG_WAITALL);
  if(recepcionInstrucciones == 0){
	  log_info(loggerConsola, "Kernel nos informa que la lista de instrucciones llego vacia");
	  terminarModulo(conexionConKernel, loggerConsola, configConsola);
	  return EXIT_FAILURE;
  }
  else if(recepcionInstrucciones ==1 ){
	  log_info(loggerConsola, "Kernel nos informa que recibio correctamente la lista de instrucciones");
  }

 ///--- Esperando mensaje de finalizacion----///

	int finalizar;
	recv(conexionConKernel, &finalizar, sizeof(int), MSG_WAITALL);

	if(finalizar == -1){
		log_info(loggerConsola, "Finalizando Consola por aviso de Kernel");
		terminarModulo(conexionConKernel, loggerConsola, configConsola);
		return EXIT_FAILURE;
	}

	log_info(loggerConsola, "Finalizando Consola...\n");

	terminarModulo(conexionConKernel, loggerConsola, configConsola);

	printf ("Finalizo Consola correctamente\n ");

	return EXIT_SUCCESS;
}
