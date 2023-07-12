/*
 * estructuras.h
 *
 *  Created on: Apr 22, 2023
 *      Author: utnso
 */

#ifndef SRC_UTILS_ESTRUCTURAS_H_
#define SRC_UTILS_ESTRUCTURAS_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>

//Bibliotecas de sockets//
#include <sys/socket.h>
#include <signal.h>
#include <netdb.h>

#include <commons/log.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include <time.h>


typedef enum
{
	MENSAJE,
	PAQUETE,
	CONTEXTO,
	YIELD,
	CREATE_SEGMENT,
	DELETE_SEGMENT,
	EXIT,
	SET,
	MOV_IN,
	MOV_OUT,
	IO,
	F_OPEN,
	F_CLOSE,
	F_SEEK,
	F_READ,
	F_WRITE,
	F_TRUNCATE,
	WAIT,
	SIGNAL,
	COMPACTAR,
	ERROR,
	OK,
	CREAR_TABLA
}op_code;

typedef struct{
	uint32_t pid;
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
}t_contextoEjec;


typedef struct {
	op_code nombre;
	uint32_t pid;
	char* param1;
	char* param2;
	char* param3;
}t_instruccion;

typedef enum{
	NEW,
	READY,
	BLOCK,
	EXEC
	} estadoPcb;

typedef struct{
	int socketConsola;
	t_contextoEjec* contexto;
	estadoPcb estadoPcb;
	t_list* tablaSegmentos; //Me imagino que va a ser una lista de struct Segmento a futuro
	double estimadoReady;//estimadorafaga
	t_list* archAbiertos;

	struct timespec llegadaACPU;//timestamp
	struct timespec llegadaAReady;//timestamp
	double tiempoDeEspera;
	double ultimaRafagaEjecutada;
	double RR;
}t_pcb;






typedef struct{
	uint32_t PID;
	uint32_t ID;
	uint32_t base;
	uint32_t tamanio;
	uint32_t limite;
	uint32_t estaEnMemoria;
}t_segmento;

void destruirSegmento(t_segmento*);
void destruirProceso(t_pcb*);
void destruirContexto(t_contextoEjec* self);
void destruirInstruccion(char* self);
int posTablaEnLista(t_list* lista, uint32_t pid);
bool pidEnTabla(t_list* lista, uint32_t pid);
#endif /* SRC_UTILS_ESTRUCTURAS_H_ */
