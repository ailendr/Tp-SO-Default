/*
 * algoritmosReemplazo.h
 *
 *  Created on: Jun 18, 2023
 *      Author: utnso
 */

#ifndef SRC_ALGORITMOSREEMPLAZO_H_
#define SRC_ALGORITMOSREEMPLAZO_H_

#include "memoriaEstructuras.h"

void asignarHuecoLibre(t_segmento* nuevoSegmento, t_segmento*(*algoritmo)(t_segmento* segmento, t_list* lista, int tam));
t_segmento* firstFit(t_segmento* nuevoSegmento, t_list* listaHuecosLibres, int tamanioLista);
t_segmento*bestFit(t_segmento* nuevoSegmento, t_list* listaHuecosLibres, int tamanioLista);
t_segmento* worstFit(t_segmento* nuevoSegmento, t_list* listaHuecosLibres, int tamanioLista);


#endif /* SRC_ALGORITMOSREEMPLAZO_H_ */
