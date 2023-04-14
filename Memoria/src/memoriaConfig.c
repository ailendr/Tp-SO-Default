/*
 * memoriaConfig.c
 *
 *  Created on: Apr 14, 2023
 *      Author: utnso
 */


#include "memoriaConfig.h"


int puertoEscucha(){
	return config_get_int_value(configMemoria, "PUERTO_ESCUCHA");
}

int tam_memoria(){
	return config_get_int_value(configMemoria, "TAM_MEMORIA");
}

int cantSegmentos(){
	return config_get_int_value(configMemoria, "CANT_SEGMENTOS");
}

int retardoMemoria(){
	return config_get_int_value(configMemoria, "RETARDO_MEMORIA");
}

int retardoCompactacion(){
	return config_get_int_value(configMemoria, "RETARDO_COMPACTACION");
}

char* algortimoAsignacion(){
	return config_get_string_value(configMemoria, "ALGORITMO_ASIGNACION");
}
