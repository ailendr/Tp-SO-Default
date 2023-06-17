/*
 * instruccion.h
 *
 *  Created on: May 15, 2023
 *      Author: utnso
 */

#ifndef SRC_INSTRUCCION_H_
#define SRC_INSTRUCCION_H_

#include "./utilsCpu.h"

char* fetch(t_contextoEjec* cont);
t_instruccion* decode (char* instruccion);
int execute (t_instruccion* instruccion, t_contextoEjec* contexto);

op_code asignarNombre (char* nombre);

void set (t_instruccion* instruccion, t_contextoEjec* contexto);
void moveIn (t_instruccion* instruccion, t_contextoEjec* contexto);
void moveOut (t_instruccion* instruccion, t_contextoEjec* contexto);

#endif /* SRC_INSTRUCCION_H_ */
