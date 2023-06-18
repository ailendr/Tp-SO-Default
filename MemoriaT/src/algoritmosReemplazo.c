/*
 * algoritmosReemplazo.c
 *
 *  Created on: Jun 18, 2023
 *      Author: utnso
 */

#include "algoritmosReemplazo.h"

/*
void createSegment(uint32_t tamanio){

	if(memoriaDisponible()>=tamanio){

	}
}
*/

void asignarHuecoLibre(t_segmento* nuevoSegmento, t_segmento*(*algoritmo)(uint32_t tamSegmento, t_list* lista, int tam)){
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
		//Falta agregar a la lista de segmentos, no se si hay que respetar el orden porque sino hay que mover
		//todos los segmentos de lugar una posicion a partir de huecoLibre
	}
	//No creo que vaya aca pero lo dejo asi por ahora, despues lo cambio
	else {
		compactar();
	}
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
