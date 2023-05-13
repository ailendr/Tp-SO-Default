/*
 * utilsCpu.h
 *
 *  Created on: May 13, 2023
 *      Author: utnso
 */

#ifndef SRC_UTILSCPU_H_
#define SRC_UTILSCPU_H_

#include "./conexionCpu.h"

int iniciarCpu ();

t_instruccion* fetch(t_contextoEjec* cont);

#endif /* SRC_UTILSCPU_H_ */
