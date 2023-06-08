/*
 * serializacion.c
 *
 *  Created on: May 28, 2023
 *      Author: utnso
 */

#include "serializacion.h"

/// ---------TO-DO LO QUE TIENE QUE VER CON INSTRUCCIONES Y PROCESOS---------///

t_list* deserializarInstrucciones(void *buffer, int desplazamiento,
		int tamanioBuffer) {
	t_list *listaInstrucciones = list_create();
	int tamanioInstruccion;
	while (desplazamiento < tamanioBuffer) {
		memcpy(&tamanioInstruccion, buffer + desplazamiento, sizeof(int));
		desplazamiento += sizeof(int);
		char *instruccion = malloc(tamanioInstruccion);
		memcpy(instruccion, buffer + desplazamiento, tamanioInstruccion);
		desplazamiento += tamanioInstruccion;
		list_add(listaInstrucciones, instruccion);
	}
	return listaInstrucciones;
}

t_contextoEjec* deserializarContexto(void *buffer, int tamanio) {

	t_contextoEjec *contexto = malloc(sizeof(t_contextoEjec));

	void *stream = buffer;
	int tamanioBuffer = tamanio;
	int desplazamiento = 0;

	memcpy(&(contexto->PC), stream + desplazamiento, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(&(contexto->AX), stream + desplazamiento, 4);
	desplazamiento += 4;
	memcpy(&(contexto->BX), stream + desplazamiento, 4);
	desplazamiento += 4;
	memcpy(&(contexto->CX), stream + desplazamiento, 4);
	desplazamiento += 4;
	memcpy(&(contexto->DX), stream + desplazamiento, 4);
	desplazamiento += 4;

	memcpy(&(contexto->EAX), stream + desplazamiento, 8);
	desplazamiento += 8;
	memcpy(&(contexto->EBX), stream + desplazamiento, 8);
	desplazamiento += 8;
	memcpy(&(contexto->ECX), stream + desplazamiento, 8);
	desplazamiento += 8;
	memcpy(&(contexto->EDX), stream + desplazamiento, 8);
	desplazamiento += 8;

	memcpy(&(contexto->RAX), stream + desplazamiento, 16);
	desplazamiento += 16;
	memcpy(&(contexto->RBX), stream + desplazamiento, 16);
	desplazamiento += 16;
	memcpy(&(contexto->RCX), stream + desplazamiento, 16);
	desplazamiento += 16;
	memcpy(&(contexto->RDX), stream + desplazamiento, 16);
	desplazamiento += 16;

	memcpy(&(contexto->pid), stream + desplazamiento, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	contexto->instrucciones = deserializarInstrucciones(stream, desplazamiento,
			tamanioBuffer);

	return contexto;
}

int bytesListaInstrucciones(t_list* instrucciones){
	int tamanioInstrucciones = list_size(instrucciones);
	int bytesAcumulados = 0;
		for (int i = 0; i < tamanioInstrucciones; i++) {
			char* instruccion = list_get(instrucciones, i);
			bytesAcumulados += strlen(instruccion)+1;
		}
		return bytesAcumulados;
}

t_paquete* serializarContexto(t_contextoEjec *procesoAEjecutar) { //En realidad pone todo en un paquete
	////----SERIALIZACION DE CONTEXTO------///

	t_paquete *paqueteContexto = malloc(sizeof(t_paquete));
	paqueteContexto->codigo_operacion = CONTEXTO;
	paqueteContexto->buffer = malloc(sizeof(t_buffer));
	int bytesLista = bytesListaInstrucciones(procesoAEjecutar->instrucciones);
	paqueteContexto->buffer->size = sizeof(uint32_t)*2+ 4*4+8*4+16*4; //Nota: cuando serializa la lista de instrucciones pide memoria con realloc
	paqueteContexto->buffer->stream = malloc(paqueteContexto->buffer->size);
	int offset = 0;

	memcpy(paqueteContexto->buffer->stream + offset, &(procesoAEjecutar->PC), sizeof(uint32_t));
	offset += sizeof(uint32_t);

	memcpy(paqueteContexto->buffer->stream + offset, &(procesoAEjecutar->AX), 4);
	offset += 4;
	memcpy(paqueteContexto->buffer->stream + offset, &(procesoAEjecutar->BX), 4);
	offset += 4;
	memcpy(paqueteContexto->buffer->stream + offset, &(procesoAEjecutar->CX), 4);
	offset += 4;
	memcpy(paqueteContexto->buffer->stream + offset, &(procesoAEjecutar->DX), 4);
	offset += 4;

	memcpy(paqueteContexto->buffer->stream + offset, &(procesoAEjecutar->EAX), 8);
	offset += 8;
	memcpy(paqueteContexto->buffer->stream + offset, &(procesoAEjecutar->EBX), 8);
	offset += 8;
	memcpy(paqueteContexto->buffer->stream + offset, &(procesoAEjecutar->ECX), 8);
	offset += 8;
	memcpy(paqueteContexto->buffer->stream + offset, &(procesoAEjecutar->EDX), 8);
	offset += 8;

	memcpy(paqueteContexto->buffer->stream + offset, &(procesoAEjecutar->RAX), 16);
	offset += 16;
	memcpy(paqueteContexto->buffer->stream + offset, &(procesoAEjecutar->RBX), 16);
	offset += 16;
	memcpy(paqueteContexto->buffer->stream + offset, &(procesoAEjecutar->RCX), 16);
	offset += 16;
	memcpy(paqueteContexto->buffer->stream + offset, &(procesoAEjecutar->RDX), 16);
	offset += 16;

	memcpy(paqueteContexto->buffer->stream + offset, &(procesoAEjecutar->pid), sizeof(uint32_t));
	offset += sizeof(uint32_t);

	t_list *instrucciones = procesoAEjecutar->instrucciones;
	int tamanioInstrucciones = list_size(instrucciones);
	for (int i = 0; i < tamanioInstrucciones; i++) {
		char *instruccion = list_get(instrucciones, i);
		agregar_a_paquete(paqueteContexto, instruccion,
				strlen(instruccion) + 1);
	}
	return paqueteContexto;
}
