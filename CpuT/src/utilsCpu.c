/*
 * utilsCpu.c
 *
 *  Created on: May 13, 2023
 *      Author: utnso
 */

#include "./utilsCpu.h"

int iniciarCpu (char* pathConfig){

	servidorCpu = 0;
	socketMemoria = 0;
	cliente = 0;

	loggerCPU = log_create("cpu.log", "CPU", 1, LOG_LEVEL_DEBUG);
	log_info(loggerCPU, "---------------------------------------------------------------------------");
	log_info(loggerCPU, "Iniciando CPU...");
	log_info(loggerCPU, "Ok -> Logger");

	configCPU = config_create(pathConfig);
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

    char* token = string_split(proxInstr, " ");
    uint32_t pos = 0;

    while (token[pos] != NULL){
    	log_info(loggerCPU,"%s", token[pos]);
    	pos++;
    }

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

//PRUEBAS UNITARIAS -----------------------------------------------------

void funcionPrueba (){
	t_contextoEjec* contextoRecibido;
	char* instr;
	t_instruccion* nuevaInstr = NULL;

	loggerCPU = log_create("cpu.log", "CPU", 1, LOG_LEVEL_DEBUG);

	preparandoContexto (&contextoRecibido);

	instr = fetch (&contextoRecibido);
	/*
		nuevaInstr = decode (&instr);
		execute (&nuevaInstr, contextoRecibido);
	*/
}

void preparandoContexto (t_contextoEjec* contexto){

	contexto->pid = 0;
	contexto->PC = 0;
	contexto->instrucciones = list_create();

	list_add(contexto->instrucciones, "SET AX AAAA");

}
