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
	//validarArchivo(archBloques, ARCHBLOQUES);
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

void iniciarArchivoDeBloques(char* pathArch){

	/*int cantidadBloques = superBloque -> blockCount;
	int i = 0;

	for(i; i<cantidadBloques; i++){

	}*/
	//--------------------------------------------------------------------------------------------------------
	//t_bloque* arrayBloques[cantidadBloques];

	//t_bloque* arrayBloques[cantidadBloques];
	//log_info(loggerFS, "");
	//t_bloque* bloque;
	//arrayBloques[cantidadBloques] -> tamBloque = superBloque->blockSize * superBloque->blockCount;
	//tamanioDelArchivo = superBloque->blockSize * superBloque->blockCount;
	//t_bloque* arrayBloques[superBloque->blockCount];


}

