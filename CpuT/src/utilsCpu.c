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

t_instruccion* fetch(t_contextoEjec* cont) {

    char* proxInstr = list_get(cont->instrucciones, cont->PC);
    t_instruccion* instAEjecutar;
    log_info(loggerCPU, "FETCH: PCB <ID %d>", cont->pid);
    log_info(loggerCPU, "Instruccion: %s", proxInstr);
    //Separa instruccion y guardarlo
    return instAEjecutar;
}
