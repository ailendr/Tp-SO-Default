/*
 * memoriaConexiones.c
 *
 *  Created on: Apr 24, 2023
 *      Author: utnso
 */

#include "memoriaConexiones.h"

void atenderPeticionesCpu(int* socket){
	/*uint32_t protocolo;
	recv(socket, &protocolo, sizeof(uint32_t), MSG_WAITALL);

	switch(protocolo){
	case(MOV_IN):

		break;

	case(MOV_OUT):

		break;

	default:
		break;*/

}

void atenderPeticionesFs(int* socket){
	/*
	uint32_t protocolo;
	recv(socket, &protocolo, sizeof(uint32_t), MSG_WAITALL);

	switch(protocolo){
	case(F_READ):

		break;

	case (F_WRITE):

		break;

	default:
		break;
	}*/
}



void atenderModulos(int socket_servidor){
	while(1){
	//NOTA: Podemos esperar el cliente y hacer el handshake tambien dentro de la func del hilo o antes
	///---CLIENTE CPU---///
	//1)VERIFICO QUE LA CONEXION ESTE OK Y QUE EL PROTOCOLO SEA EL CORRECTO
/*	int socketCpu = esperar_cliente(socket_servidor, loggerMemoria);
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
*/

	///---CLIENTE KERNEL ---///
	int socketKernel;
	socketKernel = esperar_cliente(socket_servidor, loggerMemoria);
	if(verificarSocket(socketKernel, loggerMemoria, configMemoria) == 1) {

				   close(socket_servidor);
				   exit(1);
			   }
	recibirHandshake(socketKernel, HANDSHAKE_Kernel, loggerMemoria);
	pthread_t servidorDeKernel;
	pthread_create(&servidorDeKernel, NULL, (void*)atenderPeticionesKernel, (void*) &socketKernel);
	pthread_detach(servidorDeKernel);

	//atenderPeticionesKernel(socket_servidor);

	}
}

//ATENDER PETICIONES QUE ROMPE
void atenderPeticionesKernel(int* socketKernel){
	int socket = *socketKernel;
	log_info(loggerMemoria, "Esperando Peticiones de Kernel");

		while(1){
	     int codInstruccion;
	     codInstruccion = recibir_operacion(socket);
		 switch(codInstruccion){
			case CREATE_SEGMENT:
			t_instruccion* instruccionCS = obtenerInstruccion(socket,2); //Ahora recibe bien el PID venia arrastrando error desde la creacion en cpu
			int idSegmentoCS = atoi(instruccionCS->param1);
			int tamanioSegmento = atoi(instruccionCS->param2);
		 	t_segmento* nuevoSegmento = malloc(sizeof(t_segmento));
		 	 nuevoSegmento->PID = instruccionCS->pid;
		 	 nuevoSegmento->ID = idSegmentoCS;
		 	 uint32_t mensaje = createSegment(nuevoSegmento, tamanioSegmento);
		 	 send(socket, &mensaje, sizeof(uint32_t),0);
				break;
			case DELETE_SEGMENT:
				t_instruccion* instruccionDS = obtenerInstruccion(socket,1);
				int idSegmentoDS = atoi(instruccionDS->param1);
				t_list* tablaActualizada = deleteSegment(idSegmentoDS, instruccionDS->pid);
			    enviarTablaDeSegmentos(tablaActualizada,socket);
				break;

			case COMPACTAR:
					compactar(); //->Supongo que deberia devolver un paquete o al menos la lista de tablas actualizada              // si la lista de tablas es global no hace falta porque se ve reflejado el cambio que se hace en compactar()
					enviarListaDeTablas(listaDeTablas, socket); //Serializa y envia
				break;
			case EXIT:
				t_instruccion* instrucEliminarTabla = obtenerInstruccion(socket,0);
				liberarTablaDeSegmentos(instrucEliminarTabla->pid);
				free(instrucEliminarTabla);
				break;

			case CREAR_TABLA:
				t_instruccion* pedidocCrearTabla = obtenerInstruccion(socket,0);
				t_list* tablaDeSegmentos = crearTablaDeSegmentos(pedidocCrearTabla->pid);
				enviarTablaDeSegmentos(tablaDeSegmentos,socket);

				break;
			default:
				break;
		 	 }
			}

}


//Solo para que no sea mucho codigo dentro de los cases//
void enviarTablaDeSegmentos(t_list* tablaDeSegmentos, int socket){
	t_buffer* bufferDeTabla = malloc(sizeof(t_buffer));
     bufferDeTabla->size = 0;
	 bufferDeTabla->stream = NULL;
	serializarTablaDeSegmentos(tablaDeSegmentos, bufferDeTabla);
	validarEnvioBuffer(bufferDeTabla, socket, "Tabla de Segmentos", loggerMemoria, configMemoria);
}

void enviarListaDeTablas(t_list* listaDeTablas, int socket){
	t_buffer* bufferListaDeTablas = serializarListaDeTablas(listaDeTablas);
	validarEnvioBuffer(bufferListaDeTablas, socket, "Lista de Tablas", loggerMemoria, configMemoria);
}









