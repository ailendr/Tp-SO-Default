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

    return proxInstr;
}

t_instruccion* decode (char* instruccion) {

	t_instruccion* nuevaInstruccion;

	char** token = string_split(instruccion, " ");
    	uint32_t pos = 0;

	/*
   	while (token[pos] != NULL){
    		log_info(loggerCPU,"%s", token[pos]);
    		pos++;
    	}
	*/
	
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
