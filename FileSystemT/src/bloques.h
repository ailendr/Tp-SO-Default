/*
 * bloques.h
 *
 *  Created on: Jun 16, 2023
 *      Author: utnso
 */

#ifndef SRC_BLOQUES_H_
#define SRC_BLOQUES_H_

#include "estructurasfs.h"

extern pthread_mutex_t mutexBlocks;

void escribirEnBloque (void* contenido, uint32_t sizeContenido, uint32_t numeroBloque);
void agregarContenidoABloque (void* contenido, uint32_t sizeContenido, uint32_t offset , uint32_t numeroBloque);
void liberarBloque(uint32_t numeroBloque);

#endif /* SRC_BLOQUES_H_ */
