/*
 * estructurasfs.c
 *
 *  Created on: Jun 1, 2023
 *      Author: utnso
 */

#include "estructurasfs.h"

t_superbloque* superBloque;
t_bitarray* bitMap;


void iniciarEstructuras(){
	char* archBloques = pathBloques();
	char* archBitmap = pathBitmap();

	iniciarSuperBloque();
	validarArchivo(archBloques, ARCHBLOQUES);
	validarArchivo(archBitmap, BITMAP);

}

void validarArchivo(char* pathArch, int estructura){
	FILE* archivo = fopen(pathArch, "r");
	if(archivo==NULL){
		log_info(loggerFS, "El archivo no se pudo abrir");
		exit(1);
	}
	char palabra=fgetc(archivo);
	 if(palabra==EOF){
		 switch(estructura){
		 case BITMAP:
			 iniciarBitMap();
			 break;

		 case ARCHBLOQUES:
			 iniciarArchivoDeBloques();
			 break;
		 default:
			 break;
		 }
	 }
	 else{
		 //conciderar si ya existe el archivo
		 log_info(loggerFS,"El archivo ya existe");
		 //

		 //y si el archivo es corrupto cerrar o no permitir el ingreso
		 //LO DEJAMOS PARA QUE LO HAGAN
	 }

}

//////////////////////////////INICIALIZACION ESTRUCTURAS////////////////////////////////////////

void iniciarSuperBloque(){
	t_config * rutaSuperBloque= config_create(pathSuperBloque());
	int tamBloque =  config_get_int_value(rutaSuperBloque, "BLOCK_SIZE");
	int cantBloques =  config_get_int_value(rutaSuperBloque, "BLOCK_COUNT");
	log_info(loggerFS, "El tamaño de bloque es: %d",tamBloque);
	log_info(loggerFS, "La cantidad de bloques es: %d", cantBloques);
	superBloque->blockSize=tamBloque;
	superBloque->blockCount=cantBloques;
}

void iniciarArchivoDeBloques(){
	t_bloque* bloque;
	bloque -> tamBloque = superBloque->blockSize; // * superBloque->blockCount;
	//tamanioDelArchivo = superBloque->blockSize * superBloque->blockCount;
	t_bloque* arrayBloques[superBloque->blockCount];//REVISAAAAAAAAAAR
    //se agrega el tamaño de la multipliucacion del blocksize y block count
}

void iniciarBitMap(){
	size_t bytes=superBloque->blockCount/8;
	void* bitArray=malloc(bytes);
	bitMap=bitarray_create_with_mode(bitArray, bytes, LSB_FIRST);
	char * mapeo = mmap(NULL, bytes, PROT_READ | PROT_WRITE, MAP_PRIVATE,0,0);

}
