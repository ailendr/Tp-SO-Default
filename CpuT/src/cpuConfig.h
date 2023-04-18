/*
 * cpuConfig.h
 *
 *  Created on: Apr 16, 2023
 *      Author: utnso
 */

#ifndef SRC_CPUCONFIG_H_
#define SRC_CPUCONFIG_H_

#include <utils/modulo.h>

extern t_log* loggerCPU;
extern t_config* configCPU;

//IP
char* IP_Memoria();
char* IP_Escucha();
//PUERTO
char* puertoMemoria();
char* puertoEscucha();
//RETARDO INSTRUCCIONES
int retardo();
//TAMAÑO MAXIMO SEGMENTO
int tamSegmento();

#endif /* SRC_CPUCONFIG_H_ */
