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

void eliminarBloques (int cantidadDeBloques, t_fcb* fcb){

}

void agregarBloques (int cantidadDeBloques, t_fcb* fcb){

	int proxCargar;

	if (fcb->punteroDirecto == -1 && cantidadDeBloques > 0){
		cantidadDeBloques --;
		proxCargar = proxBloqueVacio();
		bitarray_set_bit(bitMap, proxCargar);
		fcb->punteroDirecto = proxCargar;
	}

	if (fcb->punteroIndirecto == -1 && cantidadDeBloques > 0){
		cantidadDeBloques --;
		proxCargar = proxBloqueVacio();
		bitarray_set_bit(bitMap, proxCargar);
		fcb->punteroIndirecto = proxCargar;
	}

	for (int i = 0; i < cantidadDeBloques; i++){
		proxCargar = proxBloqueVacio();
		bitarray_set_bit(bitMap, proxCargar);
		//Guardar en el archivo de bloques
	}

	guardarBitMap(bitMap->bitarray);
	log_info(loggerFS, "%s", bitMap->bitarray);

}

int proxBloqueVacio(){
	for (int i = 0; i<10; i++){
		bool bit = bitarray_test_bit(bitMap, i);
		log_info(loggerFS, "Accediendo al bit %i:         %s", i,string_itoa(bit));
		if (bit == 0) return i;
	}
	log_warning(loggerFS, "NO HAY BLOQUES DISPONIBLES");
	return -1;
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
   //aca vaciar bloque
   bitarray_clean_bit(bitMap, numeroBloque);
}

void leerArchivoBloques(void* aLeer, int posicion, int cantidad){
	FILE* archivo_bloques = fopen(pathBloques(), "r+b");
	fseek(archivo_bloques, posicion, SEEK_SET);
	fread(aLeer, cantidad,1, archivo_bloques);
	fclose(archivo_bloques);
}

void escribirArchivoBloques(void* aEscribir, int posicion, int cantidad){
	log_trace(loggerFS, "escribiendo archivo pos: %d , cant: %d", posicion, cantidad);
	FILE* archivo_bloques = fopen(pathBloques(), "r+b");
	fseek(archivo_bloques, posicion, SEEK_SET);
	fwrite(aEscribir, cantidad,1, archivo_bloques);
	fclose(archivo_bloques);
}
