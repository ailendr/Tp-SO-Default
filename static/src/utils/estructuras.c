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
	free(self);
}

void destruirProceso(t_pcb* self){
	close(self->socketConsola);
	list_destroy_and_destroy_elements(self->tablaSegmentos, (void*) destruirSegmento);
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
