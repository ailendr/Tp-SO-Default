/*
 * conexionFS.c
 *
 *  Created on: Jun 17, 2023
 *      Author: utnso
 */

#include "FSUtils.h"

int servidorFS;
int socketMemoria;
int cliente;

int iniciarMemoria (){
	log_info(loggerFS, "Realizando Conexion con Memoria");

	int socketMemoria = iniciarCliente(IP_Memoria(), puertoMemoria(), loggerFS);
	if( verificarSocket (socketMemoria, loggerFS, configFS) == 1 ) {
		close (servidorFS);
		return EXIT_FAILURE;
	}

	log_info(loggerFS, "Enviando mensaje \n");
    if(enviarProtocolo(socketMemoria, HANDSHAKE_Fs,loggerFS) == -1){
		log_info(loggerFS, "Failed -> Conexion Memoria");
        terminarModulo(socketMemoria,loggerFS, configFS);
        close (servidorFS);
        return EXIT_FAILURE;
    }

	log_info(loggerFS, "Ok -> Conexion Memoria");

	return 0;
}

int iniciarServKernel (){
	log_info(loggerFS, "Creando Servidor para futuras peticiones");
	servidorFS = iniciarServidor(IP_Escucha(), puertoEscucha());

	if( verificarSocket (servidorFS, loggerFS, configFS) == 1 ){
		close (socketMemoria);
		return EXIT_FAILURE;
	}
	log_info(loggerFS, "Esperando que se conecte Kernel");

	cliente = esperar_cliente(servidorFS, loggerFS);
	if( verificarSocket (cliente, loggerFS, configFS) == 1 ){
	    close(servidorFS);
	    close(cliente);
	    return EXIT_FAILURE;
	}

	recibirHandshake(cliente, HANDSHAKE_Kernel, loggerFS);
	log_info(loggerFS, "Ok -> Servidor de Peticiones");

	return 0;
}
