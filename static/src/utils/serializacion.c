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

// SERIALIZAR ESTRUCTURA INTRUCCION -------------------------------------------------------------------------------------------------------
t_paquete* serializarInstrucciones(t_instruccion* instruc){

	t_paquete* pInstruc = malloc(sizeof(t_paquete));
	pInstruc->codigo_operacion = instruc->nombre;
	pInstruc->buffer = malloc(sizeof(t_buffer));
	pInstruc->buffer->size = sizeof(uint32_t)*4 + strlen(instruc->param1) + strlen(instruc->param2) + strlen(instruc->param3);
	pInstruc->buffer->stream = NULL;

	int offset = 0;

	memcpy(pInstruc->buffer->stream +offset, &(instruc->pid), sizeof(uint32_t));
	offset += sizeof(uint32_t);

	serializacionParametros (pInstruc, offset, instruc, 1);

	return pInstruc;

}

t_paquete* serializacionParametros (t_paquete* pInstruc, int offset, t_instruccion* instruc, int param){

	char* cargar;
	uint32_t tamParam = 0;

	if (param == 1) cargar = instruc->param1;
	if (param == 2) cargar = instruc->param2;
	if (param == 3) cargar = instruc->param3;

	if (cargar == NULL) cargar = "";

	tamParam = strlen(cargar);

	memcpy(pInstruc->buffer->stream +offset, &(tamParam), sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(pInstruc->buffer->stream +offset, &(cargar), sizeof(strlen(cargar)));
	offset += sizeof(strlen(cargar));
	serializacionParametros(pInstruc, offset, instruc, param++);

	return pInstruc;
}

t_instruccion* deserializarInstruccionEstructura (void* buffer/*, int tamanio*/){

	t_instruccion* instruccion = malloc(sizeof(t_instruccion));

	void * stream = buffer;
	int desplazamiento = 0;
	int tamParam = 0;

	memcpy(&(instruccion->pid), stream+desplazamiento, sizeof(uint32_t));
	desplazamiento+=sizeof(uint32_t);

	memcpy(&(tamParam), stream+desplazamiento,sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);
	memcpy(&(instruccion->param1), stream+desplazamiento,tamParam);
	desplazamiento+= tamParam;

	memcpy(&(tamParam), stream+desplazamiento,sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);
	memcpy(&(instruccion->param2), stream+desplazamiento,tamParam);
	desplazamiento+= tamParam;

	memcpy(&(tamParam), stream+desplazamiento,sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);
	memcpy(&(instruccion->param3), stream+desplazamiento,tamParam);
	desplazamiento+= tamParam;


	return instruccion;

}
