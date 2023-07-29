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
	log_info(loggerFS, "CANTIDAD DE BLOQUES: %s",string_itoa(cantidadDeBloques));
	if (cantidadDeBloques > 1) cantidadDeBloques++;
	//El + 1 es por el bloque que tiene todos los punteros
	return cantidadDeBloques;
}

void eliminarBloques (int cantidadDeBloques, t_fcb* fcb){

}

void agregarBloques (int cantidadDeBloques, t_fcb* fcb){

	int proxCargar;

	if (fcb->punteroDirecto == -1){
		cantidadDeBloques --;
		proxCargar = proxBloqueVacio();
		bitarray_set_bit(bitMap, proxCargar);
		fcb->punteroDirecto = proxCargar;
	}

	if (fcb->punteroIndirecto == -1){
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
		if (bitarray_test_bit(bitMap, 0) == 0) return i;
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

    memcpy(superBloque -> blockSize + offset,contenido, sizeContenido);


}

void liberarBloque(uint32_t numeroBloque){
   //aca vaciar bloque
   bitarray_clean_bit(bitMap, numeroBloque);
}


