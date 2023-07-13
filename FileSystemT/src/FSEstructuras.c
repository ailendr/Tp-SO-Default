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

static t_superbloque* crearSuperbloque(uint32_t block_size, uint32_t block_count){
	t_superbloque* sb;
    sb -> blockSize = block_size;
    sb -> blockCount = block_count;
    return sb;
}


void iniciarEstructuras(){
	char* archBloques = pathBloques();
	char* archBitmap = pathBitmap();

	iniciarSuperBloque();
	validarArchivo(archBloques, ARCHBLOQUES);
	validarArchivo(archBitmap, BITMAP);

}

void validarArchivo(char* pathArch, int estructura){
	FILE* archivo = fopen(pathArch, "r"); // "a" para escribir al final si exite
	if(archivo==NULL){
		log_info(loggerFS, "El archivo no se pudo abrir");
		exit(1);
	}
	// se mueve al final del archivo
	fseek(archivo, 0, SEEK_END);
	int posicion = ftell(archivo);
	//char palabra=fgetc(archivo);
	 if(posicion == 0){
		 log_info(loggerFS, "El archivo esta vacio");
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

		 log_info(loggerFS,"El archivo no esta vacio");
		 //recuperar el contenido del archivo
		 return;
	 }
	 fclose(archivo);

}

//////////////////////////////INICIALIZACION ESTRUCTURAS////////////////////////////////////////

void iniciarSuperBloque(){
	t_config * rutaSuperBloque= config_create(pathSuperBloque());
	int tamBloque =  config_get_int_value(rutaSuperBloque, "BLOCK_SIZE");
	int cantBloques =  config_get_int_value(rutaSuperBloque, "BLOCK_COUNT");
	log_info(loggerFS, "El tamaño de bloque es: %d",tamBloque);
	log_info(loggerFS, "La cantidad de bloques es: %d", cantBloques);
	//superBloque -> blockSize = tamBloque;
	//superBloque->blockCount = cantBloques;
	crearSuperbloque(tamBloque, cantBloques);
}

void iniciarArchivoDeBloques(char* pathArch){

	int cantidadBloques = superBloque -> blockCount;
	int i = 0;

	for(i; i<cantidadBloques; i++){

	}



	//--------------------------------------------------------------------------------------------------------
	t_bloque* arrayBloques[cantidadBloques];

	//t_bloque* arrayBloques[cantidadBloques];
	//log_info(loggerFS, "");
	//t_bloque* bloque;
	//arrayBloques[cantidadBloques] -> tamBloque = superBloque->blockSize * superBloque->blockCount;
	//tamanioDelArchivo = superBloque->blockSize * superBloque->blockCount;
	//t_bloque* arrayBloques[superBloque->blockCount];
}

void iniciarBitMap(){
	size_t bytes=superBloque->blockCount/8;
	void* bitArray=malloc(bytes);
	bitMap=bitarray_create_with_mode(bitArray, bytes, LSB_FIRST);
	//uint32_t* mapeo = mmap(NULL, bytes, PROT_READ | PROT_WRITE, MAP_SHARED,/*descriptor*/,0);

}
