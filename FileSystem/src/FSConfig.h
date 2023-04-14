/*
 * filesystemConfig.h
 *
 *  Created on: Apr 14, 2023
 *      Author: utnso
 */

#ifndef FSCONFIG_H_
#define FSCONFIG_H_

#include <commons/log.h>
#include <commons/config.h>

t_log* loggerFS;
t_config* configFS;

char* IP_Memoria();
int puertoMemoria();
int puertoEscucha();
char* pathSuperbloque();
char* pathBitmap();
char* pathBloques();
char* pathFCB();
int retardoNumerico();

#endif /* FSCONFIG_H_ */
