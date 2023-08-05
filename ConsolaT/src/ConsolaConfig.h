/*
 * utilsConsola.h
 *
 *  Created on: Apr 16, 2023
 *      Author: utnso
 */

#ifndef SRC_CONSOLACONFIG_H_
#define SRC_CONSOLACONFIG_H_

#include <utils/modulo.h>

extern t_config* configConsola;
extern t_log* loggerConsola;
extern t_config* configIPs;

char* IpKernel();
char* PuertoKernel();

#endif /* SRC_CONSOLACONFIG_H_ */
