/*
 * utilsKernel.c
 *
 *  Created on: Apr 13, 2023
 *      Author: utnso
 */

//Creo este archivo para que no se pierda el back up de utilsServidor.c

#include "utilsKernel.h"

t_log* loggerKernel;
t_config* configKernel;


char* IpMemoria(){
	return config_get_string_value(configKernel, "IP_MEMORIA");
}
char* IpFile(){
	return config_get_string_value(configKernel, "IP_FILESYSTEM");
}
char* IpCPU(){
	return config_get_string_value(configKernel, "IP_CPU");
}
char* Ip(){
	return config_get_string_value(configKernel, "IP_ESCUCHA");
}

char* PuertoMemoria(){
	return config_get_string_value(configKernel, "PUERTO_MEMORIA");
}
char* PuertoFileSystem(){
	return config_get_string_value(configKernel, "PUERTO_FILESYSTEM");
}
char* PuertoCPU(){
	return config_get_string_value(configKernel, "PUERTO_CPU");
}
char* Puerto(){
	return config_get_string_value(configKernel, "PUERTO_ESCUCHA");
}

char* Algoritmo(){
	return config_get_string_value(configKernel, "ALGORITMO_PLANIFICACION");
}

int Estimacion(){
	return config_get_int_value(configKernel, "ESTIMACION_INICIAL");
}

int Alfa(){
	return config_get_double_value(configKernel, "HRRN_ALFA");
}

int Multiprogramacion(){
	return config_get_int_value(configKernel, "GRADO_MAX_MULTIPROGRAMACION");
}

char** Recursos(){
	return config_get_array_value(configKernel,"RECURSOS");
}

char** Instancias(){
	return config_get_array_value(configKernel,"INSTANCIAS_RECURSOS");
}
