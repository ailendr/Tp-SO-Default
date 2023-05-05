/*
 * planificacion.c
 *
 *  Created on: Apr 28, 2023
 *      Author: utnso
 */

#include "planificacion.h"


t_queue* colaNew ;
t_list* colaReady ;
int procesosActivos=0;
t_pcb* ultimoEjecutado;

void crearEstados(){
  colaNew = queue_create();
  colaReady = list_create();
}

 void agregarAEstadoNew(t_pcb* procesoNuevo){
 queue_push(colaNew, procesoNuevo);
 procesoNuevo->estadoPcb = NEW;
 }

 void agregarAEstadoReady(t_pcb* procesoListo){
	 list_add(colaReady, procesoListo);
	 procesoListo->estadoPcb = READY;

	 log_info(loggerKernel, "Cola Ready con algoritmo %s .Ingresa el proceso con id %d:", Algoritmo(), procesoListo->PID);
 }

 t_pcb* extraerDeNew(){
 	 t_pcb* proceso = queue_pop(colaNew);

 	 return proceso;
  }

 t_pcb* extraerDeReady(){
	 t_pcb* proceso = list_remove(colaReady,0);//Iba a usar el list_get pero tiene sentido sacar el elemento de la cola y eliminarlo de ahi
	 return proceso;
 }




