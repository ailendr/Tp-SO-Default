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
	crearListas();
	crearEspacioMemoria();
	crearSegmentoCero();
	actualizarUltimoSegmentoLibre();
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
	t_list* listaHuecosOcupados = list_filter(listaDeSegmentos, (void*)segmentoOcupado);
	int tamanioHuecosOcupados = memoriaOcupada(listaHuecosOcupados);
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
	segmentoLibre = malloc(sizeof(t_segmento));
	int ultimaPos=list_size(listaDeSegmentos)-1;
	t_segmento* ultimoSegmento = list_get(listaDeSegmentos, ultimaPos);
	segmentoLibre->ID=-1;
	segmentoLibre->PID=-1;
	segmentoLibre->base = ultimoSegmento->limite+1; //No sé si debe estar ese +1
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
	list_destroy_and_destroy_elements(listaAux, (void*)destruirSegmento);



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
	    list_replace_and_destroy_element(tablaDeSegmentos, i,segActualizado,(void*)destruirSegmento);
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

t_tabla* crearTablaDeSegmentos(uint32_t pid){

	t_tabla* tablaDeSegmentos = malloc(sizeof(t_tabla));
	tablaDeSegmentos->segmentos = list_create();
    tablaDeSegmentos->PID = pid;
	//log_info(loggerMemoria,"el pid recuperado es : %d",*identificador);
	list_add(tablaDeSegmentos->segmentos, segmentoCero);
	list_add_in_index(listaDeTablas,pid, tablaDeSegmentos);
	log_info(loggerMemoria, "Creacion de proceso: %d", pid);
	return tablaDeSegmentos;
}

void liberarTablaDeSegmentos(uint32_t pid){
	int posDeTabla = posTablaEnLista(listaDeTablas,pid);
	t_tabla* tablaALiberar= list_get(listaDeTablas, posDeTabla);
	int tamTabla = list_size(tablaALiberar->segmentos);
	//Marco como libres a los segmentos del proceso
	for(int i=0;i<tamTabla;i++){
		t_segmento* segmentoEnTabla= list_get(tablaALiberar->segmentos, i);
		//deleteSegment(segmento->ID, pid); No se si esta bien usar esta funcion
		//Dejo esta otra opcion
		int pos = buscarPosSegmento(segmentoEnTabla->ID, pid, listaDeSegmentos);
		t_segmento* segmentoEnLista= list_get(listaDeSegmentos, pos);
        segmentoEnLista->estaEnMemoria=0;
		//list_replace(listaDeSegmentos, pos, segmento); //NO USAR REPLACE PARA ACTUALIZAR PORQUE GENERA INCONSISTENCIA: DEJAR LO DE ARRIBA
	}
	//list_clean_and_destroy_elements(tablaALiberar->segmentos, (void*) destruirSegmento); //Destruimos los segmentos de la Tabla de Segmentos
	list_remove_and_destroy_element(listaDeTablas,posDeTabla, (void*)destruirTabla);//Destruirmos esa Tabla de Segmentos de la Lista de Tablas
	log_info(loggerMemoria, "Eliminación de proceso: %d", pid);
}

t_tabla* deleteSegment(uint32_t id, uint32_t pid) { //Me sirve que retorne la tabla actualizada
	//Se pone el segmento como libre en la Lista de Segmentos
	int pos = buscarPosSegmento(id, pid ,listaDeSegmentos);
	t_segmento* segmentoAEliminar = list_get(listaDeSegmentos,pos);
	segmentoAEliminar->estaEnMemoria=0;

	//Actualizo la tabla de segmentos del proceso: Eliminando ese segmento de la tabla
	int posDeTabla = posTablaEnLista(listaDeTablas,pid);//Primero se busca la tabla en la lista global de tablas
	t_tabla* tablaDeSegmentosAActualizar = list_get(listaDeTablas,posDeTabla);
	//Se busca el segmento en la Tabla de segmentos
	t_list* segmentos = tablaDeSegmentosAActualizar->segmentos;
	int posSegEnTabla = buscarPosSegmento(id, pid, segmentos);
	list_remove_and_destroy_element(segmentos,posSegEnTabla,(void*)destruirSegmento);
	log_info(loggerMemoria,"Eliminación de Segmento: “PID: %d - Eliminar Segmento: %d - Base: %d - TAMAÑO: %d",pid,id,segmentoAEliminar->base, segmentoAEliminar->tamanio );
	//Falta la parte de unir con segmentos aledaños si estan libres
	unirHuecosAledanios(segmentoAEliminar);
    return tablaDeSegmentosAActualizar;
}

void compactar() {
	log_info(loggerMemoria,"Solicitud de Compactación");
	logearListaDeSegmentos("Antes de compactar");
	t_list* listaAux=list_filter(listaDeSegmentos, (void*)segmentoOcupado);//creo una lista aux solo con los segmentos ocupados
	list_clean_and_destroy_elements(listaDeSegmentos,(void*) destruirSegmento);//Es mejor destruir los elementos tambien por si queda algo

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
	list_destroy_and_destroy_elements(listaAux,(void*)destruirSegmento);
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


