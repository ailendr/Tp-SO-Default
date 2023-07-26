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

char* mmu (char* direccionLogica, uint32_t id, int tam){
	char* dirFisica;
	int numSegmento = 0;
	int offset = 0;
	int valorGuardar = tamSegmento();
	t_instruccion* nuevaInstruccion = malloc(sizeof(t_instruccion*));

	int dirLogica = atoi(direccionLogica);

	numSegmento = floor(dirLogica/valorGuardar);
	offset = dirLogica % tamSegmento();

	dirFisica = strcat (string_itoa(numSegmento), " ");
	dirFisica = strcat (dirFisica, string_itoa(offset));

	log_info(loggerCPU, "DIRECCION FISICA: %s", dirFisica);

	int cantBytes = 0;
	cantBytes = offset + tam;
	log_info(loggerCPU, "CANT BYTES A LEER: %i", cantBytes);

	if (cantBytes > (valorGuardar - 1)){
		log_info(loggerCPU, "SEGMENTATION FAULT: PCB <ID %d>", id);
		return "-1";
	}

	nuevaInstruccion -> nombre = MENSAJE;
	nuevaInstruccion -> pid = id;
	nuevaInstruccion -> param1 = dirFisica;
	nuevaInstruccion -> param2 = string_itoa(cantBytes);
	//nuevaInstruccion -> param3 = NULL;

	t_paquete* paqueteI;

	paqueteI = serializarInstruccion(nuevaInstruccion);
	validarEnvioDePaquete(paqueteI, socketMemoria, loggerCPU, configCPU, "Instruccion");

	valorGuardar = recibir_operacion(socketMemoria);

	if (valorGuardar == ERROR){
		log_info(loggerCPU, "SEGMENTATION FAULT: PCB <ID %d>", nuevaInstruccion->pid);
		return "-1";
	}

	log_info(loggerCPU, "Fin de la traduccion de direccion logica a fisica");
	return dirFisica;
}

int tamRegistro (char* registro){
	if (strcmp(registro, "AX") == 0 || strcmp(registro, "BX") == 0 || strcmp(registro, "CX") == 0 || strcmp(registro, "DX") == 0){
		return 4;
	}
	if (strcmp(registro, "EAX") == 0 || strcmp(registro, "EBX") == 0 || strcmp(registro, "ECX") == 0 || strcmp(registro, "EDX") == 0){
		return 8;
	}
	if (strcmp(registro, "RAX") == 0 || strcmp(registro, "RBX") == 0 || strcmp(registro, "RCX") == 0 || strcmp(registro, "RDX") == 0){
		return 16;
	}
	return 0;
}

int posibleSegFault (t_instruccion* nuevaInstr){
	if (cantidadDeParametros(nuevaInstr->nombre) == 0) return 0;
	if (strcmp(nuevaInstr->param1, "-1") == 0) return 1;
	if (cantidadDeParametros(nuevaInstr->nombre) > 1){
		if (strcmp(nuevaInstr->param2, "-1") == 0) return 1;
	}
	return 0;
}
