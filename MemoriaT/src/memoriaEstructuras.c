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
t_list* listaDeSegmentos;

/*Es para tener un hueco libre al principio cuando no hay segmentos y para cuando se compacte y queda contiguo el espacio libre*/
t_segmento* segmentoLibre;
pthread_mutex_t mutexEspacioUser;

///////////////////////////// ESTRUCTURAS DE MEMORIA/////////////////////////////////////////////////////////

void crearListas(){
	listaDeTablas = list_create();
	listaDeSegmentos = list_create();

}

void iniciarEstructuras(){
	crearListas();
	crearEspacioMemoria();
	crearSegmentoCero();
	actualizarUltimoSegmentoLibre();
	pthread_mutex_init(&mutexEspacioUser, NULL);
}

void crearEspacioMemoria (){
	int tamanioMemoria = tam_memoria();
	memoriaContigua = malloc(tamanioMemoria);
}

void crearSegmentoCero(){
	segmentoCero = malloc(sizeof(t_segmento));
	segmentoCero->ID=0;
	segmentoCero->base=0;
	segmentoCero->limite = tam_segmento_cero();//Es lo mismo que hacer base + tamanio en este caso
	segmentoCero->tamanio=tam_segmento_cero();
	segmentoCero->estaEnMemoria=1;
	segmentoCero->tieneInfo=0;
	segmentoCero->tamanioInfo=0;
	list_add(listaDeSegmentos, segmentoCero);

}

//////////////////////////UTILS DE MEMORIA/////////////////////////////////////////////

int memoriaOcupada(t_list* lista){
	int tamLista=list_size(lista);
	int memoriaOcupada = 0;
	for(int i =0; i<tamLista;i++){
	t_segmento* segmento = list_get(lista, i);
	memoriaOcupada+= segmento->tamanio;
	}
	return memoriaOcupada;
}

int memoriaDisponible(){
	t_list* listaHuecosOcupados = list_filter(listaDeSegmentos, (void*)segmentoOcupado);
	int tamanioHuecosOcupados = memoriaOcupada(listaHuecosOcupados);
	int tamMemoria = tam_memoria();
	int memoriaDis = tamMemoria - tamanioHuecosOcupados;
	return memoriaDis;
}


int  buscarPosSegmento(uint32_t idSegmento, uint32_t pid, t_list* lista){
	int tamLista = list_size(lista);
	int i = 0;

	while(i<tamLista){
  		t_segmento* segmento=list_get(lista, i);
		if(segmento->ID==idSegmento && segmento->PID==pid){
			return i;
		}
		i++;
 	}
	return -1;
}

bool huecoLibre(t_segmento* segmento){
	return segmento->estaEnMemoria ==0;
}

bool segmentoOcupado(t_segmento* segmento){
	return segmento->estaEnMemoria ==1;
}


void actualizarUltimoSegmentoLibre(){
	segmentoLibre = malloc(sizeof(t_segmento));
	int ultimaPos=list_size(listaDeSegmentos)-1;
	t_segmento* ultimoSegmento = list_get(listaDeSegmentos, ultimaPos);
	segmentoLibre->ID=-1;
	segmentoLibre->PID=-1;
	segmentoLibre->base = ultimoSegmento->limite;
	segmentoLibre->tamanio=tam_memoria() - memoriaOcupada(listaDeSegmentos);
	segmentoLibre->limite=tam_memoria();
	segmentoLibre->estaEnMemoria=0;
	segmentoLibre->tieneInfo = 0;
	segmentoLibre->tamanioInfo=0;
	list_add(listaDeSegmentos,segmentoLibre);
}


void unirHuecosAledanios(t_segmento* segmento, int pos){
	int posAnterior = pos -1;
	int posSiguiente = pos + 1;
	t_segmento* segmentoSiguiente;
	t_segmento* segmentoAnterior =list_get(listaDeSegmentos, posAnterior);
	int tamLista = list_size(listaDeSegmentos);

	if(posSiguiente!=tamLista){
	segmentoSiguiente = list_get(listaDeSegmentos, posSiguiente);

	 if(huecoLibre(segmentoSiguiente)){
			 segmento->tamanio += segmentoSiguiente->tamanio;
			 segmento->limite=segmentoSiguiente->limite;
			 list_remove_and_destroy_element(listaDeSegmentos,posSiguiente,(void*)destruirSegmento);
		 }
	}

	 if(huecoLibre(segmentoAnterior)){
		 segmento->base=segmentoAnterior->base;
		 segmento->tamanio = segmento->tamanio + segmentoAnterior->tamanio;
		 list_remove_and_destroy_element(listaDeSegmentos,pos-1,(void*)destruirSegmento);
		 //posSiguiente = pos;
	 }

	 /*if(huecoLibre(segmentoSiguiente)){
		 segmento->tamanio += segmentoSiguiente->tamanio;
		 segmento->limite=segmentoSiguiente->limite;
		 list_remove_and_destroy_element(listaDeSegmentos,posSiguiente,(void*)destruirSegmento);
	 }*/
}

