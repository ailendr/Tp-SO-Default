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
//t_list* listaHuecosLibres;

/*Esto pense tenerlo como el hueco mas grande que va quedando cuando se va llenando la memoria para ver cuando empezar a aplicar
los algoritmos de reemplazo
t_segmento* segmentoLibre; */


void crearListas(){
	listaDeTablas = list_create();
	listaDeSegmentos = list_create();
//	listaHuecosLibres = list_create();

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

}

t_list* crearTablaDeSegmentos(uint32_t pid){

	t_list* tablaDeSegmentos = list_create();
	segmentoCero->PID=pid;
	list_add(tablaDeSegmentos, segmentoCero);
	list_add_in_index(tablaDeSegmentos,pid, tablaDeSegmentos);
	log_info(loggerMemoria, "Creacion de proceso: %d", pid);
	return tablaDeSegmentos;
}



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
	t_list* listaHuecosLibres = list_filter(listaDeSegmentos, (void*)huecoLibre);
	int tamanioHuecosLibres = memoriaOcupada(listaHuecosLibres);
	int tamMemoria = tam_memoria();
	int memoriaDis = tamMemoria - memoriaOcupada(listaDeSegmentos) + tamanioHuecosLibres;
	return memoriaDis;
}

void liberarTablaDeSegmentos(uint32_t pid){
	t_list* tablaALiberar= list_get(listaDeTablas, pid);
	list_remove(listaDeTablas, pid);
	free(tablaALiberar);
	log_info(loggerMemoria, "Eliminación de proceso: %d", pid);
}

int  buscarPosSegmento(uint32_t idSegmento, t_list* lista){
	int tamLista = list_size(lista);
	int i = 0;
	t_segmento* segmento = list_get(lista, i);
	while(i<=tamLista && segmento->ID!=idSegmento){
  		i++;
  		segmento=list_get(lista, i);
 	}
	return i;
}

bool huecoLibre(t_segmento* segmento){
	return segmento->estaEnMemoria ==0;
}

////////////////////////////////FUNCIONES DE MEMORIA///////////////////////////////



//O seria que llega el id del segmento? ----------> le podemos enviar el id --->

void deleteSegment(uint32_t id){
	int pos = buscarPosSegmento(id, listaDeSegmentos);
	t_segmento* segmentoAEliminar = list_get(listaDeSegmentos,pos);
	//Actualizo la tabla de segmentos del proceso
	uint32_t pid = segmentoAEliminar->PID;
	t_list* tablaDeSegmentosAActualizar = list_get(listaDeTablas,pid);
	list_remove_element(tablaDeSegmentosAActualizar, segmentoAEliminar);
	segmentoAEliminar->estaEnMemoria=0;
	//Actualizo en la lista de segmentos que ya no esta en memoria
	list_replace(listaDeSegmentos, pos, segmentoAEliminar);
	log_info(loggerMemoria,"Eliminación de Segmento: “PID: <%d> - Eliminar Segmento: <%d> - Base: <%d> - TAMAÑO: <%d>",pid,id,segmentoAEliminar->base, segmentoAEliminar->tamanio );
	//Esto es por si usamos la lista de huecos libres
	//list_add(listaHuecosLibres, segmento);
}

void compactar(){

}

