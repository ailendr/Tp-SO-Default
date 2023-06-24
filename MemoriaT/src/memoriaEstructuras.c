/*
 * memoriaEstructuras.c
 *
 *  Created on: Jun 10, 2023
 *      Author: utnso
 */
#include "memoriaEstructuras.h"

void* memoriaContigua; //GLOBAL
t_segmento* segmentoCero;
t_list * listaDeTablas;
t_list* listaDeSegmentos;

/*Es para tener un hueco libre al principio cuando no hay segmentos y para cuando se compacte y queda contiguo el espacio libre*/
t_segmento* segmentoLibre;

///////////////////////////// ESTRUCTURAS DE MEMORIA/////////////////////////////////////////////////////////

void crearListas(){
	listaDeTablas = list_create();
	listaDeSegmentos = list_create();

}

void iniciarEstructuras(){
	crearEspacioMemoria();
	crearSegmentoCero();
	crearListas();
}

void crearEspacioMemoria (){
	int tamanioMemoria = tam_memoria();
	memoriaContigua = malloc(tamanioMemoria);
}

void crearSegmentoCero(){
	segmentoCero = malloc(sizeof(t_segmento));
	segmentoCero->ID=0;
	segmentoCero->base=0;
	segmentoCero->limite = tam_segmento_cero();//Es lo mismo que hacer base + tamanio en este caso
	segmentoCero->tamanio=tam_segmento_cero();
	segmentoCero->estaEnMemoria=1;
	list_add(listaDeSegmentos, segmentoCero);
	actualizarUltimoSegmentoLibre();

}

//////////////////////////UTILS DE MEMORIA/////////////////////////////////////////////

int memoriaOcupada(t_list* lista){
	int tamLista=list_size(lista);
	int memoriaOcupada = 0;
	for(int i =0; i<tamLista;i++){
	t_segmento* segmento = list_get(lista, i);
	memoriaOcupada+= segmento->tamanio;
	}
	return memoriaOcupada;
}

int memoriaDisponible(){
	t_list* listaHuecosLibres = list_filter(listaDeSegmentos, (void*)segmentoOcupado);
	int tamanioHuecosOcupados = memoriaOcupada(listaHuecosLibres);
	int tamMemoria = tam_memoria();
	int memoriaDis = tamMemoria - tamanioHuecosOcupados;
	return memoriaDis;
}


int  buscarPosSegmento(uint32_t idSegmento, uint32_t pid, t_list* lista){
	int tamLista = list_size(lista);
	int i = 0;
	t_segmento* segmento = list_get(lista, i);
	while(i<=tamLista && segmento->ID!=idSegmento && segmento->PID!=pid){
  		i++;
  		segmento=list_get(lista, i);
 	}
	return i;
}

bool huecoLibre(t_segmento* segmento){
	return segmento->estaEnMemoria ==0;
}

bool segmentoOcupado(t_segmento* segmento){
	return segmento->estaEnMemoria ==1;
}


void actualizarUltimoSegmentoLibre(){
	int ultimaPos=list_size(listaDeSegmentos);
	t_segmento* ultimoSegmento = list_get(listaDeSegmentos, ultimaPos);
	segmentoLibre->base = ultimoSegmento->limite+1;
	segmentoLibre->tamanio=tam_memoria() - memoriaOcupada(listaDeSegmentos);
	segmentoLibre->limite=tam_memoria();
	segmentoLibre->estaEnMemoria=0;
	list_add_in_index(listaDeSegmentos, ultimaPos+1,segmentoLibre);
}

//Mueve en una pos toda la lista de segmentos
void actualizarListaDeSegmentos(t_segmento* nuevoSegmento, t_segmento* segmento){
	t_list* listaAux = list_create();
	int tamLista = list_size(listaDeSegmentos);
	int pos= buscarPosSegmento(nuevoSegmento->ID, nuevoSegmento->PID,listaDeSegmentos);
	list_add(listaAux, segmento);
	list_add_all(listaAux,list_slice_and_remove(listaDeSegmentos, pos+1, tamLista));
	list_add_all(listaDeSegmentos, listaAux);
	free(listaAux);
}


void unirHuecosAledanios(t_segmento* segmento){
	int pos = buscarPosSegmento(segmento->ID,segmento->PID, listaDeSegmentos);
	t_segmento* segmentoAnterior =list_get(listaDeSegmentos, pos-1);
	t_segmento* segmentoSiguiente = list_get(listaDeSegmentos, pos+1);

	 if(huecoLibre(segmentoAnterior)){
		 segmento->base=segmentoAnterior->base;
		 segmento->tamanio = segmento->tamanio + segmentoAnterior->tamanio;
		 list_remove(listaDeSegmentos, pos-1);

	 }else if(huecoLibre(segmentoSiguiente)){
		 segmento->tamanio += segmentoSiguiente->tamanio;
		 segmento->limite=segmentoSiguiente->limite;
		 list_remove(listaDeSegmentos, pos + 1);
	 }
}

void actualizarTablaDeSegmentos(t_list* tablaDeSegmentos){
	int tam=list_size(tablaDeSegmentos);
	for (int i=0; i<tam; i++){
		t_segmento* segmento = list_get(tablaDeSegmentos, i);
		int posListaSeg = buscarPosSegmento(segmento->ID, segmento->PID, listaDeSegmentos);
		t_segmento* segActualizado = list_get(listaDeSegmentos, posListaSeg);
		list_replace(tablaDeSegmentos, i, segActualizado);
	}
}

