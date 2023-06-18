/*
 * algoritmosReemplazo.c
 *
 *  Created on: Jun 18, 2023
 *      Author: utnso
 */

#include "algoritmosReemplazo.h"

void asignarHuecoLibre(t_segmento* nuevoSegmento, t_segmento*(*algoritmo)(t_segmento* segmento, t_list* lista, int tam)){
	t_list* listaHuecosLibres = list_filter(listaDeSegmentos, (void*)huecoLibre);
	int tamanio=list_size(listaHuecosLibres);
	t_segmento* huecoLibre = algoritmo(nuevoSegmento, listaHuecosLibres, tamanio);
	if(huecoLibre!=NULL){
		nuevoSegmento->base=huecoLibre->base;
		nuevoSegmento->limite =nuevoSegmento->base + nuevoSegmento->tamanio;
		huecoLibre->base=nuevoSegmento->limite+1;
		huecoLibre->tamanio = huecoLibre->tamanio - nuevoSegmento->tamanio;
		huecoLibre->limite = huecoLibre->base + huecoLibre->tamanio;
		//Falta agregar a la lista de segmentos, no se si hay que respetar el orden porque sino hay que mover
		//todos los segmentos de lugar una posicion a partir de huecoLibre
	}
	//supongo que habria que ver si hay memoria suficiente para compactar o si mandar out of memory
	//Lo dejo comentado como una idea, pero no creo que vaya en esta funcion jajaja
	/*
	else if(memoriaDisponible>= nuevoSegmento->tamanio){
		compactar();

	}else{
		//Avisar que no hay memoria
	}*/
	else{//Dejo esto por ahora por dejar algo
		log_info(loggerMemoria, "No hay memoria suficiente");
	}
}

t_segmento* firstFit(t_segmento* nuevoSegmento, t_list* listaHuecosLibres, int tamanioLista){
	int i =0;
	t_segmento* huecoLibre=NULL;
	while(i <= tamanioLista && nuevoSegmento->tamanio>=huecoLibre->tamanio){
		huecoLibre = list_get(listaHuecosLibres, i);
		i++;
	}
	return huecoLibre;

}

//////YA SE QUE MATE 546354653 GATITOS///////////

t_segmento*bestFit(t_segmento* nuevoSegmento, t_list* listaHuecosLibres, int tamanioLista){
	t_segmento* huecoLibre=NULL;
	int i =0;
	int j=1;
	t_segmento* segmento1=list_get(listaHuecosLibres, i);
	t_segmento* segmento2=list_get(listaHuecosLibres, j);
	while(i<=tamanioLista && j< tamanioLista){
		if(segmento1->tamanio>=nuevoSegmento->tamanio && segmento1->tamanio <= segmento2->tamanio){
			huecoLibre = segmento1;
			segmento2 = list_get(listaHuecosLibres, j++);
		}else if(segmento2->tamanio >= nuevoSegmento->tamanio){
			huecoLibre = segmento2;
			segmento1 = list_get(listaHuecosLibres, i++);
		}
		else {
			i++; j++;}
	}

	return huecoLibre;

}

t_segmento* worstFit(t_segmento* nuevoSegmento, t_list* listaHuecosLibres, int tamanioLista){
	t_segmento* huecoLibre=NULL;
	int i =0;
	int j=1;
	t_segmento* segmento1=list_get(listaHuecosLibres, i);
	t_segmento* segmento2=list_get(listaHuecosLibres, j);
	while(i<=tamanioLista && j< tamanioLista){
		if(segmento1->tamanio>=nuevoSegmento->tamanio && segmento1->tamanio >= segmento2->tamanio){
			huecoLibre = segmento1;
			segmento2 = list_get(listaHuecosLibres, j++);
		}else if(segmento2->tamanio >= nuevoSegmento->tamanio){
			huecoLibre = segmento2;
			segmento1 = list_get(listaHuecosLibres, i++);
		}
		else {
			i++; j++;
		}
	}
	return huecoLibre;

}
