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

typedef enum{
	firstFit,
	bestFit,
	worstFit
}algoritmoDeAsignacion;

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
int memoriaOcupada(t_list* lista);
void liberarTablaDeSegmentos(uint32_t pid);
bool huecoLibre(t_segmento* segmento);
int  buscarPosSegmento(uint32_t idSegmento, t_list* lista);
void deleteSegment(uint32_t id);
void compactar();


#endif /* SRC_MEMORIAESTRUCTURAS_H_ */
