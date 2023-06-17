/*
 * bloques.c
 *
 *  Created on: Jun 15, 2023
 *      Author: utnso
 */
t_bitarray* bitMap;

pthread_mutex_t mutexBlocks;
pthread_mutex_init(&mutexBlocks, NULL);

void escribirBloque (void* contenido, uint32_t sizeContenido, uint32_t numeroBloque){
    int offset = 0;
    if (sizeContenido > superBloque -> blockSize){
        log_warning(loggerFS, "No es posible escribir %d bytes al bloque %d", sizeContenido, numeroBloque);
    }
    //Semaforo Mutex
    pthread_mutex_lock(&mutexBlocks);
    //chequear que este libre el bloque (bitMap)bitarray_set_bit
    memcpy(superBloque -> blockSize + offset, contenido, sizeContenido);
    pthread_mutex_unlock(&mutexBlocks);
    log_info(loggerFS, "Se escribio un bloque");
}

void agregarContenidoABloque (void* contenido, uint32_t sizeContenido, uint32_t offset , uint32_t numeroBloque){
    if (sizeContenido + offset > superBloque -> blockSize){
    log_warning(loggerFS, "Se quiso agregar %d bytes al bloque %d en el offset %d", sizeContenido,numeroBloque, offset);

    pthread_mutex_lock(&mutexBlocks);
    memcpy(superBloque -> blockSize + offset,contenido, sizeContenido);
    pthread_mutex_unlock(&mutexBlocks);

}



void liberarBloque(uint32_t numeroBloque){
   pthread_mutex_lock(&mutexBlocks);
   //aca vaciar bloque
   bitarray_clean_bit(bitMap, numeroBloque);
   pthread_mutex_unlock(&mutexBlocks);
}


