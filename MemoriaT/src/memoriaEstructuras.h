/*
 * memoriaEstructuras.h
 *
 *  Created on: Jun 10, 2023
 *      Author: utnso
 */

#ifndef SRC_MEMORIAESTRUCTURAS_H_
#define SRC_MEMORIAESTRUCTURAS_H_

#include "memoriaConfig.h"
#include <stdlib.h>
#include <stdint.h>
#include <commons/collections/list.h>
#include <utils/estructuras.h>


extern t_list* listaDeSegmentos;
extern t_list* listaHuecosLibres;
extern void* memoriaContigua;
extern t_segmento* segmentoCero;
extern t_list * listaDeTablas;

void iniciarEstructuras();
t_list* crearTablaDeSegmentos(uint32_t pid);
void crearSegmentoCero();
void crearListas();
void crearEspacioMemoria ();
int memoriaDisponible();
int memoriaOcupada();
void liberarTablaDeSegmentos(uint32_t pid);
void firstFIT(t_segmento* nuevoSegmento);


#endif /* SRC_MEMORIAESTRUCTURAS_H_ */
