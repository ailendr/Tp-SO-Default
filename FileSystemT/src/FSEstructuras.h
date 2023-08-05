/*
 * estructurasfs.h
 *
 *  Created on: Jun 2, 2023
 *      Author: utnso
 */

#ifndef SRC_FSESTRUCTURAS_H_
#define SRC_FSESTRUCTURAS_H_

#include <sys/mman.h>
#include <commons/bitarray.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <commons/collections/queue.h>

//#include "FSUtils.h"
#include "FSConfig.h"

/*
typedef enum{
	BITMAP,
	ARCHBLOQUES
}estructura;
*/

typedef struct{
	uint32_t numBloque;
	uint32_t tamBloque;
	char* datosBloque;
}t_bloque;

typedef struct{
   uint32_t blockSize;
   uint32_t blockCount;
}t_superbloque;


typedef struct{
	char* nombreDeArchivo;
	uint32_t tamanioArchivo;
	uint32_t punteroDirecto;
	uint32_t punteroIndirecto;
	uint32_t punteroPosicion;
	uint32_t abierto;
}t_fcb;


extern t_superbloque* superBloque;
extern t_bitarray* bitMap;
extern t_list* peticiones;


void iniciarEstructuras();
void iniciarSuperBloque();
void iniciarArchivoDeBloques();
void iniciarBitMap();
void guardarBitMap();
void bitmapRecuperado ();
void imprimir_bitmap_20();


#endif /* SRC_FSESTRUCTURAS_H_ */
