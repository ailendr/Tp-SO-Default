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
	bitMap = bitarray_create_with_mode(bitArray, bytes, LSB_FIRST);

	FILE* preGuardado = fopen(pathBitmap(), "rb+");

	if (preGuardado != NULL){

		log_info(loggerFS, "Recuperando los datos");
		fread(bitMap->bitarray, 1, bytes, preGuardado);
		fclose(preGuardado);

	} else {
		log_info(loggerFS, "Creando el archivo");
		guardarBitMap(string_repeat('0', superBloque->blockCount));
		log_info(loggerFS, "%s", bitMap->bitarray);
	}
	log_info(loggerFS, "Ok -> Creacion BitMap");

}

void guardarBitMap(char* bitArray){
	FILE* preGuardado = fopen(pathBitmap(), "wb+");
	int bytes = superBloque->blockCount/8;
	strcpy(bitMap->bitarray, bitArray);
	fwrite(bitMap->bitarray, 1, bytes, preGuardado);
	fclose(preGuardado);
}

void iniciarArchivoDeBloques(){
	FILE* archivo_bloques = fopen(pathBloques(), "r+");
   	int tamanio_archivo = superBloque -> blockCount * superBloque -> blockSize;

    if (archivo_bloques == NULL) {
        // El archivo no existe, se crea y se trunca al tamaño deseado

    	archivo_bloques = fopen(pathBloques(), "wb+");
        if (archivo_bloques == NULL) {
            log_trace(fileSystem_logger, "No se pudo crear Archivo de Bloques");
            return;
        }
        fseek(archivo_bloques, tamanio_archivo - 1, SEEK_SET);
        fputc('\0', archivo_bloques);

        log_trace(fileSystem_logger, "Archivo de Bloques creado");
        log_trace(fileSystem_logger, "Tamaño: %d", tamanio_archivo);
        //fclose(archivo_bloques);
    } else {

    	log_trace(fileSystem_logger, "Archivo de Bloques leido");
    	log_trace(fileSystem_logger, "Tamaño: %d", tamanio_archivo);
        /*// El archivo existe, se verifica el tamaño actual
        fseek(archivo_bloques, tamanio_archivo - 1, SEEK_END);
        if (ftell(archivo_bloques) == tamanio_archivo) {
   	log_trace(fileSystem_logger, "Archivo de Bloques leido");
        	log_trace(fileSystem_logger, "Tamaño: %d", tamanio_archivo);
        } else {
        	log_error(fileSystem_logger, "Archivo de Bloques no tiene el tamaño deseado.");
        	return;
        }*/
    }
    fclose(archivo_bloques);
}

