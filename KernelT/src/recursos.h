/*
 * recursos.h
 *
 *  Created on: May 25, 2023
 *      Author: utnso
 */

#ifndef SRC_RECURSOS_H_
#define SRC_RECURSOS_H_

#include "kernelConfig.h"
#include "kernelGlobales.h"

t_list* crearListaDeBloqueo();
t_list* crearListaDeInstancias();
void crearEstructurasDeRecursos();
int recursoDisponible(char* nombre);
void fwait(int posRecurso);
void implementacionWait(char* nombreRecurso);
void fsignal(int posRecurso);
void implementacionSignal(char* nombreRecurso);

#endif /* SRC_RECURSOS_H_ */