void logearListaDeSegmentos(char* mensaje){
	int tamLista = list_size(listaDeSegmentos);
	log_info(loggerMemoria, "Lista de segmentos %s: ", mensaje);
	for(int i =0; i<tamLista; i++){
		t_segmento* segmento = list_get(listaDeSegmentos, i);
		log_info(loggerMemoria, "PID: %d - Segmento: %d - Base: %d - Tamaño %d- En Memoria: %d", segmento->PID,segmento->ID, segmento->base, segmento->tamanio, segmento->estaEnMemoria);
		log_info(loggerMemoria, "Pos en la lista: %d", i);
	}
}


////////////////////////////////FUNCIONES DE MEMORIA///////////////////////////////

t_tabla* crearTablaDeSegmentos(uint32_t pid){

	t_tabla* tablaDeSegmentos = malloc(sizeof(t_tabla));
	tablaDeSegmentos->segmentos = list_create();
    tablaDeSegmentos->PID = pid;
	//log_info(loggerMemoria,"el pid recuperado es : %d",*identificador);
	list_add(tablaDeSegmentos->segmentos, segmentoCero);
	list_add_in_index(listaDeTablas,pid, tablaDeSegmentos);
	log_info(loggerMemoria, "Creacion de proceso: %d", pid);
	return tablaDeSegmentos;
}

void liberarTablaDeSegmentos(uint32_t pid){
	int posDeTabla = posTablaEnLista(listaDeTablas,pid);
	if(posDeTabla != -1){
	t_tabla* tablaALiberar= list_get(listaDeTablas, posDeTabla);
	int tamTabla = list_size(tablaALiberar->segmentos);
	//Marco como libres a los segmentos del proceso
	for(int i=1;i<tamTabla;i++){
		t_segmento* segmentoEnTabla= list_get(tablaALiberar->segmentos, i);
		deleteSegment(segmentoEnTabla->ID, pid); //1)opcion: si dentro de delete segment lo borramos de la tabla -> decrementamos porq la tabla se va achicando
		i--;                                     //2)opcion : no removemos ni destruimos el seg de la tabla sino que le podemos un id basura y lo marcamos como libre
		tamTabla--;
		//int pos = buscarPosSegmento(segmentoEnTabla->ID, pid, listaDeSegmentos);
		//t_segmento* segmentoEnLista= list_get(listaDeSegmentos, pos);
       // segmentoEnLista->estaEnMemoria=0;
		//list_replace(listaDeSegmentos, pos, segmento); //NO USAR REPLACE PARA ACTUALIZAR PORQUE GENERA INCONSISTENCIA: DEJAR LO DE ARRIBA
	}
	list_destroy(tablaALiberar->segmentos);
	list_remove(listaDeTablas,posDeTabla);//Destruirmos esa Tabla de Segmentos de la Lista de Tablas
	log_info(loggerMemoria, "Eliminación de proceso: %d", pid);
	}
	else{
		log_info(loggerMemoria, "No se encontro la tabla del proceso a elimina");}
}

t_tabla* deleteSegment(uint32_t id, uint32_t pid) { //Me sirve que retorne la tabla actualizada
	//Se pone el segmento como libre en la Lista de Segmentos
	int pos = buscarPosSegmento(id, pid ,listaDeSegmentos);
	int posDeTabla = posTablaEnLista(listaDeTablas,pid);//Primero se busca la tabla en la lista global de tablas
	t_tabla* tablaDeSegmentosAActualizar = list_get(listaDeTablas,posDeTabla);

	if(pos!= -1){
	t_segmento* segmentoAEliminar = list_get(listaDeSegmentos,pos);

	t_segmento* segAux = malloc(sizeof(t_segmento));
	segAux->ID = -1;
	segAux->PID=-1;
	segAux->base = segmentoAEliminar->base;
	segAux->tamanio = segmentoAEliminar->tamanio;
	segAux->limite=segmentoAEliminar->limite;
	segAux->estaEnMemoria = 0;
	segAux->tieneInfo=0;
	segAux->tamanioInfo=0;
	//Ver si eliminamos la info en memoria Contigua correspondiente a ese Segmento//

	list_remove(listaDeSegmentos,pos);
	//free(seg);
	//logearListaDeSegmentos("la lista de seg cuando lo remuevo al segAEliminar");

	list_add_in_index(listaDeSegmentos, pos, segAux);
	//logearListaDeSegmentos("la lista de seg cuando agrego segAux");
	//Actualizo la tabla de segmentos del proceso: Eliminando ese segmento de la tabla
	//Se busca el segmento en la Tabla de segmentos
	t_list* segmentos = tablaDeSegmentosAActualizar->segmentos;
	int posSegEnTabla = buscarPosSegmento(id, pid, segmentos);
	log_info(loggerMemoria,"Eliminación de Segmento: “PID: %d - Eliminar Segmento: %d - Base: %d - TAMAÑO: %d",pid,id,segmentoAEliminar->base, segmentoAEliminar->tamanio  );
	list_remove_and_destroy_element(segmentos,posSegEnTabla,(void*)destruirSegmento);

	//Busco si los huecos aledaños estan libres para consolidar
	unirHuecosAledanios(segAux, pos);
	//logearListaDeSegmentos("La lista de seg cuando se elimina un segmento de la tabla");
	}
    return tablaDeSegmentosAActualizar;
}

