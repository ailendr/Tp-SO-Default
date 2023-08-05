/*
 * peticiones.h
 *
 *  Created on: Jun 17, 2023
 *      Author: utnso
 */

#ifndef SRC_PETICIONES_H_
#define SRC_PETICIONES_H_

#include "operaciones.h"
#include <semaphore.h>
#include <pthread.h>

void atenderPeticiones();
void ejecutarPeticiones(t_instruccion* instruccion);

#endif /* SRC_PETICIONES_H_ */
