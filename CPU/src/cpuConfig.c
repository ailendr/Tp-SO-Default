/*
 * cpuConfig.c
 *
 *  Created on: Apr 14, 2023
 *      Author: utnso
 */

#include "cpuConfig.h"

t_config* configCPU;

char* IP_Memoria(){
	return config_get_string_value(configCPU, "IP_MEMORIA");
}

int puertoMemoria(){
	return config_get_int_value(configCPU, "PUERTO_MEMORIA");
}

char* IP_Filesystem(){
	return config_get_string_value(configCPU, "IP_FILESYSTEM");
}

int puertoFilesystem(){
	return config_get_int_value(configCPU, "PUERTO_FILESYSTEM");
}

char* IP_CPU(){
	return config_get_string_value(configCPU, "IP_CPU");
}

int puerto_CPU(){
	return config_get_int_value(configCPU, "PUERTO_CPU");
}

int puertoEscucha(){
	return config_get_int_value(configCPU, "PUERTO_ESCUCHA");
}

char* algoritmoPlanificacion(){
	return config_get_string_value(configCPU, "ALGORTIMO_PLANIFICACION");
}

int estimacionInicial(){
	return config_get_int_value(configCPU, "ESTIMACION_INICIAL");
}

int HRRRN_Alfa(){
	return config_get_int_value(configCPU, "HRRN_ALFA");
}

int maxGradoMultiprogramacion(){
	return config_get_int_value(configCPU, "GRADO_MAX_MULTIPROGRAMACION");
}

char** recursos(){
	return config_get_array_value(configCPU, "RECURSOS");
}

char** instanciasRecursos(){
	return config_get_array_value(configCPU, "INSTANCIAS_RECURSOS");
}


