/*
 * filesystemConfig.c
 *
 *  Created on: Apr 14, 2023
 *      Author: utnso
 */

#include "FSConfig.h"

char* IP_Memoria(){
	return config_get_string_value(configFS, "IP_MEMORIA");
}

int puertoMemoria(){
	return config_get_int_value(configFS, "PUERTO_MEMORIA");
}

int puertoEscucha(){
	return config_get_int_value(configFS, "PUERTO_ESCUCHA");
}

char* pathSuperbloque(){
	return config_get_string_value(configFS, "PATH_SUPERBLOQUE");
}

char* pathBitmap(){
	return config_get_string_value(configFS, "PATH_BITMAP");
}

char* pathBloques(){
	return config_get_string_value(configFS, "PATH_BLOQUES");
}

char* pathFCB(){
	return config_get_string_value(configFS, "PATH_FCB");
}

int retardoNumerico(){
	return config_get_int_value(configFS, "RETARDO_NUMERICO");
}




