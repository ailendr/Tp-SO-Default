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
	t_fcb* fcb;
	int posicion = posicionFCB(nombreArchivo);
	fcb = list_get(fcbs, posicion);
	if (fcb -> abierto == 0){
		fcb -> abierto = 1;
		log_info(loggerFS, "Operacion: ABRIR (OPEN) -> Archivo: %s", nombreArchivo);
	} else {
		log_info(loggerFS, "ERROR: EL ARCHIVO YA ESTA ABIERTO   Operacion: ABRIR (OPEN) -> Archivo: %s", nombreArchivo);
	}
	free(fcb);
}

void crearArchivo(char* nombreArchivo){

    t_fcb* fcb1;
    fcb1 -> nombreDeArchivo = nombreArchivo;
    fcb1 -> tamanioArchivo = 0;
    fcb1 -> punteroDirecto = NULL;
    fcb1 -> punteroIndirecto = NULL;
    fcb1 -> punteroPosicion = 0;
    fcb1 -> abierto = 1;
    list_add(fcbs, fcb1);

	log_info(loggerFS, "Operacion: CREAR -> Archivo: %s", nombreArchivo);
	free(fcb1);

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
	//verificar existencia
	if(tamanio > fcb -> tamanioArchivo){
		asignarBloques();
	    fcb -> tamanioArchivo = tamanio;
	}else{
	//condicion de reducir (coinciderar que no se reduzca a un valor menor al tamanio del archivo)
		fcb -> tamanioArchivo = tamanio;
	}
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
	t_fcb* fcb;

	int tamanioLista = list_size(fcbs);

	for(int j = 0; j < tamanioLista; j++){
		fcb = list_get(fcbs, j);

		if(strcmp(fcb->nombreDeArchivo, nombre) == 0){
			return j;
		}

	}

	char* path = strcat (pathFCB(), "/");
	path = strcat (path, nombre);

	lector = config_create(path);

	if(lector != NULL ) {
		fcb -> nombreDeArchivo = config_get_string_value(configFS, "NOMBRE_ARCHIVO");
		fcb -> tamanioArchivo = config_get_int_value(configFS, "TAMANIO");

		if (config_has_property(lector, "PUNTERO_DIRECTO")){
			fcb -> punteroDirecto = config_get_int_value(configFS, "PUNTERO_DIRECTO");

			if (config_has_property(lector, "PUNTERO_INDIRECTO")){
				fcb -> punteroIndirecto = config_get_int_value(configFS, "PUNTERO_INDIRECTO");
			} else {
				fcb -> punteroIndirecto = NULL;
			}

		} else {
			fcb -> punteroDirecto = NULL;
			fcb -> punteroIndirecto = NULL;
		}

		fcb -> punteroPosicion = 0;
		fcb -> abierto = 0;
		list_add(fcbs, fcb);

		free(fcb);
		config_destroy(lector);

		return list_size(fcbs) - 1;

	}

	free(fcb);
	config_destroy(lector);

	return -1;

}

void almacenarFcb (t_fcb* fcb){
	t_config* salvador;

	char* path = strcat (pathFCB(), "/");
	path = strcat (path, fcb -> nombreDeArchivo);

	salvador = config_create(path);

	bool verificador = salvador != NULL;

	config_set_value(salvador, "NOMBRE_ARCHIVO", fcb->nombreDeArchivo);
	config_set_value(salvador, "TAMANIO", fcb->tamanioArchivo);

	if (fcb -> punteroDirecto != NULL){
		config_set_value(salvador, "PUNTERO_DIRECTO", fcb -> punteroDirecto);

		if (fcb -> punteroIndirecto != NULL){
			config_set_value(salvador, "PUNTERO_INDIRECTO", fcb -> punteroIndirecto);
		}
	}


	if (verificador){
		config_save(salvador);
	} else {
		config_save_in_file(salvador, path);
	}
	//TODO
	//Verificar si asi esta bien o es al reves
}

int cantidadDeBloquesAsignar(uint32_t tamanio){
	int cantidadDeBloques = 0;
	float cantidadFloat;
	uint32_t tamanioBloque = superBloque -> blockSize;
	cantidadFloat = ceil((float)tamanio / tamanioBloque);

	numSegmento = floor(dirLogica/tamSegmento());
	offset = dirLogica % tamSegmento();

	cantidadDeBloques = (int) cantidadFloat;
	return cantidadDeBloques;
}

void asignarBloques(int cantidadDeBloques){
	//considerar si el archivo ya tiene bloques asignados()
	//verificar que los bloques esten en 0 bitmap
	//agregar en los punteros indirectos los nuevos bloques asignados
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

