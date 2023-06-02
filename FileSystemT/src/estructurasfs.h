/*
 * estructurasfs.h
 *
 *  Created on: Jun 2, 2023
 *      Author: utnso
 */

#ifndef SRC_ESTRUCTURASFS_H_
#define SRC_ESTRUCTURASFS_H_

#include <sys/mman.h>
#include "FSConfig.h"
#include <commons/bitarray.h>
#include <stdio.h>
#include <stdlib.h>


typedef enum{
	BITMAP,
	ARCHBLOQUES
}estructura;

typedef struct{
	uint32_t tamBloque;
}t_bloque;

typedef struct{
   uint32_t blockSize;
   uint32_t blockCount;
   //t_bitarray* bitmap; //va ahi el bitmap
}t_superbloque;


typedef struct{
	char* nombreDeArchivo;
	uint32_t tamanioArchivo;
	uint32_t punteroDirecto;  //porque es un numero
	uint32_t punteroIndirecto; //porque es un numero
}t_fcb;


extern t_superbloque* superBloque;
extern t_bitarray* bitMap;


void iniciarEstructuras();
void validarArchivo(char* pathArch, int estructura);
void iniciarSuperBloque();
void iniciarArchivoDeBloques();
void iniciarBitMap();

/*
 *
  #incluye <sys/mman.h>

void *mmap(void * addr , size_t length , int prot , int flags , int fd , off_t offset );

int munmap(void * addr , size_t length );

mmap () crea una nueva asignación en el espacio de direcciones virtuales del
       proceso de llamada. La dirección inicial de la nueva asignación es
       especificado en la dirección . El argumento de longitud especifica la longitud de
       el mapeo (que debe ser mayor que 0).


 */

#endif /* SRC_ESTRUCTURASFS_H_ */
