/*
 * estructuras.h
 *
 *  Created on: Apr 22, 2023
 *      Author: utnso
 */

#ifndef SRC_UTILS_ESTRUCTURAS_H_
#define SRC_UTILS_ESTRUCTURAS_H_

#include <stdint.h>

typedef struct{
	t_list* instrucciones;
	uint32_t PC;
	t_list registrosCPU; //Lo dejo con list porque no sé todavía la estructura
}contextoEjec;

typedef struct{
	uint32_t PID;
	contextoEjec contexto;
	t_list* tablaSegmentos; //Me imagino que va a ser una lista de struct Segmento a futuro
	uint32_t estimadoReady;
	uint32_t llegadaARedy;
	t_list* archAbiertos;
}pcb;


/*
Lo dejo así ya queda a futuro, pero por ahora que quede comentado
typedef struct{
	uint32_t ID;
	uint32_t tamSegmento;
	char* dirBase;
}segemento;
 * */

#endif /* SRC_UTILS_ESTRUCTURAS_H_ */
