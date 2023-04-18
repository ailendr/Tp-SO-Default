/*
 * utilsConsola.c
 *
 *  Created on: Apr 16, 2023
 *      Author: utnso
 */


#include "ConsolaConfig.h"

t_log* loggerConsola;
t_config* configConsola;

char* IpKernel(){
	return config_get_string_value(configConsola, "IP_KERNEL");
}

char* PuertoKernel(){
	return config_get_string_value(configConsola, "PUERTO_KERNEL");
}

