/*
 * modulo.c
 *
 *  Created on: Apr 16, 2023
 *      Author: utnso
 */

#include "modulo.h"

int verificarSocket (int socket, t_log* logger, t_config* config){

	if(socket == -1){
		log_error(logger,"Error al iniciar el socket");
		terminarModulo(socket,logger, config);
		return 1;
	}

	return 0;
}

int verificarConfig (int socket, t_log* logger, t_config* config){

	if (config == NULL){
		log_error(logger,"Error al recuperar el config");
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
