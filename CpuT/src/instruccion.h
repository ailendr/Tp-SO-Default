/*
 * instruccion.h
 *
 *  Created on: May 15, 2023
 *      Author: utnso
 */

#ifndef SRC_INSTRUCCION_H_
#define SRC_INSTRUCCION_H_

#include "./cpuConfig.h"
extern t_contextoEjec* contextoRecibido;
extern int servidorCpu;
extern int socketMemoria;
extern int cliente;

char* fetch(t_contextoEjec* cont);
t_instruccion* decode (char* instruccion);
void execute (t_instruccion* instruccion, t_contextoEjec* contexto);

op_code asignarNombre (char* nombre);

void set (t_instruccion* instruccion, t_contextoEjec* contexto);

#endif /* SRC_INSTRUCCION_H_ */
