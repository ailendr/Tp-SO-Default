/*
 * bloques.h
 *
 *  Created on: Jun 16, 2023
 *      Author: utnso
 */

#ifndef SRC_BLOQUES_H_
#define SRC_BLOQUES_H_

#include <commons/bitarray.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdint.h>

#include "FSEstructuras.h"

extern t_bitarray* bitMap;
extern pthread_mutex_t mutexBlocks;

void escribirBloque (void* contenido, uint32_t sizeContenido, uint32_t numeroBloque);
void agregarContenidoABloque (void* contenido, uint32_t sizeContenido, uint32_t offset , uint32_t numeroBloque);
void liberarBloque(uint32_t numeroBloque);

#endif /* SRC_BLOQUES_H_ */
