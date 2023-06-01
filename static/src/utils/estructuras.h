/*
 * estructuras.h
 *
 *  Created on: Apr 22, 2023
 *      Author: utnso
 */

#ifndef SRC_UTILS_ESTRUCTURAS_H_
#define SRC_UTILS_ESTRUCTURAS_H_
#include <stdlib.h>
#include <stdint.h>
#include <commons/collections/list.h>
#include <time.h>

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
	uint32_t PID;
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

/* Por lo que tengo entendido esto no lo tendrian asi que lo deje como estaba antes de mis quilombos
typedef enum{
    F_READ,
    F_WRITE,

    SET,
    MOV_IN,
    MOV_OUT,
    F_TRUNCATE,
    F_SEEK,
    CREATE_SEGMENT,

    IO,
    WAIT,
    SIGNAL,
    F_OPEN,
    F_CLOSE,
    DELETE_SEGMENT,

    EXIT,
    YIELD
}t_instruc;
*/

/*
Lo dejo así ya queda a futuro, pero por ahora que quede comentado
typedef struct{
	uint32_t ID;
	uint32_t tamSegmento;
	char* dirBase;
}segmento;
 * */
t_pcb* crearPcb(uint32_t pid, uint32_t estado);

t_contextoEjec* crearContexto(); //TODO
#endif /* SRC_UTILS_ESTRUCTURAS_H_ */
