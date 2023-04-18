/*
 * FS.h
 *
 *  Created on: Apr 16, 2023
 *      Author: utnso
 */

#ifndef SRC_FSCONFIG_H_
#define SRC_FSCONFIG_H_

#include <utils/modulo.h>

extern t_log* loggerFS;
extern t_config* configFS;

char* IP_Memoria();
char* IP_Escucha();
char* puertoMemoria();
char* puertoEscucha();
char* pathSuperbloque();
char* pathBitmap();
char* pathBloques();
char* pathFCB();
int retardoNumerico();

#endif /* SRC_FSCONFIG_H_ */
