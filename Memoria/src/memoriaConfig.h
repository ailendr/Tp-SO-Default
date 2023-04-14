/*
 * memoriaConfig.h
 *
 *  Created on: Apr 14, 2023
 *      Author: utnso
 */

#ifndef MEMORIACONFIG_H_
#define MEMORIACONFIG_H_

#include <commons/log.h>
#include <commons/config.h>

t_log* loggerMemoria;
t_config* configMemoria;

int puertoEscucha();
int tam_memoria();
int cantSegmentos();
int retardoMemoria();
int retardoCompactacion();
char* algortimoAsignacion();

#endif /* MEMORIACONFIG_H_ */
