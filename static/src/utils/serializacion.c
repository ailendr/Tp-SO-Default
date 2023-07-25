/*
 * serializacion.c
 *
 *  Created on: May 28, 2023
 *      Author: utnso
 */

#include "serializacion.h"

/// ---------TO-DO LO QUE TIENE QUE VER CON INSTRUCCIONES Y PROCESOS---------///

t_list* deserializarInstrucciones(void *buffer, int* desplazamiento,int tamanioBuffer) {
	t_list *listaInstrucciones = list_create();
	int tamanioInstruccion;
	while (*desplazamiento < tamanioBuffer) {
		memcpy(&tamanioInstruccion, buffer + *desplazamiento, sizeof(int));
		*desplazamiento += sizeof(int);
		char *instruccion = malloc(tamanioInstruccion);
		memcpy(instruccion, buffer + *desplazamiento, tamanioInstruccion);
		*desplazamiento += tamanioInstruccion;
		list_add(listaInstrucciones, instruccion);
	}
	return listaInstrucciones;
}

/*t_contextoEjec* deserializarContextoViejo(void *buffer, int tamanioContexto, int* desplazamiento) {

	t_contextoEjec *contexto = malloc(sizeof(t_contextoEjec));

	void *stream = buffer;

	memcpy(&(contexto->pid), stream + *desplazamiento, sizeof(uint32_t));
		*desplazamiento += sizeof(uint32_t);
	memcpy(&(contexto->PC), stream + *desplazamiento, sizeof(uint32_t));
	*desplazamiento += sizeof(uint32_t);

	memcpy(&(contexto->AX), stream + *desplazamiento, 4);
	*desplazamiento += 4;
	memcpy(&(contexto->BX), stream + *desplazamiento, 4);
	*desplazamiento += 4;
	memcpy(&(contexto->CX), stream + *desplazamiento, 4);
	*desplazamiento += 4;
	memcpy(&(contexto->DX), stream + *desplazamiento, 4);
	*desplazamiento += 4;

	memcpy(&(contexto->EAX), stream + *desplazamiento, 8);
	*desplazamiento += 8;
	memcpy(&(contexto->EBX), stream + *desplazamiento, 8);
	*desplazamiento += 8;
	memcpy(&(contexto->ECX), stream + *desplazamiento, 8);
	*desplazamiento += 8;
	memcpy(&(contexto->EDX), stream + *desplazamiento, 8);
	*desplazamiento += 8;

	memcpy(&(contexto->RAX), stream + *desplazamiento, 16);
	*desplazamiento += 16;
	memcpy(&(contexto->RBX), stream + *desplazamiento, 16);
	*desplazamiento += 16;
	memcpy(&(contexto->RCX), stream + *desplazamiento, 16);
	*desplazamiento += 16;
	memcpy(&(contexto->RDX), stream + *desplazamiento, 16);
	*desplazamiento += 16;


	contexto->instrucciones = deserializarInstrucciones(stream, desplazamiento,
			tamanioContexto);
    //free(buffer); Hacer estos free a mano donde se deserialice un contexto
	return contexto;
}
*/

////----SERIALIZACION DE CONTEXTO------///