void compactar() {
	log_info(loggerMemoria,"Solicitud de Compactación");
   // usleep(retardoCompactacion()*1000);
	logearListaDeSegmentos("Antes de compactar");
	t_list* listaAux=list_filter(listaDeSegmentos, (void*)segmentoOcupado);//creo una lista aux solo con los segmentos ocupados
	list_clean(listaDeSegmentos);//Es mejor destruir los elementos tambien por si queda algo
	list_add(listaDeSegmentos, segmentoCero);
	int tamanioLista=list_size(listaAux);

	//voy actualizando los segmentos y los vuelvo a cargar en la lista de segmentos
	for(int i=1; i<tamanioLista; i++){//Lo inicio en 1 porque segmentoCero siempre tiene los mismos valores
		t_segmento* segmento = list_get(listaAux, i);
		t_segmento* segAnterior = list_get(listaAux, i-1);
		if(segmento->tieneInfo){
			char* info = malloc(segmento->tamanioInfo);
			memcpy(&info, memoriaContigua+segmento->base,segmento->tamanioInfo);
			memcpy(memoriaContigua+segAnterior->limite, &info, segmento->tamanioInfo);
			free(info);
			}

		segmento->base = segAnterior->limite;
		segmento->limite = segmento->base + segmento->tamanio;
		list_add(listaDeSegmentos, segmento);

	}
	actualizarUltimoSegmentoLibre();
	list_destroy(listaAux);
	logearListaDeSegmentos("Despues de compactar");

	//Solo loggeamos la tabla porque los segmentos ya estan actualizados desde la listaDeSegmentos: SACAR A FUTURO///
	int tamListaTablas = list_size(listaDeTablas);
	int j=0;
	while(j<tamListaTablas){
		t_tabla* tablaDeSegmentos = list_get(listaDeTablas, j);
		if(!list_is_empty(tablaDeSegmentos->segmentos)){
         loggearTablaDeSegmentos(tablaDeSegmentos, loggerMemoria);
		}
		j++;
	}

}


void validarNumSegmento(int numSeg, int socket){
    int valorOp;
	if(numSeg >= cantSegmentos()){
	valorOp= ERROR;
    send(socket, &valorOp, sizeof(int), 0);
	}
	else{
		valorOp = OK;
	    send(socket, &valorOp, sizeof(int), 0);
	}
}

void implementarInstruccion(char* direcF, uint32_t pid,char* registro,int socket, op_code operacion, int bytes){
	char** direccionFisica = string_split(direcF, " ");
	char* numSeg = direccionFisica[1];
	char* desplazamiento = direccionFisica[2];
	int numSegmento= atoi(numSeg);
	int offset = atoi(desplazamiento);
	int posDeTabla = posTablaEnLista(listaDeTablas, pid);
	t_tabla* tabla = list_get(listaDeTablas, posDeTabla);
	int posSeg = buscarPosSegmento(numSegmento, pid, tabla->segmentos);
	t_segmento* segmento = list_get(tabla->segmentos, posSeg);
    usleep(retardoMemoria()*1000);
		if(operacion == MOV_IN || F_WRITE){
			pthread_mutex_lock(&mutexEspacioUser);
			memcpy(&registro, memoriaContigua + segmento->base + offset, bytes); //Comprobado que si pisa lo que habia antiguamente en registro :))
			pthread_mutex_unlock(&mutexEspacioUser);
			enviar_mensaje(registro, socket);
		}

	 if(operacion == MOV_OUT ||operacion == F_READ){
			pthread_mutex_lock(&mutexEspacioUser);
			memcpy(memoriaContigua + segmento->base+ offset, &registro, strlen(registro)); //Ver si habria que agregar un +1
			pthread_mutex_unlock(&mutexEspacioUser);

			escribirMemoria( segmento, strlen(registro));
		}

}


void escribirMemoria(t_segmento* segmento, int tamInfo){
	segmento->tieneInfo = 1;
	segmento->tamanioInfo = tamInfo;
}

//// Func para probar compactacion//

void escribir(t_segmento* segmento){
	char* info = "Aprobamos SO";
	memcpy(memoriaContigua+segmento->base, &info, strlen(info));

}

