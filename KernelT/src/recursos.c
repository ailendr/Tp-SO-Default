/*
 * recursos.c
 *
 *  Created on: May 25, 2023
 *      Author: utnso
 */


#include "recursos.h"

t_list* listaDeBloqueo;
t_list* listaDeInstancias;

t_list* crearListaDeBloqueo(){
	char** vectorDeRecursos = Recursos(); //la funcion devuelve una array de strings con un null al final

	//---Creo las colas y voy llenando la lista---//
	t_list* listColasPorRecurso = list_create(); //creo una lista que contiene en cada posicion una cola de bloqueados para el recurso
	int i = 0;
	while(vectorDeRecursos[i] != NULL){
		t_queue* colaDeBloqueo = queue_create();
		list_add(listColasPorRecurso, colaDeBloqueo);
		i++;
	}

	return listColasPorRecurso;
	}


t_list* crearListaDeInstancias(){

	//--Casteo el vector de strings de instancias a una lista de enteros, lo pense como un vector pero alta paja el retorno--//
	char** instanciasDeRecursos = Instancias();

	t_list* listaDeInstancias = list_create();
	int i =0;
	while(instanciasDeRecursos[i] != NULL){
		char* stringCantidad = instanciasDeRecursos[i];
		int cantidad = (int) stringCantidad;
		list_add(listaDeInstancias, cantidad);
		i++;
	}
	return listaDeInstancias;
}

void crearEstructurasDeRecursos(){ //Son globales
	listaDeBloqueo = crearListaDeBloqueo();
	listaDeInstancias = crearListaDeInstancias();
}

int recursoDisponible(char* nombre){
	char** recursos = Recursos();
	int i = 0;
	while(recursos[i] != NULL){
		char* recurso = recursos[i];
		if(strcmp(recurso,nombre)==0){
			return i;
		}
	}
	return -1;
}

void fwait(int posRecurso){
	int valor = list_get(listaDeInstancias, posRecurso);
	valor --;
	if(valor<0){
		t_queue* colaDeBloqueo = list_get(listaDeBloqueo, posRecurso);
		queue_push(colaDeBloqueo, ultimoEjecutado);
		ultimoEjecutado->estadoPcb = BLOCK;
	}


}
void implementacionWait(char* nombreRecurso){
	//primero busca el recurso y su posicion//
	int posicionRecurso = recursoDisponible(nombreRecurso);
    if(posicionRecurso ==-1){
    	finalizarProceso(ultimoEjecutado);
    }
    else{ //ejecuta lo que haria un wait
    	fwait(posicionRecurso);
    	}
}

void fsignal(int posRecurso){
	int valor = list_get(listaDeInstancias, posRecurso);
	valor ++;
	t_queue* colaDeBloqueo = list_get(listaDeBloqueo, posRecurso);
	t_pcb* procesoDesbloqueado = queue_pop(colaDeBloqueo);
	//agregarAEstadoReady(procesoDesbloqueado); Lo dejo comentado porq deberia incluir a planificacion.h y como planificacion.h incluye a recursos.h se llamaran circularmente


}

void implementacionSignal(char* nombreRecurso){
	int posicionRecurso = recursoDisponible(nombreRecurso);
	    if(posicionRecurso ==-1){
	    	finalizarProceso(ultimoEjecutado);
	    }
	    else{ //ejecuta lo que haria un signal
	    	fsignal(posicionRecurso);
	    	}
}


