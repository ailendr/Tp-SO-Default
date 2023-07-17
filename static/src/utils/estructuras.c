/*
 * estructuras.c
 *
 *  Created on: Jul 11, 2023
 *      Author: utnso
 */


#include "estructuras.h"

void destruirSegmento(t_segmento* self){
	self->PID = 0;
	self->ID=0;
	self->limite=0;
	self->base=0;
	self->tamanio=0;
	self->estaEnMemoria= 0;
	self->tieneInfo = NULL;
	self->tamanioInfo = 0;
	free(self);
}

void destruirProceso(t_pcb* self){
	close(self->socketConsola);
	//list_destroy_and_destroy_elements(self->tablaSegmentos, (void*) destruirTabla);
	list_destroy(self->archAbiertos); //HAY QUE VER COMO FINALIZAMOS LOS ARCHIVOS ABIERTOS
	destruirContexto(self->contexto);
	free(self->contexto);
	free(self);

}

void destruirContexto(t_contextoEjec* self){
	self->pid=0;
	list_destroy_and_destroy_elements(self->instrucciones, (void*) destruirInstruccion);
	self->PC=0;
/*
	self->AX="";
	self->BX="";
	self->CX='/0';
	self->DX='/0';

	self->EAX='/0';
	self->EBX='/0';
	self->ECX='/0';
	self->EDX='/0';

	self->RAX='/0';
	self->RBX='/0';
	self->RCX='/0';
	self->RDX='/0';
	*/
}

void destruirInstruccion(char* self){ //Hago esto porque al deserializar se pide memoria para cada una con malloc
	free(self);
}

void destruirTabla(t_tabla* self){
	list_destroy_and_destroy_elements(self->segmentos, (void*) destruirSegmento); //Elimino la lista donde estan los segmentos
	self->PID = 0;
	free(self);

}


//LO PONGO ACA PORQ LO USARÁ KERNEL
//Como removemos la tabla de segmentos de la lista de tablas tenemos que buscar en que posicion esta
int posTablaEnLista(t_list* listaDeTablas,uint32_t pid){
	int tamanio = list_size(listaDeTablas);
	for(int i=0; i< tamanio; i++){
		t_tabla* tabla = list_get(listaDeTablas,i);
        if(tabla->PID == pid){
        	return i;
        	}
		}
	return -1;
}

void loggearTablaDeSegmentos(t_tabla* tabla, t_log* logger){
	int tamanio = list_size(tabla->segmentos);
		log_info(logger, "La Tabla de Segmentos contiene :");
			for (int i = 0; i < tamanio; i++){
				 t_segmento* segmento= list_get(tabla->segmentos,i);
				 log_info(logger, "PID: %d - Segmento: %d - Base: %d - Tamaño %d- En Memoria: %d", segmento->PID,segmento->ID, segmento->base, segmento->tamanio, segmento->estaEnMemoria);
			}
	}


/*//Similar al Any Satisfy para ver si esta los segmentos con ese pid entonces la tabla corresponde a ese pid//
bool pidEnTabla(t_list* tabla, uint32_t pid){
	int tam = list_size(tabla);
	int cant = 0;
	for(int i=0; i< tam; i++ ){
		t_segmento* segmento = list_get(tabla,i);
		if(segmento->PID == pid){
			cant += 1;
		}
	}

	return cant>0;
}*/
