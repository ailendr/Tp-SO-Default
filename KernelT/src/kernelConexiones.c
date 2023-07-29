/*
 * KernelConexiones.c
 *
 *  Created on: Apr 23, 2023
 *      Author: utnso
 */

#include "kernelConexiones.h"


//GLOBALES: Los puse asi para poder liberarlos desde el main//
int socketCPU;
int socketFs;
int socketMemoria;

void iniciarConexionesDeKernel(){

			char* ipCPU = IpCPU();
			char* puertoCPU = PuertoCPU();
			char* ipFs = IpFile();
			char* puertoFs = PuertoFileSystem();
			char* ipMemoria =IpMemoria ();
			char* puertoMemoria = PuertoMemoria();

			  log_info(loggerKernel, "Iniciando conexion con CPU ...");
              socketCPU = iniciarCliente(ipCPU, puertoCPU, loggerKernel);
			    if( verificarSocket (socketCPU, loggerKernel, configKernel) == 1 ) exit(1);
			  log_info(loggerKernel, "Enviando mensaje ");
			    if (enviarProtocolo(socketCPU, HANDSHAKE_Kernel,loggerKernel) == -1) exit(1);


               //>>>>>>CONEXION CON FILE SYSTEM<<<<<<<<<


			  log_info(loggerKernel, "Iniciando conexion con FS ...");
              socketFs = iniciarCliente(ipFs, puertoFs,loggerKernel);
			  	if( verificarSocket (socketFs, loggerKernel, configKernel) == 1 ) exit(1);

			  log_info(loggerKernel, "Enviando mensaje");
              if (enviarProtocolo(socketFs,HANDSHAKE_Kernel,loggerKernel) == -1) exit(1);
 			  log_info(loggerKernel, "Finalizando conexion con Fs \n");


			   //>>>>>>CONEXION CON MEMORIA <<<<<<<<<


			  log_info(loggerKernel, "Iniciando conexion con MEMORIA ... \n");
			  socketMemoria = iniciarCliente(ipMemoria, puertoMemoria, loggerKernel);
			  	if( verificarSocket (socketMemoria, loggerKernel, configKernel) == 1 ) exit(1);

			  log_info(loggerKernel, "Enviando mensaje");

			  	if (enviarProtocolo(socketMemoria,HANDSHAKE_Kernel, loggerKernel) == -1) exit(1);
			//  log_info(loggerKernel, "Finalizando conexion con Memoria \n");


		}



void atenderConsolas(int socket_servidor){

	while (1) {
		  pthread_t hiloServidor;

		   int *socket_cliente = malloc(sizeof(int));
		   *socket_cliente = esperar_cliente(socket_servidor, loggerKernel);
		   if(verificarSocket(*socket_cliente, loggerKernel, configKernel) == 1) {
			  free(socket_cliente);
			  close(socket_servidor);
			  exit(1);
		   }

		   pthread_create(&hiloServidor,
		                    NULL,
		                   (void*) generarProceso,
		                   (void*)socket_cliente);
		   pthread_detach(hiloServidor);

   }
}




