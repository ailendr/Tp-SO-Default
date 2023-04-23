/*
 * KernelConexiones.c
 *
 *  Created on: Apr 23, 2023
 *      Author: utnso
 */

#include "KernelConexiones.h"
#include <utils/sockets.h>
#include "KernelConfig.h"
//GLOBALES//
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

			log_info(loggerKernel, "Iniciando conexion con CPU ... \n");
            socketCPU = iniciarCliente(ipCPU, puertoCPU, loggerKernel);
			  if( verificarSocket (socketCPU, loggerKernel, configKernel) == 1 ) exit(1);
			  log_info(loggerKernel, "Enviando mensaje");
			 enviar_mensaje("Hola CPU soy Kernel", socketCPU);
               log_info(loggerKernel, "Finalizando conexion con CPU");

               /*>>>>>>CONEXION CON FILE SYSTEM<<<<<<<<<*/


			  log_info(loggerKernel, "Iniciando conexion con FS ... \n");
              socketFs = iniciarCliente(ipFs, puertoFs,loggerKernel);
			   if( verificarSocket (socketFs, loggerKernel, configKernel) == 1 ) exit(1);
			     log_info(loggerKernel, "Enviando mensaje");
			  		enviar_mensaje("Hola FS soy Kernel", socketFs);



			   /*>>>>>>CONEXION CON MEMORIA <<<<<<<<<*/


			  log_info(loggerKernel, "Iniciando conexion con MEMORIA ... \n");


			  socketMemoria = iniciarCliente(ipMemoria, puertoMemoria, loggerKernel);
			  	if( verificarSocket (socketMemoria, loggerKernel, configKernel) == 1 ) exit(1);

			  	log_info(loggerKernel, "Enviando mensaje");
			  	enviar_mensaje("Hola Memoria soy Kernel", socketMemoria);
		}


