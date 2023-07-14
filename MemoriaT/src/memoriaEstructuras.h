/*
 * memoriaEstructuras.h
 *
 *  Created on: Jun 10, 2023
 *      Author: utnso
 */

#ifndef SRC_MEMORIAESTRUCTURAS_H_
#define SRC_MEMORIAESTRUCTURAS_H_

#include "memoriaConfig.h"


extern t_list* listaDeSegmentos;
extern void* memoriaContigua;
extern t_segmento* segmentoCero;
extern t_list * listaDeTablas;
extern t_segmento* segmentoLibre;

//ESTRUCTURAS Y UTILS DE MEMORIA
void iniciarEstructuras();
void crearSegmentoCero();
void crearListas();
void crearEspacioMemoria ();
int memoriaDisponible();
int memoriaOcupada(t_list* lista);
bool huecoLibre(t_segmento* segmento);
bool segmentoOcupado(t_segmento* segmento);
void actualizarUltimoSegmentoLibre();
int  buscarPosSegmento(uint32_t idSegmento, uint32_t pid, t_list* lista);
//void actualizarListaDeSegmentos(t_segmento* nuevoSegmento, t_segmento* huecoLibre);
void logearListaDeSegmentos(char* mensaje);
void actualizarListaDeSegmentos(int pos, t_segmento* segmento);

//FUNCIONES DE MEMORIA
t_tabla* crearTablaDeSegmentos(uint32_t pid);
void liberarTablaDeSegmentos(uint32_t pid);
t_tabla* deleteSegment(uint32_t id, uint32_t pid); //Me sirve que retorne la tabla actualizada
void unirHuecosAledanios(t_segmento* segmento);
void actualizarTablaDeSegmentos(t_list* tablaDeSegmentos);
void compactar();


#endif /* SRC_MEMORIAESTRUCTURAS_H_ */
