/*
 * modulo.h
 *
 *  Created on: Apr 16, 2023
 *      Author: utnso
 */

#ifndef SRC_UTILS_MODULO_H_
#define SRC_UTILS_MODULO_H_

#include <commons/log.h>
#include <commons/config.h>
#include "sockets.h"

int verificarSocket (int socket, t_log* logger, t_config* config);
int verificarConfig (int socket, t_log* logger, t_config* config);

void terminarModulo(int conexion,t_log* log , t_config* config);

#endif /* SRC_UTILS_MODULO_H_ */