void procesoAEjecutar(t_contextoEjec* procesoAEjecutar){ //TODO
	////----SERIALIZACION------///

	 t_paquete* paqueteContexto = malloc(sizeof(t_paquete));
	 paqueteContexto->codigo_operacion = CONTEXTO;
	 paqueteContexto->buffer = malloc(sizeof(t_buffer));
	 paqueteContexto->buffer->size = 0;
	 paqueteContexto->buffer->stream = NULL;
	 t_list* instrucciones = procesoAEjecutar->instrucciones;
	 int tamanioInstrucciones = list_size(instrucciones);
	 for(int i = 0; i < tamanioInstrucciones ;i++){
		 char* instruccion = list_get(instrucciones, i);
		 agregar_a_paquete(paqueteContexto, instruccion, strlen(instruccion)+1); //para q cpu pueda usar recibirPaquete del otro lado
	 }
         int offset = paqueteContexto->buffer->size ;
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

         memcpy(paqueteContexto->buffer->stream +offset,&(procesoAEjecutar->instrucActual), sizeof(instruccionActual));
         /////----FIN DE SERIALIZACION----///

         enviar_paquete(paqueteContexto,socketCPU);
}

 void largoPlazo(){
	 t_pcb* proceso;
	 //Con un semaforo tendriamos que ver lo de la multiprogramacion, esto lo dejo hasta que los implementemos
	 //int procesosActivos = cantProcesosEnMemoria();

	 //Pasaje de New a Ready//
	 int maxGradoMultiprogram = Multiprogramacion(); //Tiene que ser un Globals
	 if(procesosActivos <= maxGradoMultiprogram){
		 proceso = extraerDeNew(colaNew);
		 enviarProtocolo(socketMemoria, loggerKernel);//El handshake seria el pedido de memoria
		 //asignarMemoria(proceso, tabla); //PCB creado
		 agregarAEstadoReady(proceso);
	     procesosActivos ++;
	     log_info(loggerKernel, "PID: %d - Estado Anterior: NEW - Estado Actual: READY", proceso->PID);
	 }

	 ///Cualquier estado a EXIT///
	/*t_paquete* paquete = malloc(sizeof(t_paquete));
	 paquete->buffer=malloc(sizeof(t_buffer));
	 int codigo = recibir_operacion(socketCPU);
	 if(codigo=CONTEXTO){
	 recv(socketCPU, &paquete->buffer->size,sizeof(uint32_t),MSG_WAITALL);
	 paquete->buffer->stream = malloc(paquete->buffer->size);
	 recv(socketCPU, paquete->buffer->stream, paquete->buffer->size, 0);
	 /////////DESERIALIZACION////////////////
	 deserializarContexto(paquete->buffer);
	  if(contexto->proxInstruccion == EXIT){
	  finalizarProceso(ultimoEjecutado);

	    }*/

t_contextoEjec* deserializarContexto(t_buffer* buffer){
	 t_contextoEjec* contexto = malloc(sizeof(t_contextoEjec));

	 void * stream= buffer->stream;
	 //DUDA: para deserializar la lista debería recibir el tamaño?

	 memcpy(&(contexto->PC), stream, sizeof(uint32_t));
	 stream+= sizeof(uint32_t);
	 memcpy(&(contexto->AX), stream, sizeof(char[4]));
	 stream+=sizeof(char[4]);
	 memcpy(&(contexto->BX), stream, sizeof(char[4]));
	 stream+=sizeof(char[4]);
	 memcpy(&(contexto->CX), stream, sizeof(char[4]));
	 stream+=sizeof(char[4]);
	 memcpy(&(contexto->DX), stream, sizeof(char[4]));
	 stream+=sizeof(char[4]);
	 memcpy(&(contexto->EAX), stream, sizeof(char[8]));
	 stream+=sizeof(char[8]);
	 memcpy(&(contexto->EBX), stream, sizeof(char[8]));
	 stream+=sizeof(char[8]);
	 memcpy(&(contexto->ECX), stream, sizeof(char[8]));
	 stream+=sizeof(char[8]);
	 memcpy(&(contexto->EDX), stream, sizeof(char[8]));
	 stream+=sizeof(char[8]);
	 memcpy(&(contexto->RAX), stream, sizeof(char[16]));
	 stream+=sizeof(char[16]);
	 memcpy(&(contexto->RBX), stream, sizeof(char[16]));
	 stream+=sizeof(char[16]);
	 memcpy(&(contexto->RCX), stream, sizeof(char[16]));
	 stream+=sizeof(char[16]);
	 memcpy(&(contexto->RDX), stream, sizeof(char[16]));
	 stream+=sizeof(char[16]);
 }


 void finalizarProceso(t_pcb* procesoAFinalizar){
	 free(procesoAFinalizar->contexto);
	// send(socketMemoria,&motivo, sizeof(uint32_t)); //Solicita a memoria que elimine la tabla de segmentos
	 free(procesoAFinalizar);
	 log_info(loggerKernel,"Finaliza el proceso %d - Motivo: SUCCESS", procesoAFinalizar->PID);


 }


 void cortoPlazo(){
	 char* algoritmo = Algoritmo();
	 if(strcmp(algoritmo,"FIFO")){
		 algoritmoFIFO();
	 }
 }

 void algoritmoFIFO(){
	 t_pcb* procesoAEjec = extraerDeReady();
	 t_contextoEjec* contextoAEjec = procesoAEjec->contexto; //podria usar el concepto de encapsulamiento y q pcb sepa retornarme su contexto pero wee
	 procesoAEjecutar(contextoAEjec);
	 ultimoEjecutado = procesoAEjec;

 }
/// ---------TODO LO QUE TIENE QUE VER CON INSTRUCCIONES---------///

 t_list* obtenerInstrucciones(int socket_cliente){
	 int codigoOp = recibir_operacion(socket_cliente);
	 t_list* listaDeInstrucciones; //Recordar liberar esto cuando terminemos
	 uint32_t recepcion = 1; //Consola deberia verificar que si es 1 -> kernel recibio todo OK
	 if(codigoOp == PAQUETE){//Ver si lo aplicamos o sacamos
	 listaDeInstrucciones = recibir_paquete(socket_cliente);
	 send(socket_cliente, &recepcion, sizeof(uint32_t),0);
	 return listaDeInstrucciones;
   }

 }

 void generarProceso(int* socket_cliente){
	 int consolaNueva = *socket_cliente;
	   recibirProtocolo(socket_cliente); //Handashake
 	  t_list* instrucciones = obtenerInstrucciones(consolaNueva);
 	  t_pcb* procesoNuevo = crearPcb(instrucciones);
 	  agregarAEstadoNew(procesoNuevo);
 	  log_info(loggerKernel, "Se crea el proceso %d en New", procesoNuevo->PID);
   }

   void asignarMemoria(t_pcb* procesoNuevo, t_list* tablaDeSegmento){
 	  procesoNuevo->tablaSegmentos = tablaDeSegmento;
   }




