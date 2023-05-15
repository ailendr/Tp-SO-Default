/*
 * utilsCpu.c
 *
 *  Created on: May 13, 2023
 *      Author: utnso
 */

#include "./utilsCpu.h"

int iniciarCpu (){

	servidorCpu = 0;
	socketMemoria = 0;
	cliente = 0;

	loggerCPU = log_create("cpu.log", "CPU", 1, LOG_LEVEL_DEBUG);
	log_info(loggerCPU, "---------------------------------------------------------------------------");
	log_info(loggerCPU, "Iniciando CPU...");
	log_info(loggerCPU, "Ok -> Logger");

	configCPU = config_create("../CpuT/cpu.config");
	if(verificarConfig (servidorCpu, loggerCPU, configCPU) == 1 ) {
		close (socketMemoria);
		close(cliente);
		return 1;
	}
	printf ("\n El valor recuperado de la ip es %s con el puerto %s\n", IP_Escucha(), puertoEscucha());
	log_info(loggerCPU, "Ok -> Config");

	return iniciarSocketsCpu();

}

char* fetch (t_contextoEjec* cont) {

    char* proxInstr = list_get(cont->instrucciones, cont->PC);
    log_info(loggerCPU, "FETCH: PCB <ID %d>", cont->pid);
    log_info(loggerCPU, "Instruccion: %s", proxInstr);
    cont->PC+=1;

    //REVISAR

    return proxInstr;
}

t_instruccion* decode (char* instruccion) {

	t_instruccion* nuevaInstruccion;

	//Separa instruccion y guardarlo

	if (nuevaInstruccion->nombre == SET){
		sleep(retardo());
	}

	if (nuevaInstruccion->nombre == MOV_IN){
		//TODO traduccion
	}

	if (nuevaInstruccion->nombre == MOV_OUT){
		//TODO traduccion
	}

	return nuevaInstruccion;

}

void execute (t_instruccion* instruccion, t_contextoEjec* contexto) {

	switch (instruccion->nombre){
		case SET:
			set (instruccion, contexto);
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
