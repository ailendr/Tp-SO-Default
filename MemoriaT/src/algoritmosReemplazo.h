/*
 * algoritmosReemplazo.h
 *
 *  Created on: Jun 18, 2023
 *      Author: utnso
 */

#ifndef SRC_ALGORITMOSREEMPLAZO_H_
#define SRC_ALGORITMOSREEMPLAZO_H_

#include "memoriaEstructuras.h"

void asignarHuecoLibre(t_segmento* nuevoSegmento, t_segmento*(*algoritmo)(uint32_t tamSegmento, t_list* lista, int tam));
t_segmento* FirstFit(uint32_t tamSegmento, t_list* listaHuecosLibres, int tamanioLista);
t_segmento* BestFit(uint32_t tamSegmento, t_list* listaHuecosLibres, int tamanioLista);
t_segmento* WorstFit(uint32_t tamSegmento, t_list* listaHuecosLibres, int tamanioLista);
//void createSegment(uint32_t tamanio);


#endif /* SRC_ALGORITMOSREEMPLAZO_H_ */
