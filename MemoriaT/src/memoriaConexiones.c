/*
 * memoriaConexiones.c
 *
 *  Created on: Apr 24, 2023
 *      Author: utnso
 */

#include "memoriaConexiones.h"

void atenderPeticionesCpu(int* socketCpu){
	int socket = * socketCpu;

while(1){
	int codInstruccion;
    codInstruccion = recibir_operacion(socket);
    t_instruccion* instruccion;
	switch(codInstruccion){
	case(MOV_IN):
		instruccion = obtenerInstruccion(socket, 2);
	     char* cantBytes = instruccion->param1;
	     int bytes = atoi(cantBytes); //Los bytes llegan el param 1 que es el Registro
	implementarInstruccion(instruccion->param2,instruccion->pid, instruccion->param1,socket, MOV_IN, bytes);



		break;

	case(MOV_OUT):
		instruccion = obtenerInstruccion(socket, 2);
	implementarInstruccion(instruccion->param1, instruccion->pid, instruccion->param2,socket, MOV_OUT,0);


		break;
	case (MENSAJE):
		instruccion = obtenerInstruccion(socket,1);
	    char* direcF= instruccion->param1;
	    char** direccionFisica = string_split(direcF, " ");
		char* numSeg = direccionFisica[1];
		int numSegmento= atoi(numSeg);
		validarNumSegmento(numSegmento, socket); //Avisa a Cpu que es un segmento invalido
		break;
	case(-1):
			log_info(loggerMemoria, "Error al recibir el codigo de operacion. Hemos finalizado la Conexion "); //Esto es porque el recibir_op retorna un -1 si hubo error y nunca lo consideramos
	//ver como proseguir
	break;

	default:
		break;
		}
	}
}



void atenderPeticionesFs(int* socketFs){
   int socket = *socketFs;
	while(1){
		int codInstruccion;
	    codInstruccion = recibir_operacion(socket);
	    t_instruccion* instruccion;
	switch(codInstruccion){
	case(F_READ):
			instruccion = obtenerInstruccion(socket, 3);
		implementarInstruccion(instruccion->param2, instruccion->pid, instruccion->param1, socket, F_READ,0);


		break;

	case (F_WRITE):
			instruccion = obtenerInstruccion(socket, 3);
	        int bytes = atoi(instruccion->param3);
		implementarInstruccion(instruccion->param2, instruccion->pid, instruccion->param1, socket, F_WRITE, bytes);

		break;
	case(-1):
			log_info(loggerMemoria, "Error al recibir el codigo de operacion. Hemos finalizado la Conexion "); //Esto es porque el recibir_op retorna un -1 si hubo error y nunca lo consideramos
	//ver como proseguir
		break;
	default:
		break;
		}
	}
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
	     int codInstruccion = recibir_operacion(socket);
	     t_instruccion* instruccion;
		 switch(codInstruccion){
			case CREATE_SEGMENT:
				instruccion = obtenerInstruccion(socket,2); //Ahora recibe bien el PID venia arrastrando error desde la creacion en cpu
			int idSegmentoCS = atoi(instruccion->param1);
			int tamanioSegmento = atoi(instruccion->param2);
		 	t_segmento* nuevoSegmento = malloc(sizeof(t_segmento));
		 	 nuevoSegmento->PID = instruccion->pid;
		 	 nuevoSegmento->ID = idSegmentoCS;
		 	 logearListaDeSegmentos("Antes de Realizar Create Segment");
		 	 uint32_t mensaje = createSegment(nuevoSegmento, tamanioSegmento);
		 	 send(socket, &mensaje, sizeof(uint32_t),0);
		 	 free(instruccion);
				break;
			case DELETE_SEGMENT:
				instruccion = obtenerInstruccion(socket,1);
				int idSegmentoDS = atoi(instruccion->param1);
				t_tabla* tablaActualizada = deleteSegment(idSegmentoDS, instruccion->pid);
			    enviarTablaDeSegmentos(tablaActualizada,socket);
			    free(instruccion);
				break;

			case COMPACTAR:
				    usleep(retardoCompactacion());
					compactar(); //->Supongo que deberia devolver un paquete o al menos la lista de tablas actualizada              // si la lista de tablas es global no hace falta porque se ve reflejado el cambio que se hace en compactar()
					enviarListaDeTablas(listaDeTablas, socket); //Serializa y envia
				break;
			case EXIT:
				instruccion = obtenerInstruccion(socket,0);
				liberarTablaDeSegmentos(instruccion->pid);
				free(instruccion);
				break;

			case CREAR_TABLA:
				instruccion = obtenerInstruccion(socket,0);
				t_tabla* tablaDeSegmentos = crearTablaDeSegmentos(instruccion->pid);
				enviarTablaDeSegmentos(tablaDeSegmentos,socket);
				free(instruccion);

				break;
			case(-1):
				 log_info(loggerMemoria, "Error al recibir el codigo de operacion. Hemos finalizado la Conexion "); //Esto es porque el recibir_op retorna un -1 si hubo error y nunca lo consideramos
				 //ver como proseguir
				break;
			default:
				break;
		 	 }
			}

}


//Solo para que no sea mucho codigo dentro de los cases//
void enviarTablaDeSegmentos(t_tabla* tablaDeSegmentos, int socket){
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









