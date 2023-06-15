/*
 * memoriaConexiones.c
 *
 *  Created on: Apr 24, 2023
 *      Author: utnso
 */

#include "memoriaConexiones.h"

void atenderPeticionesCpu(int socket){

}

void atenderPeticionesFs(int socket){

}

void atenderPeticionesKernel(int socket){
	//recibirHandshake(socket, HANDSHAKE_PedirMemoria);
	uint32_t protocolo;
			uint32_t resultado_ok = HANDSHAKE_Ok;
			uint32_t resultado_error = - 1;

	     recv(socket, &protocolo, sizeof(uint32_t), MSG_WAITALL);
			if(protocolo == HANDSHAKE_PedirMemoria){
				   send(socket, &resultado_ok, sizeof(uint32_t), 0);
			       log_info(loggerMemoria, "Se ha iniciado estructura en memoria para un Proceso");
			}
			else{
				   send(socket, &resultado_error, sizeof(uint32_t), 0);
	/*			}
	recv(socket, &protocolo, sizeof(uint32_t), MSG_WAITALL);
	switch(protocolo){
		case CREATE_SEGMENT:
			break;
		case DELETE_SEGMENT:
			break;
		case HANDSHAKE_PedirMemoria:
			t_segmento* tablaDeSegmentos;
			tablaDeSegmentos = crearTablasDeSegmentos();
			//y hacer el send. Habria que serializar la tabla.
	}*/
			}
}





void atenderModulos(int socket_servidor){
	//NOTA: Podemos esperar el cliente y hacer el handshake tambien dentro de la func del hilo o antes
	///---CLIENTE CPU---///
	//1)VERIFICO QUE LA CONEXION ESTE OK Y QUE EL PROTOCOLO SEA EL CORRECTO
	int socketCpu = esperar_cliente(socket_servidor, loggerMemoria);
	if(verificarSocket(socketCpu, loggerMemoria, configMemoria) == 1) {

				   close(socket_servidor);
				   exit(1);
			   }
	recibirHandshake(socketCpu, HANDSHAKE_Cpu, loggerMemoria);
	//2)LEVANTO HILO PARA ATENDER LAS PETICIONES DE CADA MODULO
	pthread_t servidorDeCpu;
	pthread_create(&servidorDeCpu, NULL, (void*) atenderPeticionesCpu, (void*)&socketCpu);
	pthread_detach(servidorDeCpu);


	///---CLIENTE FILE SYSTEM---///
	//1)VERIFICO QUE LA CONEXION ESTE OK Y QUE EL PROTOCOLO SEA EL CORRECTO

	int socketFs= esperar_cliente(socket_servidor, loggerMemoria);
	if(verificarSocket(socketFs, loggerMemoria, configMemoria) == 1) {

				   close(socket_servidor);
				   exit(1);
			   }
	recibirHandshake(socketFs, HANDSHAKE_Fs, loggerMemoria);
	//2)LEVANTO HILO PARA ATENDER LAS PETICIONES DE CADA MODULO

	pthread_t servidorDeFs;
	pthread_create(&servidorDeFs, NULL, (void*)atenderPeticionesFs, (void*)&socketFs);
	pthread_detach(servidorDeFs);


	///---CLIENTE KERNEL ---///
	int socketKernel = esperar_cliente(socket_servidor, loggerMemoria);
	if(verificarSocket(socketKernel, loggerMemoria, configMemoria) == 1) {

				   close(socket_servidor);
				   exit(1);
			   }
	recibirHandshake(socketKernel, HANDSHAKE_Kernel, loggerMemoria);

	pthread_t servidorDeKernel;
	pthread_create(&servidorDeKernel, NULL, (void*)atenderPeticionesKernel, (void*)&socketKernel);
	pthread_detach(servidorDeKernel);

}

/*
while (1) {
	  pthread_t hiloServidor;
	   int *socket_cliente = malloc(sizeof(int));
	   *socket_cliente = esperar_cliente(socket_servidor, loggerMemoria);
	   if(verificarSocket(*socket_cliente, loggerMemoria, configMemoria) == 1) {
		   free(socket_cliente);
		   close(socket_servidor);
		   exit(1);
	   }
	   pthread_create(&hiloServidor,
	                    NULL,
	                   (void*) recibirProtocolo,
	                   (void*)socket_cliente);
	   pthread_detach(hiloServidor);
}*/

