/*
 * operaciones.c
 *
 *  Created on: Jul 13, 2023
 *      Author: utnso
 */

#include "operaciones.h"
//TODO Manejar errores

t_list* fcbs;

void cerrarArchivo(char* nombreArchivo){
	t_fcb* fcb;
	int posicion = posicionFCB(nombreArchivo);

	if (posicion == -1 ){
		log_info(loggerFS, "ERROR: NO EXISTE ARCHIVO   Operacion: CERRAR (CLOSE) -> Archivo: %s", nombreArchivo);
	} else {
		fcb = list_get(fcbs, posicion);

		if (fcb -> abierto == 1){
			fcb -> abierto = 0;
			almacenarFcb (fcb);
			log_info(loggerFS, "Operacion: CERRAR (CLOSE) -> Archivo: %s", nombreArchivo);
		} else {
			log_info(loggerFS, "ERROR: EL ARCHIVO NO ESTA ABIERTO   Operacion: CERRAR (CLOSE) -> Archivo: %s", nombreArchivo);
		}

	}

	free(fcb);
}

void abrirArchivo(char* nombreArchivo){
	t_fcb* fcb = malloc(sizeof(t_fcb));
	int posicion = posicionFCB(nombreArchivo);
	fcb = list_get(fcbs, posicion);
	if (fcb -> abierto == 0){
		fcb -> abierto = 1;
		log_info(loggerFS, "Operacion: ABRIR (OPEN) -> Archivo: %s", fcb -> nombreDeArchivo);
	} else {
		log_info(loggerFS, "ERROR: EL ARCHIVO YA ESTA ABIERTO   Operacion: ABRIR (OPEN) -> Archivo: %s", nombreArchivo);
	}
	free(fcb);
}

void crearArchivo(char* nombreArchivo){

    t_fcb* fcb1 = malloc(sizeof(t_fcb));
    fcb1 -> nombreDeArchivo = nombreArchivo;
    fcb1 -> tamanioArchivo = 0;
    fcb1 -> punteroDirecto = -1;
    fcb1 -> punteroIndirecto = -1;
    fcb1 -> punteroPosicion = 0;
    fcb1 -> abierto = 1;
    list_add(fcbs, fcb1);

    almacenarFcb (fcb1);

	log_info(loggerFS, "Operacion: CREAR -> Archivo: %s", nombreArchivo);
	//free(fcb1);

}

void posicionarPuntero (char* nombreArchivo, char* posicion){
	//TODO Transformacion de char a entero
	int posCursor = atoi(posicion);
	t_fcb* fcb;
	int pos = posicionFCB(nombreArchivo);

	if (pos == -1 ){
		log_info(loggerFS, "ERROR: NO EXISTE ARCHIVO   Operacion: POSICIONAR (SEEK) -> Archivo: %s", nombreArchivo);
	} else {
		fcb = list_get(fcbs, pos);

		if (fcb -> abierto == 1){
			fcb -> punteroPosicion = posCursor;
			log_info(loggerFS, "Operacion: POSICIONAR (SEEK) -> Archivo: %s -> Puntero en %i", nombreArchivo, fcb -> punteroPosicion);
		} else {
			log_info(loggerFS, "ERROR: EL ARCHIVO NO ESTA ABIERTO   Operacion: POSICIONAR (SEEK) -> Archivo: %s", nombreArchivo);
		}

	}

	free(fcb);
}

void truncarArchivo (char* nombreArchivo, uint32_t tamanio){
	t_fcb* fcb;
	int posicion = posicionFCB(nombreArchivo);
	fcb = list_get(fcbs, posicion);

	int diferencia = cantBloques(tamanio)-cantBloques(fcb -> tamanioArchivo);

	if(cantBloques(tamanio) > cantBloques(fcb -> tamanioArchivo)){
		agregarBloques(diferencia, fcb);
	    fcb -> tamanioArchivo = tamanio;
	}

	if(cantBloques(tamanio) < cantBloques(fcb -> tamanioArchivo)){
		eliminarBloques((-1)*diferencia, fcb);
	    fcb -> tamanioArchivo = tamanio;
	}

	int offset = tamanio % superBloque->blockSize;

	//TODO
}

void leerArchivo (t_instruccion* instruccion){
	t_fcb* fcb;
	int posicion = posicionFCB(instruccion->param1);

	if (posicion == -1 ){
		log_info(loggerFS, "ERROR: NO EXISTE ARCHIVO   Operacion: LEER (READ) -> Archivo: %s", instruccion->param1);
	} else {
		fcb = list_get(fcbs, posicion);
	}
}

void escribirArchivo (t_instruccion* instruccion){
	t_fcb* fcb;
	int posicion = posicionFCB(instruccion->param1);

	if (posicion == -1 ){
		log_info(loggerFS, "ERROR: NO EXISTE ARCHIVO   Operacion: ESCRIBIR (WRITE) -> Archivo: %s", instruccion->param1);
	} else {
		fcb = list_get(fcbs, posicion);
	}
}

