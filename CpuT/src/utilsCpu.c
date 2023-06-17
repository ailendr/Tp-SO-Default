/*
 * utilsCpu.c
 *
 *  Created on: May 13, 2023
 *      Author: utnso
 */

#include "./utilsCpu.h"

int servidorCpu;
int socketMemoria;
int cliente;

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


int iniciarSocketsCpu(){
	/*
	// CONEXION CON MEMORIA -----------------------------------------------------------------------------
	log_info(loggerCPU, "Realizando Conexion con Memoria");

	socketMemoria = iniciarCliente(IP_Memoria(), puertoMemoria(), loggerCPU);
	if( verificarSocket (socketMemoria, loggerCPU, configCPU) == 1 ){
		close (servidorCpu);
		close(cliente);
		return 1;
	}

	log_info(loggerCPU, "Enviando mensaje a Memoria para corroborar conexion \n");

	if(enviarProtocolo(socketMemoria,HANDSHAKE_Cpu, loggerCPU) == -1){
		log_info(loggerCPU, "Failed -> Conexion Memoria");
		terminarModulo(socketMemoria,loggerCPU, configCPU);
		close (servidorCpu);
		close(cliente);
	    return 1;
	}
	log_info(loggerCPU, "Ok -> Conexion Memoria");

	int tamSeg = tamSegmento();

	send(socketMemoria, tamSegs, sizeof(int), 0);
	*/
	// CONEXION CON KERNEL -----------------------------------------------------------------------------
    log_info(loggerCPU, "---------------------------------------------------------------------------");
    log_info(loggerCPU, "Iniciando Servidor para la conexion con el Kernel...");
	servidorCpu = iniciarServidor(IP_Escucha(), puertoEscucha());
	if(verificarSocket (servidorCpu, loggerCPU, configCPU) == 1 ){
		close(socketMemoria);
		close(cliente);
	    return 1;
	}
	log_info(loggerCPU, "Ok -> Servidor para Kernel");

	log_info(loggerCPU ,"Esperando un Cliente ... \n");
	cliente = esperar_cliente(servidorCpu, loggerCPU);
	if(verificarSocket (cliente, loggerCPU, configCPU) == 1 ){
		close(servidorCpu);
	    close(socketMemoria);
	    return 1;
	}
	recibirHandshake(cliente, HANDSHAKE_Kernel, loggerCPU);
	log_info(loggerCPU, "Ok -> Conexion Kernel");

	return 0;
}

