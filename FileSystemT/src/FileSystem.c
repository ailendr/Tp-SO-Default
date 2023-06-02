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

	log_info(loggerFS, "Iniciando conexion con Memoria ... \n");
	char* ipM = IP_Memoria();
	char* puertoM = puertoMemoria();

	int socketMemoria = iniciarCliente(ipM, puertoM, loggerFS);
	if( verificarSocket (socketMemoria, loggerFS, configFS) == 1 ) return EXIT_FAILURE;

	log_info(loggerFS, "Enviando mensaje...");
    if(enviarProtocolo(socketMemoria,HANDSHAKE_Fs, loggerFS) == -1){
        terminarModulo(socketMemoria,loggerFS, configFS);
        return EXIT_FAILURE;
    }

	log_info(loggerFS, "---------------------------------------------------------------------------");
    log_info(loggerFS, "Iniciando Servidor ... ");
    servidorFS = iniciarServidor(ip, puerto);
    if( verificarSocket (servidorFS, loggerFS, configFS) == 1 ) return EXIT_FAILURE;
    log_info(loggerFS, "Servidor listo para recibir a Kernel " );
    log_info(loggerFS, "Esperando un Cliente ...");
    int cliente = esperar_cliente(servidorFS, loggerFS);
    if( verificarSocket (cliente, loggerFS, configFS) == 1 ){
    	close(servidorFS);
    	return EXIT_FAILURE;
    }
    recibirHandshake(cliente, HANDSHAKE_Kernel, loggerFS);

	log_info(loggerFS, "Finalizando File System...\n");

	terminarModulo(cliente, loggerFS, configFS);
	close (socketMemoria);
	close (servidorFS);

	printf ("\n Finalizo File System correctamente\n ");

	return EXIT_SUCCESS;
}