// ___________________ UTILS ___________________
int posicionFCB (char* nombre){
	t_config* lector;
	int posicion = -1;
	t_fcb* fcb = malloc (sizeof(t_fcb));
	char* path = pathFCB();

	int tamanioLista = list_size(fcbs);

	for(int j = 0; j < tamanioLista; j++){
		fcb = list_get(fcbs, j);

		if(strcmp(fcb->nombreDeArchivo, nombre) == 0){
			return j;
		}

	}

	log_info(loggerFS, "PATH GUARDADO: %s", path);

	char* path2 = strcat (path, "/");
	path2 = strcat (path2, nombre);
	log_info(loggerFS, "PATH GUARDADO: %s", path2);

	lector = config_create(path2);

	if(lector != NULL ) {
		strcpy(fcb -> nombreDeArchivo, config_get_string_value(lector, "NOMBRE_ARCHIVO"));
		log_info(loggerFS, "	|-> Nombre Archivo: %s", fcb -> nombreDeArchivo);
		fcb -> tamanioArchivo = config_get_int_value(lector, "TAMANIO");
		log_info(loggerFS, "	|-> Tamanio: %i", fcb -> nombreDeArchivo);

		if (config_has_property(lector, "PUNTERO_DIRECTO")){
			fcb -> punteroDirecto = config_get_int_value(lector, "PUNTERO_DIRECTO");

			if (config_has_property(lector, "PUNTERO_INDIRECTO")){
				fcb -> punteroIndirecto = config_get_int_value(lector, "PUNTERO_INDIRECTO");
			} else {
				fcb -> punteroIndirecto = -1;
			}

		} else {
			fcb -> punteroDirecto = -1;
			fcb -> punteroIndirecto = -1;
		}

		fcb -> punteroPosicion = 0;
		fcb -> abierto = 0;
		list_add(fcbs, fcb);

		free(fcb);
		config_destroy(lector);

		return list_size(fcbs) - 1;
		config_destroy(lector);

	}

	free(fcb);

	return -1;

}

void almacenarFcb (t_fcb* fcb){

	char* path = pathFCB();
	path = strcat (path, "/");
	path = strcat (path, fcb -> nombreDeArchivo);
	log_info(loggerFS, "PATH GUARDADO: %s", path);

	FILE* salvador = fopen(path, "wb+");

	if (salvador != NULL){

		char* infoFCB = "NOMBRE_ARCHIVO:";
		fwrite(infoFCB, 1, strlen(infoFCB), salvador);
		fwrite(fcb -> nombreDeArchivo, 1, strlen(fcb -> nombreDeArchivo), salvador);

		infoFCB = "\nTAMANIO:";
		fwrite(infoFCB, 1, strlen(infoFCB), salvador);
		infoFCB = string_itoa(fcb -> tamanioArchivo);
		fwrite(infoFCB, 1, strlen(infoFCB), salvador);

		if (fcb -> punteroDirecto != -1){
			infoFCB = "\nPUNTERO_DIRECTO:";
			fwrite(infoFCB, 1, strlen(infoFCB), salvador);
			infoFCB = string_itoa(fcb -> punteroDirecto);
			fwrite(infoFCB, 1, strlen(infoFCB), salvador);

			if (fcb -> punteroDirecto != -1){
				infoFCB = "\nPUNTERO_INDIRECTO:";
				fwrite(infoFCB, 1, strlen(infoFCB), salvador);
				infoFCB = string_itoa(fcb -> punteroIndirecto);
				fwrite(infoFCB, 1, strlen(infoFCB), salvador);
			}
		}

	} else {
		log_info(loggerFS, "PATH GUARDADO: %s -> ERROR AL ABRIR ARCHIVO", path);
	}

	fclose(salvador);
}

int cantBloques (uint32_t tamanio){
	int cantidadDeBloques = 0;
	cantidadDeBloques = ceil(tamanio/superBloque->blockSize) + 1;
	//El + 1 es por el bloque que tiene todos los punteros
	return cantidadDeBloques;
}

void eliminarBloques (int cantidadDeBloques, t_fcb* fcb){

}

void agregarBloques (int cantidadDeBloques, t_fcb* fcb){

}

// ___________________ FINALIZACION ___________________
void finalizarListaFcb(){

	t_fcb* fcb;
	int tamanioLista = list_size(fcbs);
	int j = 0;

	for(j; j < tamanioLista; j++){
		fcb = list_get(fcbs, j);

		if(fcb -> abierto == 1){
			cerrarArchivo(fcb -> nombreDeArchivo);
		}

	}

	list_clean(fcbs);
	list_destroy(fcbs);
}

