/*
 * modulo.c
 *
 *  Created on: Apr 16, 2023
 *      Author: utnso
 */

#include "modulo.h"

void terminarModulo(int conexion, t_log* logger, t_config* config)
{

	if(logger != NULL){
		log_destroy(logger);
	}

	if(config != NULL){
		config_destroy(config);
	}

	close (conexion);
}
