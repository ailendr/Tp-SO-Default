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
	    char AX[4];
	    char BX[4];
	    char CX[4];
	    char DX[4];
	    char EAX[8];
	    char EBX[8];
	    char ECX[8];
	    char EDX[8];
	    char RAX[16];
	    char RBX[16];
	    char RCX[16];
	    char RDX[16];
	uint32_t proxInstruccion;
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