t_paquete* serializarContexto(t_contextoEjec *procesoAEjecutar) { //En realidad pone todo en un paquete
	t_paquete *paqueteContexto = malloc(sizeof(t_paquete));
	paqueteContexto->codigo_operacion = CONTEXTO;
	paqueteContexto->buffer = malloc(sizeof(t_buffer));
	paqueteContexto->buffer->size = sizeof(uint32_t)*2+ 4*4+8*4+16*4; //Nota: cuando serializa la lista de instrucciones pide memoria con realloc
	paqueteContexto->buffer->stream = malloc(paqueteContexto->buffer->size);
	int offset = 0;

	memcpy(paqueteContexto->buffer->stream + offset, &(procesoAEjecutar->pid), sizeof(uint32_t));
	offset += sizeof(uint32_t);
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
t_paquete* serializarInstruccion(t_instruccion* instruc){
    int cantParametros = cantidadDeParametros(instruc->nombre);
	t_paquete* pInstruc = malloc(sizeof(t_paquete));
	pInstruc->codigo_operacion = instruc->nombre;
	pInstruc->buffer = malloc(sizeof(t_buffer));
	pInstruc->buffer->size = sizeof(uint32_t)+ sizeof(op_code);
	pInstruc->buffer->stream = malloc(pInstruc->buffer->size); //Agrego esto porq es necesario reservarle memoria al stream

	int offset = 0;

	memcpy(pInstruc->buffer->stream +offset, &(instruc->pid), sizeof(uint32_t));
	offset += sizeof(uint32_t);

	memcpy(pInstruc->buffer->stream +offset, &(instruc->nombre), sizeof(op_code));
	offset += sizeof(op_code);

	agregarParametros(pInstruc, instruc, cantParametros);
	return pInstruc;

}


int cantidadDeParametros(op_code instruccion){
	int cantidad = 0;
	if(instruccion == EXIT|| instruccion == YIELD || instruccion == CREAR_TABLA) cantidad = 0;
	if(instruccion == IO || instruccion == WAIT || instruccion == SIGNAL || instruccion == DELETE_SEGMENT) cantidad = 1;
	if(instruccion == SET || instruccion == MOV_IN || instruccion == MOV_OUT || instruccion == F_OPEN|| instruccion == F_CLOSE || instruccion == F_SEEK || instruccion == F_TRUNCATE|| instruccion == CREATE_SEGMENT || instruccion == MENSAJE) cantidad = 2;
	if(instruccion== F_READ || instruccion == F_WRITE) cantidad =3;
	return cantidad;

}
void agregarParametros (t_paquete* pInstruc, t_instruccion* instruc, int param){


	if(param== 1){
		agregarParametro1(pInstruc, instruc);
	}
	if(param==2){
		agregarParametro1(pInstruc, instruc);
		agregarParametro2(pInstruc, instruc);
	}
	if(param == 3){
		agregarParametro1(pInstruc, instruc);
		agregarParametro2(pInstruc, instruc);
		agregarParametro3(pInstruc, instruc);
	}

}




void agregarParametro1(t_paquete* pInstruc,t_instruccion* instruc){
		 char*parametro = instruc->param1;
		 int tamParam = strlen(parametro)+1;
		 agregar_a_paquete(pInstruc, parametro, tamParam);

	}
void agregarParametro2(t_paquete* pInstruc,t_instruccion* instruc){
		 char* parametro = instruc->param2;
		 int tamParam = strlen(parametro)+1;
		 agregar_a_paquete(pInstruc, parametro, tamParam);
	}
void agregarParametro3(t_paquete* pInstruc,t_instruccion* instruc){
		char* parametro = instruc->param3;
		int tamParam = strlen(parametro)+1;
		 agregar_a_paquete(pInstruc, parametro, tamParam);
	}



t_instruccion* deserializarInstruccionEstructura (void* buffer, int cantParam, int* desplazamiento){

	t_instruccion* instruccion = malloc(sizeof(t_instruccion));

	void * stream = buffer;
	int tamParam = 0;

	memcpy(&(instruccion->pid), stream+*desplazamiento, sizeof(uint32_t));
	*desplazamiento+=sizeof(uint32_t);
//agrego esto por haberle enviado el nombre al serializar//
	memcpy(&(instruccion->nombre), stream+*desplazamiento, sizeof(op_code));
	*desplazamiento+= sizeof(op_code);
//---Hermosa repeticion de codigo pero toy cansada--//
	if(cantParam == 1){
	memcpy(&(tamParam), stream+*desplazamiento,sizeof(int));
	*desplazamiento+= sizeof(int);
   	instruccion -> param1 = malloc(tamParam);
	memcpy(instruccion->param1, stream+*desplazamiento,tamParam);
	}
   if(cantParam == 2){
	   memcpy(&(tamParam), stream+*desplazamiento,sizeof(int));
	   	*desplazamiento+= sizeof(int);
	   	instruccion -> param1 = malloc(tamParam);
	   	memcpy(instruccion->param1, stream+*desplazamiento,tamParam);
	   	*desplazamiento+= tamParam;

	   	memcpy(&(tamParam), stream+*desplazamiento,sizeof(int));
		*desplazamiento+= sizeof(int);
		instruccion->param2 = malloc(tamParam);
		memcpy(instruccion->param2, stream+*desplazamiento,tamParam);
		*desplazamiento+= tamParam;
   }
   if(cantParam == 3){
	   memcpy(&(tamParam), stream+*desplazamiento,sizeof(int));
		*desplazamiento+= sizeof(int);
	   	instruccion -> param1 = malloc(tamParam);
		memcpy(instruccion->param1, stream+*desplazamiento,tamParam);
		*desplazamiento+= tamParam;

		memcpy(&(tamParam), stream+*desplazamiento,sizeof(int));
		*desplazamiento+= sizeof(int);
		instruccion->param2 = malloc(tamParam);
		memcpy(instruccion->param2, stream+*desplazamiento,tamParam);
		*desplazamiento+= tamParam;
	   memcpy(&(tamParam), stream+*desplazamiento,sizeof(int));
	   	*desplazamiento+= sizeof(int);
	   	instruccion->param3 = malloc(tamParam);
	   	memcpy(instruccion->param3, stream+*desplazamiento,tamParam);

   }
	return instruccion;
}

t_instruccion* obtenerInstruccion(int socket, int cantParam){
	int tamanio = 0;
	int desplazamiento = 0;
	void *buffer = recibir_buffer(&tamanio, socket);
	t_instruccion* instruccionNueva = deserializarInstruccionEstructura(buffer, cantParam, &desplazamiento);
	free(buffer);
	return instruccionNueva;
}
////----SERIALIZACION Y DESERIALIZACION DE  CONTEXTO+INSTRUCCION = PAQUETE----////

t_paquete* serializarContextoCompuesto(t_contextoEjec *procesoAEjecutar, t_instruccion* instruc) {
	t_paquete *paqueteContexto = malloc(sizeof(t_paquete));
	paqueteContexto->codigo_operacion = PAQUETE;
	paqueteContexto->buffer = malloc(sizeof(t_buffer));
	paqueteContexto->buffer->size =  sizeof(int) // un entero para guardar el tamaño del contexto
			                        + sizeof(uint32_t)*2+ 4*4+8*4+16*4; //Nota: cuando serializa la lista de instrucciones pide memoria con realloc
	paqueteContexto->buffer->stream = malloc(paqueteContexto->buffer->size);
	int offset = 0;
	int bytesLista = bytesListaInstrucciones(procesoAEjecutar->instrucciones);
    int tamContexto = sizeof(uint32_t)*2+ 4*4 + 8*4 + 16*4 + bytesLista;

	memcpy(paqueteContexto->buffer->stream + offset, &(tamContexto), sizeof(int));
	offset += sizeof(int);

	memcpy(paqueteContexto->buffer->stream + offset, &(procesoAEjecutar->pid), sizeof(uint32_t));
	offset += sizeof(uint32_t);
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
	t_list *instrucciones = procesoAEjecutar->instrucciones;
	int tamanioInstrucciones = list_size(instrucciones);
	for (int i = 0; i < tamanioInstrucciones; i++) {
		char *instruccion = list_get(instrucciones, i);
		agregar_a_paquete(paqueteContexto, instruccion,
				strlen(instruccion) + 1);
	}
     //---Serializacion de la instruccion---//
	    int cantParametros = cantidadDeParametros(instruc->nombre);
        op_code tipoInstruccion = instruc->nombre; //funciona como nuestro codigo de paquete
		paqueteContexto->buffer->stream = realloc(paqueteContexto->buffer->stream, paqueteContexto->buffer->size
																				   + sizeof(op_code)
				                                                                  + sizeof(uint32_t)+ sizeof(op_code));

		memcpy(paqueteContexto->buffer->stream +paqueteContexto->buffer->size, &(tipoInstruccion), sizeof(op_code));
		paqueteContexto->buffer->size += sizeof(op_code);

		memcpy(paqueteContexto->buffer->stream +paqueteContexto->buffer->size, &(instruc->pid), sizeof(uint32_t));
		paqueteContexto->buffer->size += sizeof(uint32_t);

		memcpy(paqueteContexto->buffer->stream +paqueteContexto->buffer->size, &(instruc->nombre), sizeof(op_code));
		paqueteContexto->buffer->size += sizeof(op_code);

		agregarParametros(paqueteContexto, instruc, cantParametros);

		return paqueteContexto;

}


	int bytesListaInstrucciones(t_list* instrucciones){
		int tamanioInstrucciones = list_size(instrucciones);
		int bytesAcumulados = 0;
			for (int i = 0; i < tamanioInstrucciones; i++) {
				char* instruccion = list_get(instrucciones, i);
				bytesAcumulados += strlen(instruccion)+ 1 + sizeof(int); // un int para almacenar el tamaño + bytes del tamaño de cada instruccion char
			}
			return bytesAcumulados;
	}

///DESERIALIZACION ES POR PARTES //
t_contextoEjec* deserializarContexto(void *buffer, int tamanioContexto, int* desplazamiento) {

		t_contextoEjec *contexto = malloc(sizeof(t_contextoEjec));

		void *stream = buffer;

		memcpy(&(contexto->pid), stream + *desplazamiento, sizeof(uint32_t));
			*desplazamiento += sizeof(uint32_t);
		memcpy(&(contexto->PC), stream + *desplazamiento, sizeof(uint32_t));
		*desplazamiento += sizeof(uint32_t);

		memcpy(&(contexto->AX), stream + *desplazamiento, 4);
		*desplazamiento += 4;
		memcpy(&(contexto->BX), stream + *desplazamiento, 4);
		*desplazamiento += 4;
		memcpy(&(contexto->CX), stream + *desplazamiento, 4);
		*desplazamiento += 4;
		memcpy(&(contexto->DX), stream + *desplazamiento, 4);
		*desplazamiento += 4;

		memcpy(&(contexto->EAX), stream + *desplazamiento, 8);
		*desplazamiento += 8;
		memcpy(&(contexto->EBX), stream + *desplazamiento, 8);
		*desplazamiento += 8;
		memcpy(&(contexto->ECX), stream + *desplazamiento, 8);
		*desplazamiento += 8;
		memcpy(&(contexto->EDX), stream + *desplazamiento, 8);
		*desplazamiento += 8;

		memcpy(&(contexto->RAX), stream + *desplazamiento, 16);
		*desplazamiento += 16;
		memcpy(&(contexto->RBX), stream + *desplazamiento, 16);
		*desplazamiento += 16;
		memcpy(&(contexto->RCX), stream + *desplazamiento, 16);
		*desplazamiento += 16;
		memcpy(&(contexto->RDX), stream + *desplazamiento, 16);
		*desplazamiento += 16;

        int unidadesTrasladadas = sizeof(uint32_t)*2+ 4*4+8*4+16*4;
		contexto->instrucciones = deserializarListaInstruc(stream, desplazamiento,
				tamanioContexto, unidadesTrasladadas);
	    //free(buffer); Hacer estos free a mano donde se deserialice un contexto
		return contexto;
	}

t_list* deserializarListaInstruc(void *buffer, int* desplazamiento,int tamanioBuffer, int offset) {
	t_list *listaInstrucciones = list_create();
	int tamanioInstruccion = 0;
	while (offset < tamanioBuffer) {
		memcpy(&tamanioInstruccion, buffer + *desplazamiento, sizeof(int));
		*desplazamiento += sizeof(int);
		offset += sizeof(int);
		char *instruccion = malloc(tamanioInstruccion);
		memcpy(instruccion, buffer + *desplazamiento, tamanioInstruccion);
		*desplazamiento += tamanioInstruccion;
		offset += tamanioInstruccion;
		list_add(listaInstrucciones, instruccion);
	}
	return listaInstrucciones;
}


int recibirTamContexto(void* buffer, int* desplazamiento){
		int tamanio = 0;
		memcpy(&tamanio, buffer + *desplazamiento, sizeof(int));
		*desplazamiento += sizeof(int);
		return tamanio;
	}
int recibirTipoInstruccion(void* buffer, int* desplazamiento){ //Fijarme q onda esto si en un int hay problema q se reciba un op_code sino al serializar enviar como int
	int instruccion = 0;
	memcpy(&instruccion, buffer + *desplazamiento, sizeof(op_code));
	*desplazamiento += sizeof(op_code);
	return instruccion;

}

////SERIALIZACION Y DESERIALIZACION DE TABLA DE SEGMENTOS  : no voy a enviar un paquete porque el codigo de operacion no lo voy a usar/////

void serializarTablaDeSegmentos(t_tabla* tabla, t_buffer* buffer){
	    int tamanio = list_size(tabla->segmentos);
	  	buffer->stream = realloc(buffer->stream, buffer->size + sizeof(uint32_t) );
		memcpy(buffer->stream + buffer->size, &(tabla->PID), sizeof(uint32_t));
		buffer->size += sizeof(uint32_t);
		for(int i=0; i<tamanio; i++){ //Mando el segmento Cero como uno mas y que tenga el PID con basura o lo puedo hacer aparte para evitar mandar el (segmento->pid) a piacere
			t_segmento* segmento = list_get(tabla->segmentos,i);
			serializarSegmento(segmento,buffer);
		}
}

t_tabla* deserializarTablaDeSegmentos(void* buffer,int* desplazamiento, int size){ //El que llame a deserializar debe recibir el buffer antes
        t_tabla* tablaDeSegmentos = malloc(sizeof(t_tabla));
        tablaDeSegmentos->segmentos = list_create();
		memcpy(&(tablaDeSegmentos->PID), buffer + *desplazamiento, sizeof(uint32_t));
        *desplazamiento = sizeof(uint32_t);

        while(*desplazamiento<size){
		t_segmento* segmento = deserializarSegmento(buffer, desplazamiento);//Nota: Al debugguear fijarme si el desplazamiento incrementa correctamente sino pasarle la direc en memoria
		list_add(tablaDeSegmentos->segmentos,segmento);
		}
		return tablaDeSegmentos;
}

void serializarSegmento(t_segmento* segmento, t_buffer* buffer){
		buffer->stream = realloc(buffer->stream, buffer->size + 6* sizeof(uint32_t) );
		memcpy(buffer->stream + buffer->size, &(segmento->PID), sizeof(uint32_t));
		buffer->size += sizeof(uint32_t);
		memcpy(buffer->stream+ buffer->size, &(segmento->ID), sizeof(uint32_t));
		buffer->size += sizeof(uint32_t);
		memcpy(buffer->stream + buffer->size, &(segmento->base), sizeof(uint32_t));
		buffer->size += sizeof(uint32_t);
		memcpy(buffer->stream+ buffer->size, &(segmento->tamanio), sizeof(uint32_t));
		buffer->size += sizeof(uint32_t);
		memcpy(buffer->stream+ buffer->size, &(segmento->limite), sizeof(uint32_t));
		buffer->size += sizeof(uint32_t);
		memcpy(buffer->stream+ buffer->size, &(segmento->estaEnMemoria), sizeof(uint32_t));
		buffer->size += sizeof(uint32_t);

}

t_segmento* deserializarSegmento(void* buffer, int* desplazamiento){
		t_segmento* segmento = malloc(sizeof(t_segmento));
		memcpy(&(segmento->PID), buffer+*desplazamiento, sizeof(uint32_t));
		*desplazamiento += sizeof(uint32_t);
		memcpy(&(segmento->ID), buffer+*desplazamiento, sizeof(uint32_t));
		*desplazamiento += sizeof(uint32_t);
		memcpy(&(segmento->base), buffer+*desplazamiento, sizeof(uint32_t));
		*desplazamiento += sizeof(uint32_t);
		memcpy(&(segmento->tamanio), buffer+*desplazamiento, sizeof(uint32_t));
		*desplazamiento += sizeof(uint32_t);
		memcpy(&(segmento->limite), buffer+*desplazamiento, sizeof(uint32_t));
		*desplazamiento += sizeof(uint32_t);
		memcpy(&(segmento->estaEnMemoria), buffer+*desplazamiento, sizeof(uint32_t));
		*desplazamiento += sizeof(uint32_t);
		return segmento;
}

/// SERIALIZACION Y DESERIALIZACION DE LISTA DE TABLAS///
t_buffer* serializarListaDeTablas(t_list* listaDeTablas){
	    int tamanio = list_size(listaDeTablas);
		t_buffer* buffer = malloc(sizeof(t_buffer));
		buffer->size = 0;
		buffer->stream = NULL;
		for(int i=0; i<tamanio; i++){
			t_tabla* tablaDeSeg = list_get(listaDeTablas,i);
			int cantSeg = list_size(tablaDeSeg->segmentos);
			int sizeTabla = sizeof(uint32_t)+ sizeof(uint32_t)*6*cantSeg;
		  	buffer->stream = realloc(buffer->stream, buffer->size + sizeof(int));
			memcpy(buffer->stream + buffer->size, &sizeTabla, sizeof(int));
			buffer->size += sizeof(int);
	        serializarTablaDeSegmentos(tablaDeSeg, buffer);
	        //buffer->size y agregarlo al buffer
		}
	return buffer;
}

t_list* deserializarListaDeTablas(int socket){
		int size = 0;
		int desplazamiento = 0;
		t_list* listaDeTablas = list_create();
		void* bufferListaDeTablas= recibir_buffer(&size,socket);
		while(desplazamiento<size){
			int tamTabla;
		   memcpy(&tamTabla, bufferListaDeTablas+desplazamiento, sizeof(int));
		   desplazamiento += sizeof(int);
		   t_tabla* tablaDeSeg = deserializarTabla(bufferListaDeTablas,&desplazamiento,tamTabla);//Nota: Al debugguear fijarme si el desplazamiento incrementa correctamente sino pasarle la direc en memoria
		   list_add(listaDeTablas,tablaDeSeg);
		}
		free(bufferListaDeTablas);
		return listaDeTablas;
}

t_tabla* deserializarTabla(void* buffer,int* desplazamiento, int tamTabla){
		int offset = 0;
		t_tabla* tablaDeSegmentos = malloc(sizeof(t_tabla));
        tablaDeSegmentos->segmentos = list_create();
		memcpy(&(tablaDeSegmentos->PID), buffer + *desplazamiento, sizeof(uint32_t));
        *desplazamiento += sizeof(uint32_t);
        offset += sizeof(uint32_t);
        while(offset<tamTabla){
		t_segmento* segmento = deserializarSegmento(buffer, desplazamiento);//Nota: Al debugguear fijarme si el desplazamiento incrementa correctamente sino pasarle la direc en memoria
		offset += sizeof(uint32_t)*6;
		list_add(tablaDeSegmentos->segmentos,segmento);
		}
		return tablaDeSegmentos;

}
