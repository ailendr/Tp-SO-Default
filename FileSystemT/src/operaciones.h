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
void abrirArchivo(t_fcb* fcb );
void crearArchivo(char* nombreArchivo);
void posicionarPuntero (char* nombreArchivo, char* posicion);
int truncarArchivo (char* nombreArchivo, uint32_t tamanio);

void leerArchivo (t_instruccion* instruccion, void* bufferLectura, int bytesALeer);
int offsetSegunPuntero(int puntero);
int bloqueSegunPuntero(int puntero);
int posicionArchivoBloques(int num_bloque, int offset);
int bloqueLogicoAFisico(t_fcb* fcb, int num_bloque);
int min(int num1, int num2);
void escribirArchivo (t_instruccion* instruccion, void* bufferEscritura, int bytesAEscribir);
void escribirYLeerArchivo(void* buffer, int bytes, t_fcb* fcb, op_code operacion);

t_fcb* cargarFCB (char* nombre);
t_fcb* buscarFCB(char* archivo);
void almacenarFcb (t_fcb* fcb);

#endif /* SRC_OPERACIONES_H_ */
