/*
 * conexionCpu.c
 *
 *  Created on: May 12, 2023
 *      Author: utnso
 */


#include "./cpu.h"

int servidorCpu;
int socketMemoria;
int cliente;

int iniciarSocketsCpu(){

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


t_paquete* serializarContexto(t_contextoEjec* procesoAEjecutar){

	t_paquete* paqueteContexto = malloc(sizeof(t_paquete));
	paqueteContexto->codigo_operacion = CONTEXTO;
	paqueteContexto->buffer = malloc(sizeof(t_buffer));
	paqueteContexto->buffer->size = 0;
	paqueteContexto->buffer->stream = NULL;

	int offset = 0;
	memcpy(paqueteContexto->buffer->stream +offset, &(procesoAEjecutar->PC), sizeof(uint32_t));
	offset += sizeof(uint32_t);

	memcpy(paqueteContexto->buffer->stream +offset, &(procesoAEjecutar->AX), 4);
	offset += 4;
	memcpy(paqueteContexto->buffer->stream +offset, &(procesoAEjecutar->BX), 4);
	offset += 4;
	memcpy(paqueteContexto->buffer->stream +offset, &(procesoAEjecutar->CX), 4);
	offset += 4;
	memcpy(paqueteContexto->buffer->stream +offset, &(procesoAEjecutar->DX), 4);
	offset += 4;

	memcpy(paqueteContexto->buffer->stream +offset, &(procesoAEjecutar->EAX), 8);
	offset += 8;
	memcpy(paqueteContexto->buffer->stream +offset, &(procesoAEjecutar->EBX), 8);
	offset += 8;
	memcpy(paqueteContexto->buffer->stream +offset, &(procesoAEjecutar->ECX), 8);
	offset += 8;
	memcpy(paqueteContexto->buffer->stream +offset, &(procesoAEjecutar->EDX), 8);
	offset += 8;

	memcpy(paqueteContexto->buffer->stream +offset, &(procesoAEjecutar->RAX), 16);
	offset += 16;
	memcpy(paqueteContexto->buffer->stream +offset, &(procesoAEjecutar->RBX), 16);
	offset += 16;
	memcpy(paqueteContexto->buffer->stream +offset, &(procesoAEjecutar->RCX), 16);
	offset += 16;
	memcpy(paqueteContexto->buffer->stream +offset, &(procesoAEjecutar->RDX), 16);
	offset += 16;

	memcpy(paqueteContexto->buffer->stream +offset,&(procesoAEjecutar->pid), sizeof(uint32_t));
	offset +=  sizeof(uint32_t);

	t_list* instrucciones = procesoAEjecutar->instrucciones;
	int tamanioInstrucciones = list_size(instrucciones);
	char* instruccion;
	for(int i = 0; i < tamanioInstrucciones ;i++){
		instruccion = list_get(instrucciones, i);
		agregar_a_paquete(paqueteContexto, instruccion, strlen(instruccion)+1); //para q cpu pueda usar recibirPaquete del otro lado
	}
	 return paqueteContexto;
}

t_contextoEjec* deserializarContexto(void* buffer, int tamanio){

 	 t_contextoEjec* contexto = malloc(sizeof(t_contextoEjec));

 	 void * stream = buffer;
 	 int tamanioBuffer = tamanio;
 	 int desplazamiento = 0;

 	 memcpy(&(contexto->PC), stream+desplazamiento,sizeof(uint32_t));
 	 desplazamiento+= sizeof(uint32_t);
 	 memcpy(&(contexto->AX), stream+desplazamiento, sizeof(char[4]));
 	 desplazamiento+=sizeof(char[4]);
 	 memcpy(&(contexto->BX), stream+desplazamiento, sizeof(char[4]));
 	 desplazamiento+=sizeof(char[4]);
 	 memcpy(&(contexto->CX), stream+desplazamiento, sizeof(char[4]));
 	 desplazamiento+=sizeof(char[4]);
 	 memcpy(&(contexto->DX), stream+desplazamiento, sizeof(char[4]));
 	 desplazamiento+=sizeof(char[4]);

 	 memcpy(&(contexto->EAX), stream+desplazamiento, sizeof(char[8]));
 	 desplazamiento+=sizeof(char[8]);
 	 memcpy(&(contexto->EBX), stream+desplazamiento, sizeof(char[8]));
 	 desplazamiento+=sizeof(char[8]);
 	 memcpy(&(contexto->ECX), stream+desplazamiento, sizeof(char[8]));
 	 desplazamiento+=sizeof(char[8]);
 	 memcpy(&(contexto->EDX), stream+desplazamiento, sizeof(char[8]));
 	 desplazamiento+=sizeof(char[8]);

 	 memcpy(&(contexto->RAX), stream+desplazamiento, sizeof(char[16]));
 	 desplazamiento+=sizeof(char[16]);
 	 memcpy(&(contexto->RBX), stream+desplazamiento, sizeof(char[16]));
 	 desplazamiento+=sizeof(char[16]);
 	 memcpy(&(contexto->RCX), stream+desplazamiento,sizeof(char[16]));
 	 desplazamiento+=sizeof(char[16]);
 	 memcpy(&(contexto->RDX), stream+desplazamiento, sizeof(char[16]));
 	 desplazamiento+=sizeof(char[16]);

 	 memcpy(&(contexto->pid), stream+desplazamiento,sizeof(uint32_t));
     desplazamiento += sizeof(uint32_t);

     contexto->instrucciones = deserializarInstrucciones(stream, desplazamiento,tamanioBuffer);

      return contexto;

}

t_paquete* serializarInstrucciones(t_instruccion* instruc){

	t_paquete* pInstruc = malloc(sizeof(t_paquete));
	pInstruc->codigo_operacion = instruc->nombre;
	pInstruc->buffer = malloc(sizeof(t_buffer));
	pInstruc->buffer->size = 0;
	pInstruc->buffer->stream = NULL;

	int offset = 0;

	memcpy(pInstruc->buffer->stream +offset, &(instruc->pid), sizeof(uint32_t));
	offset += sizeof(uint32_t);

	if (instruc->param1 != NULL){
		memcpy(pInstruc->buffer->stream +offset, &(instruc->param1), sizeof(strlen(instruc->param1)));
		offset += sizeof(strlen(instruc->param1));

		if (instruc->param2 != NULL){
			memcpy(pInstruc->buffer->stream +offset, &(instruc->param2), sizeof(strlen(instruc->param2)));
			offset += sizeof(strlen(instruc->param2));

			if (instruc->param3 != NULL){
				memcpy(pInstruc->buffer->stream +offset, &(instruc->param3), sizeof(strlen(instruc->param3)));
				offset += sizeof(strlen(instruc->param3));
			}

		}

	}

	return pInstruc;

}

t_list* deserializarInstrucciones(void*buffer, int desplazamiento,int tamanioBuffer){

	t_list* listaInstrucciones = list_create();
 	int tamanioInstruccion;
 	while(desplazamiento < tamanioBuffer)
 	{
		memcpy(&tamanioInstruccion, buffer + desplazamiento, sizeof(int));
 		desplazamiento+=sizeof(int);
 		char* instruccion = malloc(tamanioInstruccion);
 		memcpy(instruccion, buffer+desplazamiento, tamanioInstruccion);
 		desplazamiento+=tamanioInstruccion;
 		list_add(listaInstrucciones, instruccion);
 	}
 	return listaInstrucciones;
 }
