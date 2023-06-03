/*
 * instruccion.c
 *
 *  Created on: May 15, 2023
 *      Author: utnso
 */
#include "instruccion.h"

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

