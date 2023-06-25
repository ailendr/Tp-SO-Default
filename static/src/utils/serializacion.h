/*
 * serializacion.h
 *
 *  Created on: May 28, 2023
 *      Author: utnso
 */

#ifndef SRC_UTILS_SERIALIZACION_H_
#define SRC_UTILS_SERIALIZACION_H_

#include <string.h>
#include "sockets.h"

t_list* deserializarInstrucciones(void *buffer, int desplazamiento, int tamanioBuffer);
t_contextoEjec* deserializarContexto(void *buffer, int tamanio);
t_paquete* serializarContexto(t_contextoEjec *procesoAEjecutar);


t_paquete* serializarInstruccion(t_instruccion* instruccion);
//t_paquete* serializacionParametros (t_paquete* pInstruc, int offset, t_instruccion* instruc, int param);
t_instruccion* deserializarInstruccionEstructura (void* buffer, int cantParam);
t_instruccion* obtenerInstruccion(int socket, int cantParam);
//Serializacion y deserializacion de tabla de segmento//
t_buffer* serializarTablaDeSegmentos(t_list* tabla);
void serializarSegmento(t_segmento* segmento, t_buffer* buffer);
t_segmento* deserializarSegmento(void* buffer, int desplazamiento);
t_list* deserializarTablaDeSegmentos(int socket);


///Hecho por naty///
int cantidadDeParametros(op_code instruccion);
void agregarParametros (t_paquete* pInstruc, t_instruccion* instruc, int param);
void agregarParametro1(t_paquete* pInstruc,t_instruccion* instruc);
void agregarParametro2(t_paquete* pInstruc,t_instruccion* instruc);
void agregarParametro3(t_paquete* pInstruc,t_instruccion* instruc);




#endif /* SRC_UTILS_SERIALIZACION_H_ */
