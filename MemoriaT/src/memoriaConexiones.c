/*
 * memoriaConexiones.c
 *
 *  Created on: Apr 24, 2023
 *      Author: utnso
 */

#include "memoriaConexiones.h"

void atenderPeticionesCpu(int socket){
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

void atenderPeticionesFs(int socket){
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
void enviarTablaDeSegmentos(t_list* tablaDeSegmentos, int socket){
	t_buffer* bufferDeTabla = serializarTablaDeSegmentos(tablaDeSegmentos);
	enviarBuffer(bufferDeTabla, socket);
	free(bufferDeTabla);
}

void atenderPeticionesKernel(int socket){
	while(1){
	/*//Primer Peticion : crear Tabla para un Proceso//
	int pidDeProceso = 0;
	recv(socket, &pidDeProceso, sizeof(uint32_t), MSG_WAITALL);
	t_list* tablaDeSegmentos = crearTablaDeSegmentos(pidDeProceso);
	enviarTablaDeSegmentos(tablaDeSegmentos);*/

	//Segunda Peticion : Son Instrucciones que llegan en paquete entonces tienen cod_op y buffer
     int codInstruccion = recibir_operacion(socket);
	 switch(codInstruccion){
		case CREATE_SEGMENT:
		t_instruccion* instruccionCS = obtenerInstruccion(socket,2);
		int idSegmentoCS = atoi(instruccionCS->param1);
		int tamanioSegmento = atoi(instruccionCS->param2);
	 	t_segmento* nuevoSegmento = malloc(sizeof(t_segmento));
	 	 nuevoSegmento->PID=instruccionCS->pid;
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
/*
		case COMPACTAR:
			compactar(); //->Supongo que deberia devolver un paquete o al menos la lista de tablas actualizada
			//hago send de todas las tablas actualizadas.
			break;
		case EXIT:
			liberarTablaDeSegmentos(pid);
			break;
*/
		default://Si entra aca es porque recibio un pid para crear la tabla de segmentos->sino agregamos el handashake eso da igual pero somos la q controlamos q envia kernely se supone q no enviamos nada incorrecto

			t_list* tablaDeSegmentos = crearTablaDeSegmentos(codInstruccion);
			enviarTablaDeSegmentos(tablaDeSegmentos,socket);

			break;
	 	 }
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

