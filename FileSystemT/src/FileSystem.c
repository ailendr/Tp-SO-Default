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

	// REALIZAR LAS CONEXIONES CON EL CLIENTE  -----------------------------------------
	iniciarMemoria ();
	// INICIALIZAR ESTRUCTURAS DESPUES DE SER CLIENTE DE MEMORIA  ----------------------------------------------------------------------
	iniciarEstructuras();
	peticiones = list_create();
	fcbs = list_create();
	// REALIZAR LAS CONEXIONES COMO SERVIDOR  -----------------------------------------

	t_instruccion* nuevaInstruc = malloc(sizeof(t_instruccion));
		nuevaInstruc -> nombre = F_CREATE;
		nuevaInstruc -> pid = 1;
		nuevaInstruc -> param1 = "DATYYYYYYYYYYYY.config";
		list_add(peticiones, nuevaInstruc);

	t_instruccion* nuevaInstruc2 = malloc(sizeof(t_instruccion));
	nuevaInstruc2 -> nombre = F_OPEN;
	nuevaInstruc2 -> pid = 1;
	nuevaInstruc2 -> param1 = "NATYYYYYYYYYYYY.config";

	t_instruccion* nuevaInstruc3 = malloc(sizeof(t_instruccion));
		nuevaInstruc3 -> nombre = F_OPEN;
		nuevaInstruc3 -> pid = 1;
		nuevaInstruc3 -> param1 = "DATYYYYYYYYYYYY.config";

	list_add(peticiones, nuevaInstruc3);
	list_add(peticiones, nuevaInstruc3);

	iniciarServKernel ();
	for (int j = 0; j<3 ; j++){
		ejecutarPeticiones();
	}


	atenderPeticiones();

	// FINALIZAR MODULO -----------------------------------------------------------------------------
	log_info(loggerFS, "Finalizando File System...\n");

	list_destroy(peticiones);
	if (fcbs != NULL){
		finalizarListaFcb();
	}
	terminarModulo(servidorFS, loggerFS, configFS);
	close (socketMemoria);
	close (cliente);

	printf ("\n Finalizo File System correctamente\n ");

	return EXIT_SUCCESS;
}
