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
			//void *list_replace(t_list*, int index, void* element);
			int pos= buscarPosSegmento(segLibre->ID, segLibre->PID,listaDeSegmentos); //Almacena cualquier cosa el id y pid del seg libre, deberia ser -1 si consideramos q la primera vez : el primer hueco libre sale del ultimo hueco libre
			//No me gusta nada como se busca el hueco libre en la lista seg porq 1) que valor le ponemos al id y pid cuando creemos ese seg libre? si le ponemos -1 luego en la lista de segmentos habra mas de uno que cumpla y 2) si no se le pone nada al id y pid se llena de basura y cambia de valor siempre entonces imposible encontrarlo en la lista d seg

			t_segmento* segmentoLibre = malloc(sizeof(t_segmento));
			segmentoLibre= list_replace(listaDeSegmentos, pos,nuevoSegmento);
			segmentoLibre->base=nuevoSegmento->limite+1;
			segmentoLibre->tamanio = segLibre->tamanio - nuevoSegmento->tamanio;
			segmentoLibre->limite = segLibre->base + segLibre->tamanio;
			segmentoLibre->ID=-1; //agrego esto pero NO
			segmentoLibre->PID=-1;//idem
			//muevo de a un lugar la pos de los segmentos desde seglibre
			actualizarListaDeSegmentos(nuevoSegmento, segmentoLibre);
			//Actualizo tabla de segmentos
			int posDeTabla = posTablaEnLista(listaDeTablas,nuevoSegmento->PID);
			t_list* tablaDeSegmentos = list_get(listaDeTablas, posDeTabla);//Ver esto al debugguear
			list_add(tablaDeSegmentos, nuevoSegmento);

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

t_segmento* FirstFit(uint32_t tamSegmento, t_list* listaHuecosLibres, int tamanioLista){
	int posicionHuecoLibre = huecoLibreDisponible(tamSegmento,listaHuecosLibres, tamanioLista);
	t_segmento* huecoLibre;
	if(posicionHuecoLibre == -1){
		huecoLibre = NULL;
	}
	else{
		huecoLibre = list_get(listaHuecosLibres, posicionHuecoLibre);
	}

	return huecoLibre;
}



int huecoLibreDisponible(uint32_t tamSegmento, t_list* listaHuecosLibres, int tamanioLista){
	int i =0;
		while(i <= tamanioLista){
			t_segmento* huecoLibre= list_get(listaHuecosLibres, i);
			if(tamSegmento <= huecoLibre->tamanio){
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
