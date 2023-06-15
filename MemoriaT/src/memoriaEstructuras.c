/*
 * memoriaEstructuras.c
 *
 *  Created on: Jun 10, 2023
 *      Author: utnso
 */
#include "memoriaEstructuras.h"
/*
void* memoriaContigua; //GLOBAL
t_segmento* segmentoCero;
t_list * listaDeTablas;
void crearEspacioMemoria (){
	int tamanioMemoria = tam_memoria();
	memoriaContigua = malloc(tamanioMemoria);
}

void crearSegmentoCero(){
	segmentoCero = malloc(sizeof(t_segmento));
	segmentoCero->ID=0;
	segmentoCero->base=0;
	segmentoCero->limite = tam_segmento();

}*/

////NO VI QUE ESTABA CREADA ESTA RAMA Y EMPECE EN OTRA JAJAJA, LO DEJO PARA QUE CHARLEMOS LO DE SEGMENTOCERO
/*
void iniciarMemoria(){
uint32_t opTamSegmento= HANDSHAKE_TamSegmento;
int tamSegmentoMaxCpu;
int tamSegmento= tam_segmento_cero();
//Este send y recv lo dejo así para tener la idea, pero creo que debería estar en MemoriaConexion
//Igual esto va una vez que se levantan todas las conexiones, así que no estoy segura
send(socketCpu,&opTamSegmento,sizeof(uint32_t),0 );
recv(socketCpu, &tamSegmentoMaxCpu, sizeof(uint32_t), MSG_WAITALL);
if(tamSegmento<= tamSegmentoMaxCpu){
	segmentoCero = malloc(tamMemoria);
}else{
	segmentoCero=malloc(tamSegmentoMaxCpu);
}
}*/
/*
void crearListaDeTablas(){
listaDeTablas = list_create();

}

t_list* crearTablaDeSegmento(uint32_t pid){

	t_list* tablaDeSegmentos = list_create();
	list_add(tablaDeSegmentos, segmentoCero);
	list_add_in_index(tablaDeSegmentos,pid, tablaDeSegmentos);
	return tablaDeSegmentos;
	 }

void iniciarEstructuras(){
	crearEspacioMemoria();
	crearSegmentoCero();


}*/

//Hacer una funcion crearSegmento que la invocamos cuando recibamos el op_code CreateSegment


void* memoriaContigua; //GLOBAL
t_segmento* segmentoCero;
//Agregar los externs de estas variables en el .h
//int memoriaOcupada=0;
t_list* listaDeSegmentos;
t_list* listaHuecosLibres;
t_list* listaDeTablas;

//Deberia tener un struct tablaDeSegmentos. Esto moverlo a static-> estructuras.h

void crearEspacioMemoria (){
	int tamanioMemoria = tam_memoria();
	memoriaContigua = malloc(tamanioMemoria);
}

void crearSegmentoCero(){
	segmentoCero = malloc(sizeof(t_segmento));
	segmentoCero->ID=0;
	segmentoCero->base=0; //Si le dejo esto, significa que en memoria van a apuntar todos los procesos al mismo segmentoCero. No debería ser que es el segmentoCero de igual tam para todos, pero tienen =! base?
	segmentoCero->limite = tam_segmento_cero();

}

void crearListas(){
	listaDeTablas = list_create();
	listaDeSegmentos = list_create();
	listaHuecosLibres = list_create();

}

void iniciarEstructuras(){
	crearEspacioMemoria();
	crearSegmentoCero();
}

int tamanioSegmento(t_segmento* segmento){
	int base = segmento->base;
	int limite = segmento->limite;
	int tamanio = base + limite;
	return tamanio;
}

t_tablaDeSegmentos* crearTablaDeSegmentos(uint32_t pid){
	t_tablaDeSegmentos* tablaDeSegmentos;
	tablaDeSegmentos->segmentos=list_create();
	tablaDeSegmentos->pid = pid;
	list_add(tablaDeSegmentos->segmentos, segmentoCero);
	list_add(listaDeSegmentos, tablaDeSegmentos);
	//memoriaOcupada+= tamanioSegmento(segmentoCero);
	return tablaDeSegmentos;
	}

int memoriaOcupada(){
	int tamLista=list_size(listaDeSegmentos);
	int memoriaOcupada = 0;
	for(int i =0; i<tamLista;i++){
	t_segmento* segmento = list_get(listaDeSegmentos, i);
	memoriaOcupada+= tamanioSegmento(segmento);
	}
	return memoriaOcupada;
}

int memoriaDisponible(){
	int tamMemoria = tam_memoria();
	//int memoriaOcupada = memoriaOcupada();
	int memoriaDis = tamMemoria - memoriaOcupada();
	return memoriaDis;
}

void liberarTablaDeSegmentos(uint32_t pid){
	t_tablaDeSegmentos* tablaALiberar=buscarTabla(pid);
	free(tablaALiberar->segmentos);
	list_remove(listaDeTablas, tablaALiberar);
	log_info(loggerMemoria, "Finalizo el proceso: %d", pid);
}

t_tablaDeSegmentos* buscarTabla(uint32_t pid){
	int tamLista = list_size(listaDeTablas);
	int i=0;
	t_tablaDeSegmentos* tabla = list_get(listaDeTablas, i);
	while(i<=tamLista && tabla->pid != pid){
		i++;
	tabla=list_get(listaDeTablas, i);
	}
	return tabla;
}
/*
//O seria que llega el id del segmento?
void deleteSegment(t_segmento* segmentoAEliminar, uint32_t pid){
	t_list* tablaDeSegmentos = buscarTabla(pid);
	int idSegmento = segmentoAEliminar->id;
	tablaDeSegmentos->segmentos=
}*/



/*
void crearSegmento(){
	if(memoriaDisponible()>=tamanioSegmento()){
	//Aca tendría que hacer lo de aplicar el agoritmo de firstfit, bla bla
}*/

void firstFIT(t_segmento* nuevoSegmento){
	int i = 0;
	t_segmento* segmentoAnterior;
	t_segmento* segmento=list_get(listaDeSegmentos, i);
	while(segmento!=NULL){
		segmentoAnterior=segmento;
		i++;
	}
	nuevoSegmento->base = segmentoAnterior->limite +1;
	list_add_in_index(listaDeSegmentos,i, nuevoSegmento);
}
