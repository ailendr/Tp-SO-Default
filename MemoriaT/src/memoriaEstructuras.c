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



int memoriaOcupada(){
	int tamLista=list_size(listaDeSegmentos);
	int memoriaOcupada = 0;
	for(int i =0; i<tamLista;i++){
	t_segmento* segmento = list_get(listaDeSegmentos, i);
	memoriaOcupada+= segmento->tamanio;
	}
	return memoriaOcupada;
}

int memoriaDisponible(){
	int tamMemoria = tam_memoria();
	int memoriaDis = tamMemoria - memoriaOcupada();
	return memoriaDis;
}

void liberarTablaDeSegmentos(uint32_t pid){
	t_list* tablaALiberar= list_get(listaDeTablas, pid);
	list_remove(listaDeTablas, pid);
	free(tablaALiberar);
	log_info(loggerMemoria, "Eliminación de proceso: %d", pid);
}
/*
int buscarSegmento(uint32_t idSegmento){
	//Esta funcion nos podria servir
}
*/
bool huecoLibre(t_segmento* segmento){
	return segmento->estaEnMemoria ==0;
}



////////////////////////////////FUNCIONES DE MEMORIA///////////////////////////////

/*
void createSegment(){
	if(memoriaDisponible()>=tamanioSegmento()){
}*/


//O seria que llega el id del segmento? ----------> le podemos enviar el id --->

void deleteSegment(t_segmento* segmentoAEliminar, int id){
	int tamLista = list_size(listaDeSegmentos);
	int i = 0;
	t_segmento* segmento = list_get(listaDeSegmentos, i);
	while(i<=tamLista && segmento->ID!=id){
  		i++;
  		segmento=list_get(listaDeSegmentos, i);
 	}
	//Actualizo la tabla de segmentos del proceso
	uint32_t pid = segmento->PID;
	t_list* tablaDeSegmentosAActualizar = list_get(listaDeTablas,pid);
	list_remove_element(tablaDeSegmentosAActualizar, segmento);
	segmento->estaEnMemoria=0;
	log_info(loggerMemoria,"Eliminación de Segmento: “PID: <%d> - Eliminar Segmento: <%d> - Base: <%D> - TAMAÑO: <%d>",pid,id,segmentoAEliminar->base, segmentoAEliminar->tamanio );
	//Esto es por si usamos la lista de huecos libres
	//list_add(listaHuecosLibres, segmento);
}



