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

t_instruccion* decode (char* instruccion, t_contextoEjec* contextoRecibido) {

	t_instruccion* nuevaInstruccion = malloc(sizeof(t_instruccion*));

	char** token = string_split(instruccion, " ");

	nuevaInstruccion->nombre = asignarNombre(token[0]);
    nuevaInstruccion->pid = contextoRecibido->pid;

	if (token[1] != NULL) {
		nuevaInstruccion->param1 = token[1];
		if (token[2] != NULL) {
			nuevaInstruccion->param2 = token[2];
			if(token[3] != NULL) {
				nuevaInstruccion->param3 = token[3];
			}
		}
	}

	op_code nombreI = nuevaInstruccion->nombre;

	if (nombreI == SET){
		sleep(retardo());
		log_info(loggerCPU, "Realizando el retardo en el SET");
	}

	char* aux;

	if (nombreI == MOV_IN || nombreI == F_READ || nombreI == F_WRITE){
		aux = nuevaInstruccion->param2;
		nuevaInstruccion->param2 = mmu(aux, contextoRecibido->pid);
	}

	if (nombreI == MOV_OUT){
		aux = nuevaInstruccion->param1;
		nuevaInstruccion->param1 = mmu(aux, contextoRecibido->pid);
	}


	log_info(loggerCPU, "Finalizado el decode");

	return nuevaInstruccion;

}

int execute (t_instruccion* instruccion, t_contextoEjec* contexto) {


	op_code nombreI = instruccion->nombre;

	if (nombreI == SET){
		set (instruccion, contexto);
	    log_info(loggerCPU, "Instruccion SET finalizada");
		return 0;
	}

	if (nombreI == MOV_IN){
		moveIn (instruccion, contexto);
		log_info(loggerCPU, "Instruccion MOVE_IN finalizada");
		return 0;
	}

	if (nombreI == MOV_OUT){
		moveOut (instruccion, contexto);
		log_info(loggerCPU, "Instruccion MOVE_OUT finalizada");
		return 0;
	}

	return 1;

}

op_code asignarNombre (char* nombre){
	op_code instruccion;
	if (strcmp(nombre, "YIELD") == 0) instruccion = YIELD;
	if (strcmp(nombre, "CREATE_SEGMENT") == 0) instruccion = CREATE_SEGMENT;
	if (strcmp(nombre, "DELETE_SEGMENT") == 0) instruccion = DELETE_SEGMENT;
	if (strcmp(nombre, "EXIT") == 0) instruccion = EXIT;
	if (strcmp(nombre, "SET") == 0) instruccion = SET;
	if (strcmp(nombre, "MOV_IN") == 0) instruccion = MOV_IN;
	if (strcmp(nombre, "MOV_OUT") == 0) instruccion = MOV_OUT;
	if (strcmp(nombre, "IO") == 0) instruccion = IO;
	if (strcmp(nombre, "F_OPEN") == 0) instruccion = F_OPEN;
	if (strcmp(nombre, "F_CLOSE") == 0) instruccion = F_CLOSE;
	if (strcmp(nombre, "F_SEEK") == 0) instruccion = F_SEEK;
	if (strcmp(nombre, "F_READ") == 0) instruccion = F_READ;
	if (strcmp(nombre, "F_WRITE") == 0) instruccion = F_WRITE;
	if (strcmp(nombre, "F_TRUNCATE") == 0) instruccion = F_TRUNCATE;
	if (strcmp(nombre, "WAIT") == 0) instruccion = WAIT;
	if (strcmp(nombre, "SIGNAL") == 0) instruccion = SIGNAL;
	return instruccion;
}


// UTILS DE LAS INSTRUCCIONES --------------------------------------------------------------
void set (t_instruccion* instruccion, t_contextoEjec* contexto){

	if (strcmp(instruccion->param1, "AX") == 0) strcpy(contexto->AX, instruccion->param2);
	if (strcmp(instruccion->param1, "BX") == 0) strcpy(contexto->BX, instruccion->param2);
	if (strcmp(instruccion->param1, "CX") == 0) strcpy(contexto->CX, instruccion->param2);
	if (strcmp(instruccion->param1, "DX") == 0) strcpy(contexto->DX, instruccion->param2);
	if (strcmp(instruccion->param1, "EAX") == 0) strcpy(contexto->EAX, instruccion->param2);
	if (strcmp(instruccion->param1, "EBX") == 0) strcpy(contexto->EBX, instruccion->param2);
	if (strcmp(instruccion->param1, "ECX") == 0) strcpy(contexto->ECX, instruccion->param2);
	if (strcmp(instruccion->param1, "EDX") == 0) strcpy(contexto->EDX, instruccion->param2);
	if (strcmp(instruccion->param1, "RAX") == 0) strcpy(contexto->RAX, instruccion->param2);
	if (strcmp(instruccion->param1, "RBX") == 0) strcpy(contexto->RBX, instruccion->param2);
	if (strcmp(instruccion->param1, "RCX") == 0) strcpy(contexto->RCX, instruccion->param2);
	if (strcmp(instruccion->param1, "RDX") == 0) strcpy(contexto->RDX, instruccion->param2);

}

void moveIn (t_instruccion* instruccion, t_contextoEjec* contexto){
	t_paquete* paqueteI;
	char* valorGuardar;

	paqueteI = serializarInstruccion(instruccion);
	validarEnvioDePaquete(paqueteI, socketMemoria, loggerCPU, configCPU, "Instruccion");

	valorGuardar = recibir_mensaje(socketMemoria);
	instruccion->param2 = valorGuardar;
	set(instruccion, contexto);

}

void moveOut (t_instruccion* instruccion, t_contextoEjec* contexto){

	if (strcmp(instruccion->param2, "AX") == 0) strcpy(instruccion->param2, contexto->AX);
	if (strcmp(instruccion->param2, "BX") == 0) strcpy(instruccion->param2, contexto->BX);
	if (strcmp(instruccion->param2, "CX") == 0) strcpy(instruccion->param2, contexto->CX);
	if (strcmp(instruccion->param2, "DX") == 0) strcpy(instruccion->param2, contexto->DX);
	if (strcmp(instruccion->param2, "EAX") == 0) strcpy(instruccion->param2, contexto->EAX);
	if (strcmp(instruccion->param2, "EBX") == 0) strcpy(instruccion->param2, contexto->EBX);
	if (strcmp(instruccion->param2, "ECX") == 0) strcpy(instruccion->param2, contexto->ECX);
	if (strcmp(instruccion->param2, "EDX") == 0) strcpy(instruccion->param2, contexto->EDX);
	if (strcmp(instruccion->param2, "RAX") == 0) strcpy(instruccion->param2, contexto->RAX);
	if (strcmp(instruccion->param2, "RBX") == 0) strcpy(instruccion->param2, contexto->RBX);
	if (strcmp(instruccion->param2, "RCX") == 0) strcpy(instruccion->param2, contexto->RCX);
	if (strcmp(instruccion->param2, "RDX") == 0) strcpy(instruccion->param2, contexto->RDX);

	t_paquete* paqueteI;

	paqueteI = serializarInstruccion(instruccion);
	validarEnvioDePaquete(paqueteI, socketMemoria, loggerCPU, configCPU, "Instruccion");
	//TODO VALIDACION
}
