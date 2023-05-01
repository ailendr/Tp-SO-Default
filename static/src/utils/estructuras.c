/*
 * estructuras.c
 *
 *  Created on: Apr 28, 2023
 *      Author: utnso
 */
#include "estructuras.h"

 t_pcb* crearPcb (uint32_t pid, estadoPcb estado, t_list* instrucciones){
	    t_pcb* pcb = malloc(sizeof(t_pcb));
	    t_contextoEjec* contexto = malloc(sizeof(t_contextoEjec*));
	 	pcb->PID = pid;
	 	contexto=crearContexto(instrucciones);
	 	pcb->tablaSegmentos = list_create();//recibir por parametro
	 	pcb->estadoPcb = estado;
	 	pcb->estimadoReady = 0;
	 	pcb->llegadaAReady = 0;
	 	pcb->archAbiertos = list_create();//recibir por parametro
	 	return pcb;
}

t_contextoEjec* crearContexto(t_list* instrucciones) {
	t_contextoEjec* contexto;
	contexto->instrucciones=instrucciones;
	contexto->PC=0;
	return contexto;
}

