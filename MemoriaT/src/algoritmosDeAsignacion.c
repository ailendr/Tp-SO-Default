/*
 * algoritmosReemplazo.c
 *
 *  Created on: Jun 18, 2023
 *      Author: utnso
 */

#include "algoritmosDeAsignacion.h"


uint32_t createSegment(t_segmento* nuevoSegmento, uint32_t tamanio){
	if(memoriaDisponible()>=tamanio ){ //Iba a validar que no pase los 16 segmentos por cada proceso pero en el tp seccion KERNEL dice que no se pedira crear un id del seg superior al arch de config
		algAsignacion algoritmo= asignarAlgoritmo();
		t_list* listaHuecosLibres = list_filter(listaDeSegmentos, (void*)huecoLibre);
		int tamanio=list_size(listaHuecosLibres);
		int tamSegmento = nuevoSegmento->tamanio;
		int posSegLibre;
		switch(algoritmo){
		case firstFit:
			posSegLibre= FirstFit(tamSegmento);
		break;
		case bestFit:
			posSegLibre= WorstYBest(tamSegmento, listaHuecosLibres, (void*)BestFit);
		break;
		case worstFit:
			posSegLibre = WorstYBest(tamSegmento, listaHuecosLibres, (void*)WorstFit);
		}
		if(posSegLibre!=-1){
			t_segmento* segLibre = list_get(listaDeSegmentos, posSegLibre);
			nuevoSegmento->base=segLibre->base;
			nuevoSegmento->limite =nuevoSegmento->base + nuevoSegmento->tamanio;
			nuevoSegmento->estaEnMemoria=1;
			//void *list_replace(t_list*, int index, void* element);
			//No me gusta nada como se busca el hueco libre en la lista seg porq 1) que valor le ponemos al id y pid cuando creemos ese seg libre? si le ponemos -1 luego en la lista de segmentos habra mas de uno que cumpla y 2) si no se le pone nada al id y pid se llena de basura y cambia de valor siempre entonces imposible encontrarlo en la lista d seg

			t_segmento* segmentoLibre = malloc(sizeof(t_segmento));
			segmentoLibre= list_replace(listaDeSegmentos, posSegLibre,nuevoSegmento);
			segmentoLibre->base=nuevoSegmento->limite+1;
			segmentoLibre->tamanio = segLibre->tamanio - nuevoSegmento->tamanio;
			segmentoLibre->limite = segLibre->base + segLibre->tamanio;
			segmentoLibre->ID=-1; //agrego esto pero NO
			segmentoLibre->PID=-1;//idem
			//muevo de a un lugar la pos de los segmentos desde seglibre
			actualizarListaDeSegmentos(nuevoSegmento, segmentoLibre);
			//Actualizo tabla de segmentos
			int posDeTabla = posTablaEnLista(listaDeTablas,nuevoSegmento->PID);
			t_tabla* tablaDeSegmentos = list_get(listaDeTablas, posDeTabla);//Ver esto al debugguear
			list_add(tablaDeSegmentos->segmentos, nuevoSegmento);

			log_info(loggerMemoria, "PID: %d - Crear Segmento: %d - Base: %d - TAMAÑO: %d", nuevoSegmento->PID, nuevoSegmento->ID, nuevoSegmento->base, tamanio);

			return OK;
		}
		else {
			return COMPACTAR;
		}

	}
	else{
		return ERROR;

	}
}

/*
void actualizarListaDeSegmentos(int pos, t_segmento* segmento){
	int tamLista=list_size(listaDeSegmentos);
	t_segmento* segmentoAux;
	for(int i=pos;i<=tamLista;i++){
		segmentoAux=list_replace(listaDeSegmentos, i, segmento);
		segmento = segmentoAux;
	}
}
*/
algAsignacion asignarAlgoritmo(){
	algAsignacion algoritmo;
	if (strcmp(algoritmoAsignacion(), "FIRST") == 0) algoritmo = firstFit;
	if (strcmp(algoritmoAsignacion(), "WORST") == 0) algoritmo = worstFit;
	if (strcmp(algoritmoAsignacion(), "BEST") == 0) algoritmo = bestFit;
	return algoritmo;
}

int FirstFit(uint32_t tamSegmento){
	return huecoLibreDisponible(tamSegmento,listaDeSegmentos);
}



int huecoLibreDisponible(uint32_t tamSegmento, t_list* listaDeSegmentos){
	int tamanioLista = list_size(listaDeSegmentos);
	int i =0;
		while(i <= tamanioLista){
			t_segmento* huecoFree= list_get(listaDeSegmentos, i);
			if(tamSegmento <= huecoFree->tamanio && huecoLibre(huecoFree)){
				return i;
				}
			i++;
			}
	     return -1;
	}


bool BestFit(t_segmento* segmento1, t_segmento* segmento2){
		return segmento1->tamanio <= segmento2->tamanio;

}

bool WorstFit(t_segmento* segmento1, t_segmento* segmento2){
		return segmento1->tamanio >= segmento2->tamanio;
}


int WorstYBest(uint32_t tamSegmento, t_list* listaDeSegmentos, bool(algoritmo)(t_segmento* seg1, t_segmento* seg2)){
	int tamanio = list_size(listaDeSegmentos);
	t_segmento* huecoFree1 = NULL;
	//int i =0;
	//int j=1;
	int iterador = 0;
	while(iterador < tamanio ){
		t_segmento* segmento=list_get(listaDeSegmentos, iterador);
		if(huecoLibre(segmento)){
        huecoFree1 = segmento;
        return iterador;
		}
	}
	return 0;
}
	/* TODO
	t_segmento* segmento1=list_get(listaHuecosLibres, i);
	t_segmento* segmento2=list_get(listaHuecosLibres, j);
	while(i<=tamanioLista && j< tamanioLista){
		if(segmento1->tamanio>=tamSegmento && algoritmo(segmento1, segmento2)){
			huecoLibre = segmento1;
			segmento2 = list_get(listaHuecosLibres, j++);
		}else if(segmento2->tamanio >= tamSegmento){
			huecoLibre = segmento2;
			segmento1 = list_get(listaHuecosLibres, i++);
		}
		else {
			i++; j++;
		}
	}
	return huecoLibre;
}*/

/*/Como removemos la tabla de segmentos de la lista de tablas tenemos que buscar en que posicion esta//
int posTablaEnList(t_list* listaDeTablas,uint32_t pid){
	int tamanio = list_size(listaDeTablas);
	for(int i=0; i< tamanio; i++){
		t_list* tabla = list_get(listaDeTablas,i);
        if(pidEnTabla(tabla,pid)){
        	return i;
        	}
		}
	return -1;
}

//Similar al Any Satisfy para ver si esta los segmentos con ese pid entonces la tabla corresponde a ese pid//
bool pidTabla(t_list* tabla, uint32_t pid){
	int* pidTabla = list_get(tabla,0);
	printf("el valor del pid es : %d", *pidTabla);
	return *pidTabla == pid;
}
*/
