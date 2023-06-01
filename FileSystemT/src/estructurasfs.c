/*
 * estructurasfs.c
 *
 *  Created on: Jun 1, 2023
 *      Author: utnso
 */

#include <commons/bitarray.h>
#include "FileSystem.h"

typedef struct{
   uint32_t blockSize;
   uint32_t blockCount;
   t_bitarray* bitmap; //va ahi el bitmap
}t_superbloque;


typedef struct{
	char* nombreDeArchivo;
	uint32_t tamanioArchivo;
	uint32_t punteroDirecto;  //porque es un numero
	uint32_t punteroIndirecto; //porque es un numero
}t_fcb;


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

