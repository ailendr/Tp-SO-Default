/*
 * algoritmosReemplazo.c
 *
 *  Created on: Jun 18, 2023
 *      Author: utnso
 */

#include "algoritmosDeAsignacion.h"


uint32_t createSegment(t_segmento* nuevoSegmento, uint32_t tamanio){
	if(memoriaDisponible()>=tamanio){
		algAsignacion algoritmo= asignarAlgoritmo();
		t_segmento* segLibre;
		t_list* listaHuecosLibres = list_filter(listaDeSegmentos, (void*)huecoLibre);
		int tamanio=list_size(listaHuecosLibres);
		int tamSegmento = nuevoSegmento->tamanio;
		switch(algoritmo){
		case firstFit:
			segLibre= FirstFit(tamSegmento, listaHuecosLibres, tamanio);
		break;
		case bestFit:
			segLibre= WorstYBest(tamSegmento, listaHuecosLibres, tamanio, (void*)BestFit);
		break;
		case worstFit:
			segLibre= WorstYBest(tamSegmento, listaHuecosLibres, tamanio, (void*)WorstFit);
		}
		if(segLibre!=NULL){
			nuevoSegmento->base=segLibre->base;
			nuevoSegmento->limite =nuevoSegmento->base + nuevoSegmento->tamanio;
			nuevoSegmento->estaEnMemoria=1;
			segLibre->base=nuevoSegmento->limite+1;
			segLibre->tamanio = segLibre->tamanio - nuevoSegmento->tamanio;
			segLibre->limite = segLibre->base + segLibre->tamanio;
			//muevo de a un lugar la pos de los segmentos desde seglibre
			actualizarListaDeSegmentos(nuevoSegmento, segLibre);

			//Actualizo tabla de segmentos
			t_list* tablaDeSegmentos = list_get(listaDeTablas, nuevoSegmento->PID);
			list_add(tablaDeSegmentos, nuevoSegmento);

			log_info(loggerMemoria, "PID: %d - Crear Segmento: %d - Base: %d - TAMAÑO: %d", nuevoSegmento->PID, nuevoSegmento->ID, nuevoSegmento->base, tamanio);

			return nuevoSegmento->base;
		}
		else {
			return COMPACTAR;
		}

	}
	else{
		return HANDSHAKE_OutOfMemory;

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

t_segmento* FirstFit(uint32_t tamSegmento, t_list* listaHuecosLibres, int tamanioLista){
	int i =0;
	t_segmento* huecoLibre=NULL;
	while(i <= tamanioLista && tamSegmento>=huecoLibre->tamanio){
		huecoLibre = list_get(listaHuecosLibres, i);
		i++;
	}
	return huecoLibre;

}


bool BestFit(t_segmento* segmento1, t_segmento* segmento2){
		return segmento1->tamanio <= segmento2->tamanio;

}

bool WorstFit(t_segmento* segmento1, t_segmento* segmento2){
		return segmento1->tamanio >= segmento2->tamanio;
}


t_segmento* WorstYBest(uint32_t tamSegmento, t_list* listaHuecosLibres, int tamanioLista, bool(algoritmo)(t_segmento* seg1, t_segmento* seg2)){
	t_segmento* huecoLibre=NULL;
	int i =0;
	int j=1;
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
}
