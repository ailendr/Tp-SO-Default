
#ifndef SRC_INSTRUCCION_H_
#define SRC_INSTRUCCION_H_

#include "./utilsCpu.h"

char* fetch(t_contextoEjec* cont);
t_instruccion* decode (char* instruccion, t_contextoEjec* contexto);
int execute (t_instruccion* instruccion, t_contextoEjec* contexto);

op_code asignarNombre (char* nombre);

void set (t_instruccion* instruccion, t_contextoEjec* contexto);
void moveIn (t_instruccion* instruccion, t_contextoEjec* contexto);
void moveOut (t_instruccion* instruccion, t_contextoEjec* contexto);

#endif /* SRC_INSTRUCCION_H_ */
