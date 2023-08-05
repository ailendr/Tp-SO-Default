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
	t_fcb* fcb = cargarFCB(nombreArchivo);

	if ( fcb == NULL ){
		log_info(loggerFS, "ERROR: NO EXISTE ARCHIVO   Operacion: CERRAR (CLOSE) -> Archivo: %s", nombreArchivo);
	} else {
		if (fcb -> abierto == 1){
			fcb -> abierto = 0;
			almacenarFcb (fcb);
			log_info(loggerFS, "Operacion: CERRAR (CLOSE) -> Archivo: %s", nombreArchivo);
		} else {
			log_info(loggerFS, "ERROR: EL ARCHIVO NO ESTA ABIERTO   Operacion: CERRAR (CLOSE) -> Archivo: %s", nombreArchivo);
		}

	}

}

void abrirArchivo(t_fcb* fcb ){
	if (fcb -> abierto == 0){
		fcb -> abierto = 1;
		log_info(loggerFS, "Operacion: ABRIR (OPEN) -> Archivo: %s", fcb->nombreDeArchivo);
	} else {
		log_info(loggerFS, "ERROR: EL ARCHIVO YA ESTA ABIERTO   Operacion: ABRIR (OPEN) -> Archivo: %s", fcb->nombreDeArchivo);
	}
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

}

void posicionarPuntero (char* nombreArchivo, char* posicion){
	//Transformacion de char a entero
	int posCursor = atoi(posicion);
	t_fcb* fcb = cargarFCB(nombreArchivo);

	if ( fcb == NULL ){
		log_info(loggerFS, "ERROR: NO EXISTE ARCHIVO   Operacion: POSICIONAR (SEEK) -> Archivo: %s", nombreArchivo);
	} else {

		if (fcb -> abierto == 1){
			fcb -> punteroPosicion = posCursor;
			log_info(loggerFS, "Operacion: POSICIONAR (SEEK) -> Archivo: %s -> Puntero en %i", nombreArchivo, fcb -> punteroPosicion);
		} else {
			log_info(loggerFS, "ERROR: EL ARCHIVO NO ESTA ABIERTO   Operacion: POSICIONAR (SEEK) -> Archivo: %s", nombreArchivo);
		}

	}

}

int truncarArchivo (char* nombreArchivo, uint32_t tamanio){
	t_fcb* fcb = cargarFCB(nombreArchivo);

	int diferencia = cantBloques(tamanio) - cantBloques(fcb -> tamanioArchivo);

	if(cantBloques(tamanio) > cantBloques(fcb -> tamanioArchivo)){
		aumentar(diferencia, fcb);
	}

	if(cantBloques(tamanio) < cantBloques(fcb -> tamanioArchivo)){
		eliminarBloques(diferencia, fcb);
	}

	log_info(loggerFS, "Operacion: TRUNCAR (TRUNCATE) -> Archivo: %s", fcb -> nombreDeArchivo);
	log_info(loggerFS, "	|-> Tamanio Viejo: %d", fcb -> tamanioArchivo);
	log_info(loggerFS, "	|-> Tamanio Nuevo: %d", tamanio);
    fcb -> tamanioArchivo = tamanio;

    almacenarFcb (fcb);
    guardarBitMap();

	imprimir_bitmap_20(bitMap);

    return 0;

}

void leerArchivo (t_instruccion* instruccion, void* bufferLectura, int bytesALeer){
	t_fcb* fcb = cargarFCB(instruccion->param1);

	if (fcb == NULL){
		log_info(loggerFS, "ERROR: NO EXISTE ARCHIVO   Operacion: LEER (READ) -> Archivo: %s", instruccion->param1);
	} else {
		escribirYLeerArchivo(bufferLectura, bytesALeer, fcb, F_READ);
	}
}

