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
los algoritmos de asignacion*/
t_segmento* segmentoLibre;


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
	actualizarUltimoSegmentoLibre();
	list_add(listaDeSegmentos, segmentoLibre);


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
	int tamTabla = list_size(tablaALiberar);
	//Marco como libres a los segmentos del proceso
	for(int i=0;i<tamTabla;i++){
		t_segmento* segmento= list_get(tablaALiberar, i);
		//deleteSegment(segmento->ID, pid); No se si esta bien usar esta funcion
		//Dejo esta otra opcion
		int pos = buscarPosSegmento(segmento->ID, pid, listaDeSegmentos);
		segmento->estaEnMemoria=0;
		list_replace(listaDeSegmentos, pos, segmento);

	}
	list_remove(listaDeTablas, pid);
	free(tablaALiberar);
	log_info(loggerMemoria, "Eliminación de proceso: %d", pid);
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


void actualizarUltimoSegmentoLibre(){
	int ultimaPos=list_size(listaDeSegmentos);
	t_segmento* ultimoSegmento = list_get(listaDeSegmentos, ultimaPos);
	segmentoLibre->base = ultimoSegmento->limite+1;
	segmentoLibre->tamanio=tam_memoria() - memoriaOcupada(listaDeSegmentos);
	segmentoLibre->limite=tam_memoria();
	segmentoLibre->estaEnMemoria=0;
	//list_add_in_index(listaDeSegmentos, ultimaPos+1,segmentoLibre);
}

////////////////////////////////FUNCIONES DE MEMORIA///////////////////////////////


void deleteSegment(uint32_t id, uint32_t pid){
	int pos = buscarPosSegmento(id, pid ,listaDeSegmentos);
	t_segmento* segmentoAEliminar = list_get(listaDeSegmentos,pos);
	//Actualizo la tabla de segmentos del proceso
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

