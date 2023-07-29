/*
 * FS.h
 *
 *  Created on: Apr 16, 2023
 *      Author: utnso
 */

#ifndef SRC_FSCONFIG_H_
#define SRC_FSCONFIG_H_

#include <utils/modulo.h>
#include <commons/string.h>
#include <string.h>

extern t_log* loggerFS;
extern t_config* configFS;
//Agrego los sickets
extern int servidorFS;
extern int socketMemoria;
extern int cliente;


char* IP_Memoria();
char* IP_Escucha();
char* puertoMemoria();
char* puertoEscucha();
char* pathSuperBloque();
char* pathBitmap();
char* pathBloques();
char* pathFCB();
int retardoNumerico();

#endif /* SRC_FSCONFIG_H_ */
