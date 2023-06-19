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

int asignarHuecoLibre(t_segmento* nuevoSegmento, t_segmento*(*algoritmo)(uint32_t tamSegmento, t_list* lista, int tam));
t_segmento* FirstFit(uint32_t tamSegmento, t_list* listaHuecosLibres, int tamanioLista);
t_segmento* BestFit(uint32_t tamSegmento, t_list* listaHuecosLibres, int tamanioLista);
t_segmento* WorstFit(uint32_t tamSegmento, t_list* listaHuecosLibres, int tamanioLista);
algAsignacion asignarAlgoritmo();
void createSegment(t_segmento* nuevoSegmento, uint32_t tamanio);


#endif /* SRC_ALGORITMOSDEASIGNACION_H_ */