void logearListaDeSegmentos(char* mensaje){
	int tamLista = list_size(listaDeSegmentos);
	log_info(loggerMemoria, "Lista de segmentos %s: ", mensaje);
	for(int i =0; i<tamLista; i++){
		t_segmento* segmento = list_get(listaDeSegmentos, i);
		int pos=buscarPosSegmento(segmento->ID, segmento->PID, listaDeSegmentos);
		log_info(loggerMemoria, "PID: %d - Segmento: %d - Base: %d - Tamaño %d", segmento->PID,segmento->ID, segmento->base, segmento->tamanio);
		log_info(loggerMemoria, "Pos en la lista: %d", pos);
	}
}

////////////////////////////////FUNCIONES DE MEMORIA///////////////////////////////

t_list* crearTablaDeSegmentos(uint32_t pid){

	t_list* tablaDeSegmentos = list_create();
	list_add(tablaDeSegmentos, segmentoCero);
	list_add_in_index(listaDeTablas,pid, tablaDeSegmentos);
	log_info(loggerMemoria, "Creacion de proceso: %d", pid);
	return tablaDeSegmentos;
}

void liberarTablaDeSegmentos(uint32_t pid){
	t_list* tablaALiberar= list_get(listaDeTablas, pid);
	int tamTabla = list_size(tablaALiberar);
	//Marco como libres a los segmentos del proceso
	for(int i=0;i<tamTabla;i++){
		t_segmento* segmentoEnTabla= list_get(tablaALiberar, i);
		//deleteSegment(segmento->ID, pid); No se si esta bien usar esta funcion
		//Dejo esta otra opcion
		int pos = buscarPosSegmento(segmentoEnTabla->ID, pid, listaDeSegmentos);
		t_segmento* segmentoEnLista= list_get(listaDeSegmentos, pos);
        segmentoEnLista->estaEnMemoria=0;
		//list_replace(listaDeSegmentos, pos, segmento); //NO USAR REPLACE PARA ACTUALIZAR PORQUE GENERA INCONSISTENCIA: DEJAR LO DE ARRIBA

	}
	list_clean(listaDeTablas);// Rlimina los segmentos de la tabla
	//Habria que usar list_clean_and_destroy_elements pero mañana me fijo bien
	//list_clean_and_destroy_elements(listaDeTablas, element_destroyer) ->HAY QUE MANDARLE UNA FUNCION QUE INDIQUE COMO SE LIBERA ESA ESTRUCTURA
	list_remove(listaDeTablas, pid);// Elimina la tabla
	log_info(loggerMemoria, "Eliminación de proceso: %d", pid);
}

void deleteSegment(uint32_t id, uint32_t pid){ //no me cabe pasarle el pid porq el segmento ya lo tiene
	int pos = buscarPosSegmento(id, pid ,listaDeSegmentos);
	t_segmento* segmentoAEliminar = list_get(listaDeSegmentos,pos);
	//Actualizo la tabla de segmentos del proceso
	t_list* tablaDeSegmentosAActualizar = list_get(listaDeTablas,pid);
	int posEnTabla = buscarPosSegmento(id, pid, tablaDeSegmentosAActualizar);
	list_remove(tablaDeSegmentosAActualizar,posEnTabla);//lo elimino de la tabla de segmentos
	segmentoAEliminar->estaEnMemoria=0;
	//Actualizo en la lista de segmentos que ya no esta en memoria
	list_replace(listaDeSegmentos, pos, segmentoAEliminar);
	log_info(loggerMemoria,"Eliminación de Segmento: “PID: %d - Eliminar Segmento: %d - Base: %d - TAMAÑO: %d",pid,id,segmentoAEliminar->base, segmentoAEliminar->tamanio );
	//Falta la parte de unir con segmentos aledaños si estan libres
	unirHuecosAledanios(segmentoAEliminar);

}


void compactar(){
	log_info(loggerMemoria,"Solicitud de Compactación");
	logearListaDeSegmentos("antes de compactar");
	t_list* listaAux=list_filter(listaDeSegmentos, (void*)segmentoOcupado);//creo una lista aux solo con los segmentos ocupados
	list_clean(listaDeSegmentos);//dejo vacia la lista de segmentos
	int tamanioLista=list_size(listaAux);

	//voy actualizando los segmentos y los vuelvo a cargar en la lista de segmentos
	for(int i=1; i<=tamanioLista; i++){//Lo inicio en 1 porque segmentoCero siempre tiene los mismos valores
		t_segmento* segmento = list_get(listaAux, i);
		t_segmento* segAnterior = list_get(listaAux, i-1);
		segmento->base = segAnterior->limite +1;
		segmento->limite = segmento->base + segmento->tamanio;
		list_add(listaDeSegmentos, segmento);
	}
	actualizarUltimoSegmentoLibre();
	free(listaAux);
	logearListaDeSegmentos("despues de compactar");
	//Actualizo las tablas de segmento, ponele
	int tamListaTablas = list_size(listaDeTablas);
	int j=0;
	while(j<=tamListaTablas){
		t_list* tablaDeSegmentos = list_get(listaDeTablas, j);
		if(!list_is_empty(tablaDeSegmentos)){
			actualizarTablaDeSegmentos(tablaDeSegmentos);
		}
		j++;
	}
}

