/*
 * modulo.c
 *
 *  Created on: Apr 16, 2023
 *      Author: utnso
 */

#include "modulo.h"

int verificarServidor (int socket, t_log* logger, t_config* config){

	if(socket == -1){
		log_error(logger,"Error al iniciar el servidor");
		terminarModulo(socket,logger, config);
		return 1;
	}

	return 0;
}

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
