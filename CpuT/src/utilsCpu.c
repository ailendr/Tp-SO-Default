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

	send(socketMemoria, &tamSeg, sizeof(int), 0);

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

char* mmu (char* direccionLogica, uint32_t id){
	char* dirFisica;
	int numSegmento = 0;
	int offset = 0;
	t_instruccion* nuevaInstruccion;
	int dirLogica = atoi(direccionLogica);

	numSegmento = floor(dirLogica/tamSegmento());
	offset = dirLogica % tamSegmento();

	dirFisica = strcat (string_itoa(numSegmento), " ");
	dirFisica = strcat (dirFisica, string_itoa(offset));

	nuevaInstruccion -> nombre = MENSAJE;
	nuevaInstruccion -> pid = id;
	nuevaInstruccion -> param1 = dirFisica;
	nuevaInstruccion -> param2 = NULL;
	nuevaInstruccion -> param3 = NULL;

	t_paquete* paqueteI;
	op_code* valorGuardar;

	paqueteI = serializarInstruccion(nuevaInstruccion);
	validarEnvioDePaquete(paqueteI, socketMemoria, loggerCPU, configCPU, "Instruccion");

	valorGuardar = recibir_operacion(socketMemoria);

	if (valorGuardar == ERROR){
		log_info(loggerCPU, "SEGMENTATION FAULT: PCB <ID %d>",nuevaInstruccion->pid);
		return "-1";
	}

	log_info(loggerCPU, "Fin de la traduccion de direccion logica a fisica");
	return dirFisica;
}

