/*
 * recursos.h
 *
 *  Created on: May 25, 2023
 *      Author: utnso
 */

#ifndef SRC_RECURSOS_H_
#define SRC_RECURSOS_H_
#include <stdlib.h>
#include "kernelConfig.h"
#include "kernelGlobales.h"

t_list* crearListaDeBloqueo();
t_list* crearListaDeInstancias();
void crearEstructurasDeRecursos();
int recursoDisponible(char* nombre);


#endif /* SRC_RECURSOS_H_ */
