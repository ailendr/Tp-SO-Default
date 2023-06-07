/*
 * utilsCpu.h
 *
 *  Created on: May 13, 2023
 *      Author: utnso
 */

#ifndef SRC_UTILSCPU_H_
#define SRC_UTILSCPU_H_

#include "./instruccion.h"

int iniciarCpu (char* pathConfig);

char* fetch(t_contextoEjec* cont);
t_instruccion* decode (char* instruccion);
void execute (t_instruccion* instruccion, t_contextoEjec* contexto);

op_code asignarNombre (char* nombre);

void funcionPrueba();
void preparandoContexto (t_contextoEjec* contexto);

#endif /* SRC_UTILSCPU_H_ */
