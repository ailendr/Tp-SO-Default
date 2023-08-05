/*
 * bloques.c
 *
 *  Created on: Jun 15, 2023
 *      Author: utnso
 */

#include "bloques.h"

int cantBloques (uint32_t tamanio){
	int cantidadDeBloques = 0;
	cantidadDeBloques = ceil(tamanio/superBloque->blockSize);
	if (cantidadDeBloques > 1) cantidadDeBloques++;
	//El + 1 es por el bloque que tiene todos los punteros
	return cantidadDeBloques;
}

void aumentar (int cantidadDeBloques, t_fcb* fcb){

	if (fcb->punteroDirecto == -1 && cantidadDeBloques > 0){
		cantidadDeBloques --;
		fcb->punteroDirecto = asignarBloque();
		log_info(loggerFS, "	-> Se cargo un nuevo bloque con puntero directo para el archivo %s", fcb -> nombreDeArchivo);
		log_info(loggerFS, "	-> BLOQUE DIRECTO %i", fcb -> punteroDirecto);
	}

	if (fcb->punteroIndirecto == -1 && cantidadDeBloques > 0){
		cantidadDeBloques --;
		fcb->punteroIndirecto = asignarBloque();
		log_info(loggerFS, "	-> Se cargo un nuevo bloque con puntero directo para el archivo %s", fcb -> nombreDeArchivo);
		log_info(loggerFS, "	-> BLOQUE INDIRECTO %i", fcb -> punteroIndirecto);

	}

	int objetivo = cantBloques(fcb -> tamanioArchivo) + cantidadDeBloques;
	int asignados = cantBloques(fcb -> tamanioArchivo);

	for(int i=0; i < objetivo; i++){
		agregar_bloque(fcb, &asignados);
	}
	usleep(retardoNumerico()*1000);
	log_info(loggerFS, "Acceso Bloque - Archivo: %s - Bloque Archivo:%s - Bloque File System %d",fcb->nombreDeArchivo, "puntero", fcb->punteroIndirecto);

}

int proxBloqueVacio(){
	for (int i = 0; i < bitMap->size*8; i++){
		bool bit = bitarray_test_bit(bitMap, i);
		log_info(loggerFS, "Accediendo al bit %i:         %s", i,string_itoa(bit));
		if (bit == 0) return i;
	}
	log_warning(loggerFS, "NO HAY BLOQUES DISPONIBLES");
	return -1;
}

uint32_t asignarBloque(){
	uint32_t bloque_a_asignar;
	bloque_a_asignar = proxBloqueVacio();
	bitarray_set_bit(bitMap, bloque_a_asignar);
	//printf("\nAsignado------\n");
	log_info(loggerFS, "Acceso a Bitmap - Bloque: %d - Estado: %d", bloque_a_asignar, bitarray_test_bit(bitMap, bloque_a_asignar));
	//printf("-------------------\n");
	guardarBitMap();
	return bloque_a_asignar;
}

void agregar_bloque(t_fcb* fcb, int* asignados){
	uint32_t bloque = asignarBloque();
	int posicion = fcb->punteroIndirecto * superBloque->blockSize + ((*asignados)-1)*sizeof(uint32_t);
	escribirArchivoBloques((void*)&bloque, posicion, sizeof(uint32_t));
	(*asignados)++;
}

void escribirBloque (void* contenido, uint32_t sizeContenido, uint32_t numeroBloque){
    int offset = 0;
    if (sizeContenido > superBloque -> blockSize){
        log_warning(loggerFS, "No es posible escribir %d bytes al bloque %d", sizeContenido, numeroBloque);
    }

    //chequear que este libre el bloque (bitMap)bitarray_set_bit
    memcpy(superBloque -> blockSize + offset, contenido, sizeContenido);

    log_info(loggerFS, "Se escribio un bloque");
}

void agregarContenidoABloque (void* contenido, uint32_t sizeContenido, uint32_t offset , uint32_t numeroBloque){
    if (sizeContenido + offset > superBloque -> blockSize){
        log_warning(loggerFS, "Se quiso agregar %d bytes al bloque %d en el offset %d", sizeContenido,numeroBloque, offset);
    }

   // memcpy(superBloque -> blockSize + offset,contenido, sizeContenido); //para copiar tiene que ser un void* como es superBloque rompe


}

void liberarBloque(uint32_t numeroBloque){
	bitmapRecuperado ();
	bitarray_clean_bit(bitMap, numeroBloque);
	guardarBitMap(bitMap->bitarray);
	log_info(loggerFS, "Se libero el bloque numero %i", numeroBloque);
	bitarray_destroy(bitMap);
}

void leerArchivoBloques(void* aLeer, int posicion, int cantidad){
	FILE* archivo_bloques = fopen(pathBloques(), "r+b");
	fseek(archivo_bloques, posicion, SEEK_SET);
	fread(aLeer, sizeof(char), cantidad+1, archivo_bloques);
	fclose(archivo_bloques);
}

void escribirArchivoBloques(void* aEscribir, int posicion, int cantidad){
	log_trace(loggerFS, "escribiendo archivo pos: %d , cant: %d", posicion, cantidad);
	FILE* archivo_bloques = fopen(pathBloques(), "r+b");
	fseek(archivo_bloques, posicion, SEEK_SET);
	fwrite(aEscribir, cantidad,1, archivo_bloques);
	fclose(archivo_bloques);
}
void eliminarBloques (int cantidadDeBloques, t_fcb* fcb){

}

