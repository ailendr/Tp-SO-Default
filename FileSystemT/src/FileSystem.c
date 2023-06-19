#include <stdlib.h>
#include <stdio.h>
#include "FileSystem.h"


int main(/*int argc, char** argv*/) {

	// VERIFICAR QUE SE LLAMA CORRECTAMENTE A FILESYSTEM --------------------------------------------
	/*
	if(argc < 2){
		printf ("Faltan argumentos para poder ejecutar FILE SYSTEM. Revisar el llamado");
		return EXIT_FAILURE;
	}

	if(argc > 2){
		printf ("Se invoca a FILE SYSTEM con demasiados argumentos. Revisar el llamado");
		return EXIT_FAILURE;
	}
	*/

	// CREACION DE LOGGER Y CONFIG ------------------------------------------------------------------
	printf("Hola soy FileSystem y soy servidor de Kernel y me conecto a Memoria \n ");

	loggerFS = log_create("FileSystem.log", "FS",1,LOG_LEVEL_DEBUG);

	log_info(loggerFS, "---------------------------------------------------------------------------");
	log_info(loggerFS, "Iniciando FileSystem...");

    servidorFS = 0;

    //configFS = config_create(argv[1]);
	configFS = config_create("../FileSystemT/filesystem.config");

	if(verificarConfig (servidorFS, loggerFS, configFS) == 1 ) return EXIT_FAILURE;

	printf ("El valor recuperado de la ip es %s con el puerto %s\n", IP_Escucha(), puertoEscucha());


	// REALIZAR LAS CONEXIONES CON EL CLIENTE Y EL SERVIDOR -----------------------------------------
	iniciarMemoria ();
	iniciarServKernel ();

	// INICIALIZAR ESTRUCTURAS ----------------------------------------------------------------------
	iniciarEstructuras();
	peticiones = list_create();

	// INICIALIZAR SEMAFOROS ------------------------------------------------------------------------
	sem_init(&nuevoPedido,0, 0);

	// INICIALIZAR HILO  ----------------------------------------------------------------------------
    pthread_t hiloEjecutor;
    pthread_t hiloAtencion;

	pthread_create(&hiloAtencion,NULL,(void*)atenderPeticiones,NULL);
	pthread_create(&hiloEjecutor,NULL,(void*)ejecutarPeticiones,NULL);

	pthread_detach(hiloAtencion);
	pthread_detach(hiloEjecutor);

	// FINALIZAR MODULO -----------------------------------------------------------------------------
	log_info(loggerFS, "Finalizando File System...\n");

	list_destroy(peticiones);
	sem_destroy(&nuevoPedido);
	terminarModulo(servidorFS, loggerFS, configFS);
	close (socketMemoria);
	close (cliente);

	printf ("\n Finalizo File System correctamente\n ");

	return EXIT_SUCCESS;
}

 /*







*/

