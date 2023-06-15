/*
 * memoriaConfig.h
 *
 *  Created on: Apr 16, 2023
 *      Author: utnso
 */

#ifndef SRC_MEMORIACONFIG_H_
#define SRC_MEMORIACONFIG_H_

#include <commons/log.h>
#include <commons/config.h>
#include <utils/modulo.h>

extern t_log* loggerMemoria;
extern t_config* configMemoria;

char* puertoEscucha();
char* ipEscucha();
int tam_memoria();
int tam_segmento_cero();
int cantSegmentos();
int retardoMemoria();
int retardoCompactacion();
char* algortimoAsignacion();

#endif /* SRC_MEMORIACONFIG_H_ */