void escribirArchivo (t_instruccion* instruccion, void* bufferEscritura, int bytesAEscribir){
	t_fcb* fcb = cargarFCB(instruccion->param1);

	if (fcb == NULL){
		log_info(loggerFS, "ERROR: NO EXISTE ARCHIVO   Operacion: ESCRIBIR (WRITE) -> Archivo: %s", instruccion->param1);
	} else {

		escribirYLeerArchivo(bufferEscritura, bytesAEscribir, fcb, F_WRITE);
		int valorOp=OK;
		send(cliente, &valorOp, sizeof(int), 0);
		free(bufferEscritura);

	}
}

void escribirYLeerArchivo(void* buffer, int bytes, t_fcb* fcb, op_code operacion){
	int tamanioRecorrido=0;
	uint32_t posPtro = fcb->punteroPosicion;

	while(tamanioRecorrido < bytes){
		int tamMax = (superBloque->blockSize)- (posPtro % superBloque->blockSize);
		int tamAEscribir_Leer = min(bytes - tamanioRecorrido, tamMax);
		int bloque_logico = bloqueSegunPuntero(posPtro);
		int bloque_fisico = bloqueLogicoAFisico(fcb, bloque_logico);
		int offset = offsetSegunPuntero(posPtro);
		int posicion = posicionArchivoBloques(bloque_fisico, offset);
		if(operacion == F_WRITE){
			escribirArchivoBloques(buffer + tamanioRecorrido, posicion, tamAEscribir_Leer);
		}
		else if (operacion == F_READ){
			leerArchivoBloques(buffer + tamanioRecorrido, posicion, tamAEscribir_Leer);
		}

		usleep(retardoNumerico() * 1000);
		log_info(loggerFS, "Acceso Bloque - Archivo:%s - Bloque Archivo:%d - Bloque File System: %d",fcb->nombreDeArchivo, bloque_logico, bloque_fisico);

		tamanioRecorrido += tamAEscribir_Leer;
		posPtro += tamAEscribir_Leer;
	}
}

// ___________________ UTILS ___________________
t_fcb* cargarFCB (char* nombre){
	t_fcb* fcb = buscarFCB(nombre);

	if (fcb != NULL) return fcb;

	t_config* lector;
	char* path = string_new();
	string_append(&path, pathFCB());
	string_append(&path, "/");
	string_append(&path, nombre);

	lector = config_create(path);

	if(lector != NULL ) {

		fcb = malloc (sizeof(t_fcb));
		fcb -> nombreDeArchivo = config_get_string_value(lector, "NOMBRE_ARCHIVO");
		fcb -> tamanioArchivo = config_get_int_value(lector, "TAMANIO");
		fcb -> punteroDirecto = config_get_int_value(lector, "PUNTERO_DIRECTO");
		fcb -> punteroIndirecto = config_get_int_value(lector, "PUNTERO_INDIRECTO");
		fcb -> punteroPosicion = 0;
		fcb -> abierto = 0;
		list_add(fcbs, fcb);

		log_info(loggerFS, "ARCHIVO ENCONTRADO EN LA POS %i", list_size(fcbs) - 1);
		log_info(loggerFS, "	|-> Nombre Archivo: %s", fcb -> nombreDeArchivo);
		log_info(loggerFS, "	|-> Tamanio: %i", fcb -> tamanioArchivo);

		//config_destroy(lector);

	}

	return fcb;

}

t_fcb* buscarFCB(char* archivo){
	for(int i=0;i<list_size(fcbs);i++){
		t_fcb* fcb = list_get(fcbs, i);
		if(string_equals_ignore_case(fcb->nombreDeArchivo, archivo)){
			return fcb;
		}
	}
	return NULL;
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

		infoFCB = "\nPUNTERO_DIRECTO=";
		fwrite(infoFCB, 1, strlen(infoFCB), salvador);
		infoFCB = string_itoa(fcb -> punteroDirecto);
		fwrite(infoFCB, 1, strlen(infoFCB), salvador);

		infoFCB = "\nPUNTERO_INDIRECTO=";
		fwrite(infoFCB, 1, strlen(infoFCB), salvador);
		infoFCB = string_itoa(fcb -> punteroIndirecto);
		fwrite(infoFCB, 1, strlen(infoFCB), salvador);


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












