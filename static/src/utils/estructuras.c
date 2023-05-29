/*
 * estructuras.c
 *
 *  Created on: Apr 28, 2023
 *      Author: utnso
 */
#include "estructuras.h"

 t_pcb* crearPcb (uint32_t pid,uint32_t estado){
	    t_pcb* pcb = malloc(sizeof(t_pcb));
	 	pcb->PID = pid;
	 //	crearContexto(); Deberiamos crear una funcion para esto
	 	pcb->tablaSegmentos = list_create();//recibir por parametro
	 	pcb->estadoPcb = estado;
	 	pcb->estimadoReady = 0;
	 	pcb->llegadaARedy = 0;
	 	pcb->archAbiertos = list_create();//recibir por parametro
	 	return pcb;
}

t_contextoEjec* crearContexto() {}	//TODO

