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

int main(void) {
	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */

	char* ip, puerto;

	ip = "127.0.0.1";
	puerto = "8000";

	int conexion = crear_conexion(ip, puerto);

	return EXIT_SUCCESS;
}
