/*
 * utilsCpu.h
 *
 *  Created on: May 13, 2023
 *      Author: utnso
 */

#ifndef SRC_UTILSCPU_H_
#define SRC_UTILSCPU_H_

#include "./cpuConfig.h"
#include <math.h>

extern int servidorCpu;
extern int socketMemoria;
extern int cliente;

int iniciarCpu (char* pathConfig);

int iniciarSocketsCpu();

char* mmu (char* direccionLogica, uint32_t id, int tam);
int tamRegistro (char* registro);

#endif /* SRC_UTILSCPU_H_ */
