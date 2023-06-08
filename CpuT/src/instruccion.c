/*
 * instruccion.c
 *
 *  Created on: May 15, 2023
 *      Author: utnso
 */
#include "instruccion.h"

// INTERPRETACION DE LAS INSTRUCCIONES -----------------------------------------------------

char* fetch (t_contextoEjec* cont) {

	char* proxInstr = list_get(cont->instrucciones, cont->PC);
    log_info(loggerCPU, "FETCH: PCB <ID %d>", cont->pid);
    log_info(loggerCPU, "   -> Instruccion: %s", proxInstr);
    cont->PC += 1;
    log_info(loggerCPU, "   -> Program Counter: %d", cont->PC);

    return proxInstr;
}

t_instruccion* decode (char* instruccion) {

	t_instruccion* nuevaInstruccion;

	char** token = string_split(instruccion, " ");

	nuevaInstruccion->nombre = asignarNombre (token[0]);

	if (token[1] != NULL) {
		nuevaInstruccion->param1 = token[1];
		if (token[2] != NULL) {
			nuevaInstruccion->param2 = token[2];
			if(token[3] != NULL) {
				nuevaInstruccion->param3 = token[3];
			}
		}
	}

	if (nuevaInstruccion->nombre == SET){
		sleep(retardo());
		log_info(loggerCPU, "Entre en el SET");
	}

	if (nuevaInstruccion->nombre == MOV_IN){
		//TODO traduccion
	}

	if (nuevaInstruccion->nombre == MOV_OUT){
		//TODO traduccion
	}


	log_info(loggerCPU, "Finalizado el decode");

	return nuevaInstruccion;

}

void execute (t_instruccion* instruccion, t_contextoEjec* contexto) {

	switch (instruccion->nombre){
		case SET:
			set (instruccion, contexto);
		    log_info(loggerCPU, "Recibio un SET");
			break;
		case MOV_IN:
			//TODO lo que se deba, la traduccion se hace, borrarlo aca
			break;
		case MOV_OUT:
			//TODO lo que se deba, la traduccion se hace, borrarlo aca
			break;
		default:
			log_error(loggerCPU, "Error con instruccion");
	}

}

op_code asignarNombre (char* nombre){
	if (strcmp(nombre, "YIELD")) return YIELD;
	if (strcmp(nombre, "CREATE_SEGMENT")) return CREATE_SEGMENT;
	if (strcmp(nombre, "DELETE_SEGMENT")) return DELETE_SEGMENT;
	if (strcmp(nombre, "EXIT")) return EXIT;
	if (strcmp(nombre, "SET")) return SET;
	if (strcmp(nombre, "MOV_IN")) return MOV_IN;
	if (strcmp(nombre, "MOV_OUT")) return MOV_OUT;
	if (strcmp(nombre, "IO")) return IO;
	if (strcmp(nombre, "F_OPEN")) return F_OPEN;
	if (strcmp(nombre, "F_CLOSE")) return F_CLOSE;
	if (strcmp(nombre, "F_SEEK")) return F_SEEK;
	if (strcmp(nombre, "F_READ")) return F_READ;
	if (strcmp(nombre, "F_WRITE")) return F_WRITE;
	if (strcmp(nombre, "F_TRUNCATE")) return F_TRUNCATE;
	if (strcmp(nombre, "WAIT")) return WAIT;
	if (strcmp(nombre, "SIGNAL")) return SIGNAL;
}


// UTILS DE LAS INSTRUCCIONES --------------------------------------------------------------
void set (t_instruccion* instruccion, t_contextoEjec* contexto){

	if (strcmp(instruccion->param1, "AX")) strcpy(contexto->AX, instruccion->param2);
	if (strcmp(instruccion->param1, "BX")) strcpy(contexto->BX, instruccion->param2);
	if (strcmp(instruccion->param1, "CX")) strcpy(contexto->CX, instruccion->param2);
	if (strcmp(instruccion->param1, "DX")) strcpy(contexto->DX, instruccion->param2);
	if (strcmp(instruccion->param1, "EAX")) strcpy(contexto->EAX, instruccion->param2);
	if (strcmp(instruccion->param1, "EBX")) strcpy(contexto->EBX, instruccion->param2);
	if (strcmp(instruccion->param1, "ECX")) strcpy(contexto->ECX, instruccion->param2);
	if (strcmp(instruccion->param1, "EDX")) strcpy(contexto->EDX, instruccion->param2);
	if (strcmp(instruccion->param1, "RAX")) strcpy(contexto->RAX, instruccion->param2);
	if (strcmp(instruccion->param1, "RBX")) strcpy(contexto->RBX, instruccion->param2);
	if (strcmp(instruccion->param1, "RCX")) strcpy(contexto->RCX, instruccion->param2);
	if (strcmp(instruccion->param1, "RDX")) strcpy(contexto->RDX, instruccion->param2);

}


