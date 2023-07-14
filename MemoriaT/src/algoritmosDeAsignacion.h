/*
 * algoritmosReemplazo.h
 *
 *  Created on: Jun 18, 2023
 *      Author: utnso
 */

#ifndef SRC_ALGORITMOSDEASIGNACION_H_
#define SRC_ALGORITMOSDEASIGNACION_H_

#include "memoriaEstructuras.h"



typedef enum{
	firstFit,
	bestFit,
	worstFit
}algAsignacion;


int FirstFit(uint32_t tamSegmento);
int huecoLibreDisponible(uint32_t tamSegmento, t_list* listaDeSegmentos);
bool BestFit(t_segmento* segmento1, t_segmento* segmento2);
bool WorstFit(t_segmento* segmento1, t_segmento* segmento2);
int WorstYBest(uint32_t tamSegmento, t_list* listaHuecosLibres, bool(algoritmo)(t_segmento* seg1, t_segmento* seg2));
algAsignacion asignarAlgoritmo();
uint32_t createSegment(t_segmento* nuevoSegmento, uint32_t tamanio);



#endif /* SRC_ALGORITMOSDEASIGNACION_H_ */
