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

}

void abrirArchivo(char* nombreArchivo){
	t_fcb* fcb;
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

int truncarArchivo (char* nombreArchivo, uint32_t tamanio){
	t_fcb* fcb;
	int posicion = posicionFCB(nombreArchivo);
	fcb = list_get(fcbs, posicion);

	int diferencia = cantBloques(tamanio)-cantBloques(fcb -> tamanioArchivo);

	if(cantBloques(tamanio) > cantBloques(fcb -> tamanioArchivo)){
		agregarBloques(diferencia, fcb);
	}

	if(cantBloques(tamanio) < cantBloques(fcb -> tamanioArchivo)){
		eliminarBloques(diferencia, fcb);
	}

	log_info(loggerFS, "Operacion: TRUNCAR (TRUNCATE) -> Archivo: %s", fcb -> nombreDeArchivo);
	log_info(loggerFS, "	|-> Tamanio Viejo: %d", fcb -> tamanioArchivo);
	log_info(loggerFS, "	|-> Tamanio Nuevo: %d", tamanio);
    fcb -> tamanioArchivo = tamanio;

    return 0;

}

void leerArchivo (t_instruccion* instruccion, void* bufferLectura, int bytesALeer){
	t_fcb* fcb;
	int posicion = posicionFCB(instruccion->param1);

	if (posicion == -1 ){
		log_info(loggerFS, "ERROR: NO EXISTE ARCHIVO   Operacion: LEER (READ) -> Archivo: %s", instruccion->param1);
	} else {
		fcb = list_get(fcbs, posicion);
		uint32_t posPtro = fcb->punteroPosicion;
		//FILE* archivo_bloques = fopen(pathBloques(), "r+");
		//fseek(archivo_bloques, posPtro,SEEK_SET);
		//fread(archivo_bloques, si)
		int tam_leido = 0;
			//log_trace(fileSystem_logger, "leyendo archivo: %s", archivo);
			while(tam_leido < bytesALeer){
				int tam_a_leer_max = (superBloque->blockSize ) - (posPtro % superBloque->blockSize);
				int tam_a_leer = min((bytesALeer-tam_leido), tam_a_leer_max);
				//t_fcb* fcb = fcb_segun_nombre();
				int bloque_logico = bloqueSegunPuntero(posPtro);
				int bloque_fisico = bloqueLogicoAFisico(fcb, bloque_logico);
				int offset = offsetSegunPuntero(posPtro);
				int posicion = posicionArchivoBloques(bloque_fisico, offset);
				leerArchivoBloques(bufferLectura + tam_leido, posicion, tam_a_leer);

				usleep(retardoNumerico()*1000);
				log_info(loggerFS, "Acceso Bloque - Archivo:%s - Bloque Archivo:%d - Bloque File System: %d",fcb->nombreDeArchivo , bloque_logico, bloque_fisico);

				tam_leido += tam_a_leer;
				posPtro += tam_a_leer;
			}

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
	t_fcb* fcb2;

	int tamanioLista = list_size(fcbs);

	for(int j = 0; j < tamanioLista; j++){
		fcb2 = list_get(fcbs, j);

		if(strcmp(fcb2->nombreDeArchivo, nombre) == 0){

			log_info(loggerFS, "ARCHIVO ENCONTRADO EN LA POS %i", j);
			log_info(loggerFS, "	|-> Nombre Archivo: %s", fcb2 -> nombreDeArchivo);
			log_info(loggerFS, "	|-> Tamanio: %d", fcb2 -> tamanioArchivo);
			return j;
		}

	}

	t_fcb* fcb = malloc (sizeof(t_fcb));

	t_config* lector;
	char* path = string_new();
	string_append(&path, pathFCB());
	string_append(&path, "/");
	string_append(&path, nombre);

	lector = config_create(path);

	if(lector != NULL ) {
		fcb -> nombreDeArchivo = config_get_string_value(lector, "NOMBRE_ARCHIVO");
		fcb -> tamanioArchivo = config_get_int_value(lector, "TAMANIO");

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

		log_info(loggerFS, "ARCHIVO ENCONTRADO EN LA POS %i", list_size(fcbs) - 1);
		log_info(loggerFS, "	|-> Nombre Archivo: %s", fcb -> nombreDeArchivo);
		log_info(loggerFS, "	|-> Tamanio: %i", fcb -> tamanioArchivo);

		free(fcb);
		config_destroy(lector);

		return list_size(fcbs) - 1;

	}

	free(fcb);

	return -1;

}

void almacenarFcb (t_fcb* fcb){

	char* path = string_new();
	string_append(&path, pathFCB());
	string_append(&path, "/");
	string_append(&path, fcb -> nombreDeArchivo);
	FILE* salvador = fopen(path, "wb+");

	if (salvador != NULL){

		char* infoFCB = "NOMBRE_ARCHIVO=";
		fwrite(infoFCB, 1, strlen(infoFCB), salvador);
		fwrite(fcb -> nombreDeArchivo, 1, strlen(fcb -> nombreDeArchivo), salvador);

		infoFCB = "\nTAMANIO=";
		fwrite(infoFCB, 1, strlen(infoFCB), salvador);
		infoFCB = string_itoa(fcb -> tamanioArchivo);
		fwrite(infoFCB, 1, strlen(infoFCB), salvador);

		if (fcb -> punteroDirecto != -1){
			infoFCB = "\nPUNTERO_DIRECTO=";
			fwrite(infoFCB, 1, strlen(infoFCB), salvador);
			infoFCB = string_itoa(fcb -> punteroDirecto);
			fwrite(infoFCB, 1, strlen(infoFCB), salvador);

			if (fcb -> punteroDirecto != -1){
				infoFCB = "\nPUNTERO_INDIRECTO=";
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

///----------UTILS OPERACIONES-------------///
//FUNCIONES PA LEER//

int min(int num1, int num2){
 return num1<num2? num1 : num2;
}

int posicionArchivoBloques(int num_bloque, int offset){
	return num_bloque*superBloque->blockSize + offset;
}
int bloqueSegunPuntero(int puntero){
	return puntero/superBloque->blockSize;
}

int offsetSegunPuntero(int puntero){
	return puntero % superBloque->blockSize;
}
int bloqueLogicoAFisico(t_fcb* fcb, int num_bloque){
	int bloque_fisico;
	if(num_bloque == 0){
		return fcb->punteroDirecto;
	}else{
		int posicion = fcb->punteroIndirecto * (superBloque->blockSize) + (num_bloque - 1)*sizeof(uint32_t);
		leerArchivoBloques((void*)&bloque_fisico, posicion, sizeof(uint32_t));
		return bloque_fisico;
	}
}












