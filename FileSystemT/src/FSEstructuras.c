/*
 * estructurasfs.c
 *
 *  Created on: Jun 1, 2023
 *      Author: utnso
 */

#include "FSEstructuras.h"

t_superbloque* superBloque;
t_bitarray* bitMap;
t_list* peticiones;


void iniciarEstructuras(){
	iniciarSuperBloque();
	iniciarBitMap();
	iniciarArchivoDeBloques();
}

//////////////////////////////INICIALIZACION ESTRUCTURAS////////////////////////////////////////

void iniciarSuperBloque(){
	log_info(loggerFS, "Creando SuperBloque ....");
	superBloque = malloc(sizeof(t_superbloque));
	t_config * rutaSuperBloque= config_create(pathSuperBloque());
	int tamBloque =  config_get_int_value(rutaSuperBloque, "BLOCK_SIZE");
	int cantBloques =  config_get_int_value(rutaSuperBloque, "BLOCK_COUNT");
	log_info(loggerFS, "El tamaño de bloque es: %d",tamBloque);
	log_info(loggerFS, "La cantidad de bloques es: %d", cantBloques);
	superBloque -> blockSize = tamBloque;
	superBloque -> blockCount = cantBloques;
	config_destroy(rutaSuperBloque);
	log_info(loggerFS, "Ok -> Creacion Superbloque");
}

void iniciarBitMap(){
	log_info(loggerFS, "Bajando Bitmap ....");
	int bytes = superBloque->blockCount/8;
	char* bitArray = malloc(bytes);
	memset(bitArray, 0, bytes);
	bitMap = bitarray_create_with_mode(bitArray, bytes, MSB_FIRST);
	FILE* preGuardado = fopen(pathBitmap(), "rb+");

	if (preGuardado == NULL){

		log_info(loggerFS, "Creando el Bitmap ...");
		guardarBitMap();
		log_info(loggerFS, "Se creo correctamente");

	} else {

		log_info(loggerFS, "Recuperando los datos...");
		bitmapRecuperado (); //Para saber que no tira error aca
		log_info(loggerFS, "Se recupero correctamente");
		fclose(preGuardado); //Va aca porque si es nulo no tengo q cerrarlo

	}
	imprimir_bitmap_20(bitMap);
	log_info(loggerFS, "BitMap -> OK");

}

void guardarBitMap(){
	FILE* preGuardado = fopen(pathBitmap(), "wb+");
	fwrite(bitMap->bitarray, 1, bitMap->size, preGuardado);
	fclose(preGuardado);
}

void bitmapRecuperado (){
	FILE* preGuardado = fopen(pathBitmap(), "rb+");
	fread(bitMap->bitarray, 1, bitMap->size, preGuardado);
	fclose(preGuardado);
}

void iniciarArchivoDeBloques(){
	FILE* archivo_bloques = fopen(pathBloques(), "r+");
   	int tamanio_archivo = superBloque -> blockCount * superBloque -> blockSize;

    if (archivo_bloques == NULL) {
        // El archivo no existe, se crea y se trunca al tamaño deseado

    	archivo_bloques = fopen(pathBloques(), "wb+");

        if (archivo_bloques == NULL) {
        	log_info(loggerFS, "No se pudo crear Archivo de Bloques");
            return;
        }

        fseek(archivo_bloques, tamanio_archivo - 1, SEEK_SET);
        fputc('\0', archivo_bloques);

        log_info(loggerFS, "Archivo de Bloques Creado");
        log_info(loggerFS, "Tamaño: %d", tamanio_archivo);

    } else {

    	log_info(loggerFS, "Archivo de Bloques leido");
    	log_info(loggerFS, "Tamaño: %d", tamanio_archivo);

    }

    fclose(archivo_bloques);
}

void imprimir_bitmap_20(){
	printf("\n\nimprimiendo bitmap: \n[");
	printf("bitmap->size: %ld\n", bitMap->size);
	for (int i = 0; i < 20; i++) {
	    bitarray_clean_bit(bitMap, i);
	    printf("%d ", bitarray_test_bit(bitMap, i));
	}
	printf("]\n\n");
}

