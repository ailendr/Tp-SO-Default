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
void crearEspacioMemoria (){
	int tamanioMemoria = tam_memoria();
	memoriaContigua = malloc(tamanioMemoria);
}

void crearSegmentoCero(){
	segmentoCero = malloc(sizeof(t_segmento));
	segmentoCero->ID=0;
	segmentoCero->base=0;
	segmentoCero->limite = tam_segmento();

}

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


}

//Hacer una funcion crearSegmento que la invocamos cuando recibamos el op_code CreateSegment

