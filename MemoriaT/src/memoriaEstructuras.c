/*
 * memoriaEstructuras.c
 *
 *  Created on: Jun 10, 2023
 *      Author: utnso
 */
#include "memoriaEstructuras.h"

void* memoriaContigua; //GLOBAL
t_segmento* segmentoCero;
void crearEspacioMemoria (){
	int tamanioMemoria = tam_memoria();
	memoriaContigua = malloc(tamanioMemoria);

	//toy quemada pero creo q maso seria asi//
	segmentoCero = malloc(sizeof(t_segmento));
	segmentoCero->ID=0;
	segmentoCero->base=0;
	segmentoCero->limite = tam_segmento();

//Para mi deberiamos llenar la memoria solo con los tamaños de los segmentos porq no se si la cant de bytes q dan es para almacenar IDS , uint32_t de la base y etc
/*	 int offset=0; o genericamente offset = segmento->base
	 memcpy(memoriaContigua+offset,segmentoCero, sizeof(t_segmento));*/

}

////NO VI QUE ESTABA CREADA ESTA RAMA Y EMPECE EN OTRA JAJAJA, LO DEJO PARA QUE CHARLEMOS LO DE SEGMENTOCERO
/*
void iniciarMemoria(){
uint32_t opTamSegmento= HANDSHAKE_TamSegmento;
int tamSegmentoMaxCpu;
int tamMemoria= tam_memoria();
//Este send y recv lo dejo así para tener la idea, pero creo que debería estar en MemoriaConexion
//Igual esto va una vez que se levantan todas las conexiones, así que no estoy segura
send(socketCpu,&opTamSegmento,sizeof(uint32_t),0 );
recv(socketCpu, &tamSegmentoMaxCpu, sizeof(uint32_t), MSG_WAITALL);
if(tamMemoria<= tamSegmentoMaxCpu){
	segmentoCero = malloc(tamMemoria);
}else{
	segmentoCero=malloc(tamSegmentoMaxCpu);
}
}*/

void crearTablasDeSegmentos(){
	//Preguntar si cada tabla se crea a medida q kernel acepta procesos o directamente la cant de tablas sale del gr de multiprog q acepta la memoria
	int i;
	for(i = 0; i< 4/* multiProgramacion*/; i++){
	t_list* tablaDeSegmentos = list_create();
	list_add(tablaDeSegmentos,i); //me gustaria q la tabla tenga un identiicador por PID
	list_add(tablaDeSegmentos, segmentoCero);
	 for(int j=1; j<cantSegmentos()+1; j++){ //asi arranca creando el segmento 1 para cada proceso
	//Preguntar si inicialmente le crea a cada proceso los 16 segmentos sin tamaño o solo le da una tabla con el segmentoCero
		 t_segmento* segmento = malloc(sizeof(t_segmento));
		 segmento->ID = j;
		 list_add(tablaDeSegmentos, segmento);
	 }
	}
}
void iniciarEstructuras(){
	crearEspacioMemoria();
	crearTablasDeSegmentos();


}
