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


extern sem_t nuevoPedido;

void atenderPeticiones();
void ejecutarPeticiones();

#endif /* SRC_PETICIONES_H_ */
