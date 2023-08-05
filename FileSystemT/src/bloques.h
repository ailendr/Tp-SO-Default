/*
 * bloques.h
 *
 *  Created on: Jun 16, 2023
 *      Author: utnso
 */

#ifndef SRC_BLOQUES_H_
#define SRC_BLOQUES_H_

#include <stdint.h>

#include "FSEstructuras.h"

int cantBloques (uint32_t tamanio);
void eliminarBloques (int cantidadDeBloques, t_fcb* fcb);
void aumentar (int cantidadDeBloques, t_fcb* fcb);
int proxBloqueVacio();
uint32_t asignarBloque();
void agregar_bloque(t_fcb* fcb, int* asignados);

void escribirBloque (void* contenido, uint32_t sizeContenido, uint32_t numeroBloque);
void agregarContenidoABloque (void* contenido, uint32_t sizeContenido, uint32_t offset , uint32_t numeroBloque);
void liberarBloque(uint32_t numeroBloque);

void leerArchivoBloques(void* aLeer, int posicion, int cantidad);
void escribirArchivoBloques(void* aEscribir, int posicion, int cantidad);

#endif /* SRC_BLOQUES_H_ */
