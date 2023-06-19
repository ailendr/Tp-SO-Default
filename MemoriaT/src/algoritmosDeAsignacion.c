/*
 * algoritmosReemplazo.c
 *
 *  Created on: Jun 18, 2023
 *      Author: utnso
 */

#include "algoritmosDeAsignacion.h"


void createSegment(t_segmento* nuevoSegmento, uint32_t tamanio){
	if(memoriaDisponible()>=tamanio){
		algAsignacion algoritmo= asignarAlgoritmo();
		int pudoAsignar;
		switch(algoritmo){
		case firstFit:
			pudoAsignar= asignarHuecoLibre(nuevoSegmento, (void*)FirstFit);
		break;
		case bestFit:
			pudoAsignar= asignarHuecoLibre(nuevoSegmento, (void*)BestFit);
		break;
		case worstFit:
			pudoAsignar= asignarHuecoLibre(nuevoSegmento, (void*)WorstFit);
		}
		if(pudoAsignar==1){
			//hacer send de base a kernel
			log_info(loggerMemoria, "PID: %d - Crear Segmento: %d - Base: %d - TAMAÑO: %d", nuevoSegmento->PID, nuevoSegmento->ID, nuevoSegmento->base, tamanio);
		}
		else if(pudoAsignar==0){
			//hacer send de que hay que compactar
		}

	}
}

void actualizarListaDeSegmentos(int pos, t_segmento* segmento){
	int tamLista=list_size(listaDeSegmentos);
	t_segmento* segmentoAux;
	for(int i=pos;i<=tamLista;i++){
		segmentoAux=list_replace(listaDeSegmentos, i, segmento);
		segmento = segmentoAux;
	}
}

algAsignacion asignarAlgoritmo(){
	algAsignacion algoritmo;
	if (strcmp(algoritmoAsignacion(), "FIRST") == 0) algoritmo = firstFit;
	if (strcmp(algoritmoAsignacion(), "WORST") == 0) algoritmo = worstFit;
	if (strcmp(algoritmoAsignacion(), "BEST") == 0) algoritmo = bestFit;
	return algoritmo;
}

int asignarHuecoLibre(t_segmento* nuevoSegmento, t_segmento*(*algoritmo)(uint32_t tamSegmento, t_list* lista, int tam)){
	t_list* listaHuecosLibres = list_filter(listaDeSegmentos, (void*)huecoLibre);
	int tamanio=list_size(listaHuecosLibres);
	int tamSegmento = nuevoSegmento->tamanio;
	t_segmento* huecoLibre = algoritmo(tamSegmento, listaHuecosLibres, tamanio);
	if(huecoLibre!=NULL){
		nuevoSegmento->base=huecoLibre->base;
		nuevoSegmento->limite =nuevoSegmento->base + nuevoSegmento->tamanio;
		huecoLibre->base=nuevoSegmento->limite+1;
		huecoLibre->tamanio = huecoLibre->tamanio - nuevoSegmento->tamanio;
		huecoLibre->limite = huecoLibre->base + huecoLibre->tamanio;
		//No se si hay que respetar el orden de como estaban, asi que asi muevo de a un lugar la pos de los segmentos desde huecolibre
		actualizarListaDeSegmentos(huecoLibre);
		actualizarUltimoSegmentoLibre();
		return 1;
	}
	else {
		return 0;}
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

//////YA SE QUE MATE 546354653 GATITOS///////////

t_segmento* BestFit(uint32_t tamSegmento, t_list* listaHuecosLibres, int tamanioLista){
	t_segmento* huecoLibre=NULL;
	int i =0;
	int j=1;
	t_segmento* segmento1=list_get(listaHuecosLibres, i);
	t_segmento* segmento2=list_get(listaHuecosLibres, j);
	while(i<=tamanioLista && j< tamanioLista){
		if(segmento1->tamanio>=tamSegmento && segmento1->tamanio <= segmento2->tamanio){
			huecoLibre = segmento1;
			segmento2 = list_get(listaHuecosLibres, j++);
		}else if(segmento2->tamanio >= tamSegmento){
			huecoLibre = segmento2;
			segmento1 = list_get(listaHuecosLibres, i++);
		}
		else {
			i++; j++;}
	}

	return huecoLibre;

}

t_segmento* WorstFit(uint32_t tamSegmento, t_list* listaHuecosLibres, int tamanioLista){
	t_segmento* huecoLibre=NULL;
	int i =0;
	int j=1;
	t_segmento* segmento1=list_get(listaHuecosLibres, i);
	t_segmento* segmento2=list_get(listaHuecosLibres, j);
	while(i<=tamanioLista && j< tamanioLista){
		if(segmento1->tamanio>=tamSegmento && segmento1->tamanio >= segmento2->tamanio){
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
