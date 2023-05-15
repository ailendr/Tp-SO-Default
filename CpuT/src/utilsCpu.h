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

char* fetch(t_contextoEjec* cont);
t_instruccion* decode (char* instruccion);
void execute (t_instruccion* instruccion, t_contextoEjec* contexto);

#endif /* SRC_UTILSCPU_H_ */
