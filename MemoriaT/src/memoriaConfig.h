/*
 * memoriaConfig.h
 *
 *  Created on: Apr 16, 2023
 *      Author: utnso
 */

#ifndef SRC_MEMORIACONFIG_H_
#define SRC_MEMORIACONFIG_H_

#include <utils/modulo.h>
#include <commons/string.h>
extern t_log* loggerMemoria;
extern t_config* configMemoria;

char* puertoEscucha();
char* ipEscucha();
int tam_memoria();
int tam_segmento_cero();
int cantSegmentos();
int retardoMemoria();
int retardoCompactacion();
char* algoritmoAsignacion();

#endif /* SRC_MEMORIACONFIG_H_ */
