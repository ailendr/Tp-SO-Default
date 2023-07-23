/*
 * serializacion.h
 *
 *  Created on: May 28, 2023
 *      Author: utnso
 */

#ifndef SRC_UTILS_SERIALIZACION_H_
#define SRC_UTILS_SERIALIZACION_H_

#include "sockets.h"

t_list* deserializarInstrucciones(void *buffer, int* desplazamiento,int tamanioBuffer);
//t_contextoEjec* deserializarContextoViejo(void *buffer, int tamanioContexto, int* desplazamiento);
t_paquete* serializarContexto(t_contextoEjec *procesoAEjecutar);


t_paquete* serializarInstruccion(t_instruccion* instruccion);
//t_paquete* serializacionParametros (t_paquete* pInstruc, int offset, t_instruccion* instruc, int param);
t_instruccion* deserializarInstruccionEstructura (void* buffer, int cantParam, int* desplazamiento);
t_instruccion* obtenerInstruccion(int socket, int cantParam);

//Lo que se usa para serializar y deserializar CONTEXTO COMPUESTO//
t_paquete* serializarContextoCompuesto(t_contextoEjec *procesoAEjecutar, t_instruccion* instruc);
int bytesListaInstrucciones(t_list* instrucciones);
int recibirTamContexto(void* buffer, int* desplazamiento);
int recibirTipoInstruccion(void* buffer, int* desplazamiento);
t_contextoEjec* deserializarContexto(void *buffer, int tamanioContexto, int* desplazamiento); //sirve tanto para el contexto solo como para el q viene dentro del buffer con la instruccion)(
t_list* deserializarListaInstruc(void *buffer, int* desplazamiento,int tamanioBuffer, int offset);



//Serializacion y deserializacion de tabla de segmento//
void serializarSegmento(t_segmento* segmento, t_buffer* buffer);
t_segmento* deserializarSegmento(void* buffer, int* desplazamiento);
void serializarTablaDeSegmentos(t_tabla* tabla, t_buffer* buffer);
t_tabla* deserializarTablaDeSegmentos(void* buffer,int* desplazamiento, int size);

//Serializacion y deserializacion Lista de Tablas//
t_buffer* serializarListaDeTablas(t_list* listaDeTablas);
t_list* deserializarListaDeTablas(int socket);
t_tabla* deserializarTabla(void* buffer,int* desplazamiento, int size);


///Hecho por naty///
int cantidadDeParametros(op_code instruccion);
void agregarParametros (t_paquete* pInstruc, t_instruccion* instruc, int param);
void agregarParametro1(t_paquete* pInstruc,t_instruccion* instruc);
void agregarParametro2(t_paquete* pInstruc,t_instruccion* instruc);
void agregarParametro3(t_paquete* pInstruc,t_instruccion* instruc);




#endif /* SRC_UTILS_SERIALIZACION_H_ */
