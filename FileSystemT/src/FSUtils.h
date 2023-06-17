/*
 * conexionFS.h
 *
 *  Created on: Jun 17, 2023
 *      Author: utnso
 */

#ifndef SRC_FSUTILS_H_
#define SRC_FSUTILS_H_

#include "FSConfig.h"

extern int servidorFS;
extern int socketMemoria;
extern int cliente;

int iniciarMemoria ();
int iniciarServKernel ();

#endif /* SRC_FSUTILS_H_ */
