/*
 * algoritmosReemplazo.c
 *
 *  Created on: Jun 18, 2023
 *      Author: utnso
 */

#include "algoritmosDeAsignacion.h"


uint32_t createSegment(t_segmento* nuevoSegmento, uint32_t tamanio){
	if(memoriaDisponible()>=tamanio ){ //Iba a validar que no pase los 16 segmentos por cada proceso pero en el tp seccion KERNEL dice que no se pedira crear un id del seg superior al arch de config
		nuevoSegmento->tamanio = tamanio;
		algAsignacion algoritmo= asignarAlgoritmo();
		int tamSegmento = nuevoSegmento->tamanio;
		int posSegLibre;
		switch(algoritmo){
		case firstFit:
			posSegLibre= FirstFit(tamSegmento);
		break;
		case bestFit:
			posSegLibre= WorstYBest(tamSegmento, listaDeSegmentos, (void*)BestFit);
		break;
		case worstFit:
			posSegLibre = WorstYBest(tamSegmento, listaDeSegmentos, (void*)WorstFit);
		}
		if(posSegLibre!=-1){
			t_segmento* segLibre = list_get(listaDeSegmentos, posSegLibre);
			nuevoSegmento->base=segLibre->base;
			nuevoSegmento->limite =nuevoSegmento->base + nuevoSegmento->tamanio;
			nuevoSegmento->estaEnMemoria=1;
			nuevoSegmento->tieneInfo=0;
			nuevoSegmento->tamanioInfo=0;

            if(nuevoSegmento->tamanio < segLibre->tamanio){

				t_segmento* segmentoLibre = malloc(sizeof(t_segmento));
				segmentoLibre= list_replace(listaDeSegmentos, posSegLibre,nuevoSegmento);
				segmentoLibre->base=nuevoSegmento->limite;
				segmentoLibre->tamanio = segLibre->tamanio - nuevoSegmento->tamanio;
				segmentoLibre->limite = segLibre->base + segLibre->tamanio;
				segmentoLibre->ID=-1; //agrego esto pero NO
				segmentoLibre->PID=-1;//idem

				//logearListaDeSegmentos("despues de asignar la pos del segLibre al segNuevo: "
				//muevo de a un lugar la pos de los segmentos desde seglibre
				//int pos = buscarPosSegmento(nuevoSegmento->ID,nuevoSegmento->PID, listaDeSegmentos);
				list_add_in_index(listaDeSegmentos,posSegLibre+1,segmentoLibre);
             }
            else{
            	list_replace_and_destroy_element(listaDeSegmentos, posSegLibre, nuevoSegmento,(void*)destruirSegmento);
            }



			//Actualizo tabla de segmentos
			int posDeTabla = posTablaEnLista(listaDeTablas,nuevoSegmento->PID);
			t_tabla* tablaDeSegmentos = list_get(listaDeTablas, posDeTabla);//Ver esto al debugguear
			list_add(tablaDeSegmentos->segmentos, nuevoSegmento);

			log_info(loggerMemoria, "PID: %d - Crear Segmento: %d - Base: %d - TAMAÑO: %d", nuevoSegmento->PID, nuevoSegmento->ID, nuevoSegmento->base, nuevoSegmento->tamanio);
			logearListaDeSegmentos("Despues de mover los segmentos");
			log_info(loggerMemoria, "Memoria disponible:%d", memoriaDisponible());
			return OK;
		}
		else {
			return COMPACTAR;
		}

	}
	else{
		log_info(loggerMemoria,"No se pudo crear segmento, no hay memoria suficiente");
		destruirSegmento(nuevoSegmento); //antes de tirar error liberamos la memoria q le reservamos
		return ERROR;

	}
}

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
	//logearListaDeSegmentos("Antes de retornar un hueco libre");
		while(i < tamanioLista){
			t_segmento* huecoFree= list_get(listaDeSegmentos, i); //rompe acá con un segundo createSegment
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
		return segmento1->tamanio > segmento2->tamanio;
}

int WorstYBest(uint32_t tamSegmento, t_list* listaDeSegmentos, bool(algoritmo)(t_segmento* seg1, t_segmento* seg2)){
	int tamanio =list_size(listaDeSegmentos);
	t_segmento* segmento;
	t_segmento* huecoFree;

	int primerHuecoLibre = huecoLibreDisponible(tamSegmento, listaDeSegmentos);//Busco el primer hueco libre en el que entre para comparar
	huecoFree=list_get(listaDeSegmentos, primerHuecoLibre);

	int pos = primerHuecoLibre; //Lo hago por el caso cuando no tenemos segmentos o no borramos ninguno todavia y asigna segmentoLibre
	for (int i=primerHuecoLibre+1; i<tamanio; i++){
		segmento=list_get(listaDeSegmentos, i);

		if(huecoLibre(segmento) && segmento->tamanio >= tamSegmento){
			if(algoritmo(segmento, huecoFree)){ //como en worst compara por >= , deja este segundo segmento como el hueco libre
				huecoFree=segmento;
				pos=i;
			}

		}
	}

	return pos;
}

/*


//Como removemos la tabla de segmentos de la lista de tablas tenemos que buscar en que posicion esta//
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
