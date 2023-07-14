/*
 * peticiones.h
 *
 *  Created on: Jun 17, 2023
 *      Author: utnso
 */

#ifndef SRC_PETICIONES_H_
#define SRC_PETICIONES_H_

#include "bloques.h"
#include <semaphore.h>
#include <pthread.h>
#include <stdint.h>

extern sem_t nuevoPedido;

void atenderPeticiones();
void ejecutarPeticiones();

#endif /* SRC_PETICIONES_H_ */