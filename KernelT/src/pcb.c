/*
 * pcb.c
 *
 *  Created on: May 2, 2023
 *      Author: utnso
 */
#include "pcb.h"

t_contextoEjec* crearContexto(t_list* instrucciones, uint32_t pidDelContexto){
   	t_contextoEjec* contexto = malloc(sizeof(t_contextoEjec));
   	contexto->instrucciones=instrucciones;
   	contexto->PC = 0;
   	contexto->pid = pidDelContexto;
   	//TODO Al parecer no sé como inicializar vectores :)))
   /*	contexto->AX[4] = NULL;
   	contexto->BX[4]= "000";
   	contexto->CX[4]= NULL;
   	contexto->DX[4]= NULL;
   	contexto->EAX[8]= NULL;
    contexto->ECX[8]= NULL;
    contexto->EDX[8]= NULL;
    contexto->RAX[16]= NULL;
    contexto->RBX[16]= NULL;
    contexto->RCX[16]= NULL;
    contexto->RDX[16]= NULL;
    */
   	return contexto;
   }

  t_pcb* crearPcb (t_list* instrucciones, uint32_t pidNuevo, int conexionConsola){
  	    t_pcb* pcb = malloc(sizeof(t_pcb));
  	    pcb->socketConsola = conexionConsola;
  	 	pcb->contexto = crearContexto(instrucciones, pidNuevo);
  	 //	pcb->tablaSegmentos = es una t_tabla q se le asigna en largo plazo cuando pide memoria;//
  	 //	pcb->estadoPcb = estado; El estado se pone en agregarANew
  	 	pcb->estimadoReady = Estimacion(); //debe ser un globals
  	 	pcb->archAbiertos = list_create();//recibir por parametro
  	 	pcb->RR=0;
  	 	pcb->tiempoDeEspera=0;
  	 	pcb->ultimaRafagaEjecutada=0;
  	 	return pcb;
  }

