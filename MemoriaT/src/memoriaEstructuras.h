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
//extern t_list* listaHuecosLibres;
extern void* memoriaContigua;
extern t_segmento* segmentoCero;
extern t_list * listaDeTablas;
extern t_segmento* segmentoLibre;

void iniciarEstructuras();
t_list* crearTablaDeSegmentos(uint32_t pid);
void crearSegmentoCero();
void crearListas();
void crearEspacioMemoria ();
int memoriaDisponible();
int memoriaOcupada(t_list* lista);
void liberarTablaDeSegmentos(uint32_t pid);
bool huecoLibre(t_segmento* segmento);
void actualizarUltimoSegmentoLibre();
int  buscarPosSegmento(uint32_t idSegmento, uint32_t pid, t_list* lista);
void deleteSegment(uint32_t id, uint32_t pid);
void compactar();


#endif /* SRC_MEMORIAESTRUCTURAS_H_ */
