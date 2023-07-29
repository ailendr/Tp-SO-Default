/*
 * operaciones.h
 *
 *  Created on: Jul 13, 2023
 *      Author: utnso
 */

#ifndef SRC_OPERACIONES_H_
#define SRC_OPERACIONES_H_

#include "bloques.h"

extern t_list* fcbs;

void cerrarArchivo(char* nombreArchivo);
void abrirArchivo(char* nombreArchivo);
void crearArchivo(char* nombreArchivo);
void posicionarPuntero (char* nombreArchivo, char* posicion);
void truncarArchivo (char* nombreArchivo, uint32_t tamanio);
void leerArchivo(t_instruccion* instruccion);
void escribirArchivo (t_instruccion* instruccion);

int posicionFCB (char* nombre);
void almacenarFcb (t_fcb* fcb);

void finalizarListaFcb();

#endif /* SRC_OPERACIONES_H_ */
