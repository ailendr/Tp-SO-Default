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
uint32_t pid = 0;


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


void procesoAEjecutar(t_contextoEjec* procesoAEjecutar){
	////----SERIALIZACION DE CONTEXTO------///

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
        	 for(int i = 0; i < tamanioInstrucciones ;i++){
        		 char* instruccion = list_get(instrucciones, i);
        		 agregar_a_paquete(paqueteContexto, instruccion, strlen(instruccion)+1); //para q cpu pueda usar recibirPaquete del otro lado
        	 }

         enviar_paquete(paqueteContexto,socketCPU); //Acá se serializa el paquete y se envia
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
}

 void cortoPlazo(){
	 char* algoritmo = Algoritmo();
	 //while(procesosActivos!=0){
	 if(strcmp(algoritmo,"FIFO")==0){
		 algoritmoFIFO();

	 }

	 	instruccionAEjecutar();
}


 void instruccionAEjecutar(){
	 int tamanio=0;


	 //A futuro agregar los de FS
	 t_contextoEjec* contextoActualizado ;
	 while(1){
	 	int codigo = recibir_operacion(socketCPU);

	 	 switch (codigo) {
	 	 case CONTEXTO:
	 		void* buffer = recibir_buffer(&tamanio, socketCPU);
	 		contextoActualizado = deserializarContexto(buffer, tamanio);
	 		ultimoEjecutado->contexto=contextoActualizado;
	 		free(buffer);
	 		break;
	 		//Consultar si esto debería estar en largo plazo por teoría
		case EXIT:
			finalizarProceso(ultimoEjecutado);
			break;
	 	 case YIELD:
	 		 agregarAEstadoReady(ultimoEjecutado);
		 break;
	 	 case CREATE_SEGMENT:
	 		 break;
	 	 case DELETE_SEGMENT:
	 		 break;
	 	 default:
	 		 break;
		}
	 }
 }

 void algoritmoFIFO(){
	 t_pcb* procesoAEjec = extraerDeReady();
	 t_contextoEjec* contextoAEjec = procesoAEjec->contexto; //podria usar el concepto de encapsulamiento y q pcb sepa retornarme su contexto pero wee
	 procesoAEjecutar(contextoAEjec);
	 procesoAEjec->estadoPcb = EXEC;
	 ultimoEjecutado = procesoAEjec;
 }




/// ---------TODO LO QUE TIENE QUE VER CON INSTRUCCIONES Y PROCESOS---------///

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


 t_list* obtenerInstrucciones(int socket_cliente){
	 int codigoOp = recibir_operacion(socket_cliente);
	 t_list* listaDeInstrucciones; //Recordar liberar esto cuando terminemos
	 uint32_t recepcion = 1; //Consola deberia verificar que si es 1 -> kernel recibio todo OK
	 if(codigoOp == PAQUETE){//Ver si lo aplicamos o sacamos
		 listaDeInstrucciones = recibir_paquete(socket_cliente);
		 send(socket_cliente, &recepcion, sizeof(uint32_t),0);
	 //return listaDeInstrucciones;
    }
	 return listaDeInstrucciones;
 }


 void generarProceso(int* socket_cliente){
	 int consolaNueva = *socket_cliente;
	  recibirProtocolo(socket_cliente); //Handashake
 	  t_list* instrucciones = obtenerInstrucciones(consolaNueva);
 	  t_pcb* procesoNuevo = crearPcb(instrucciones, pid);
 	  agregarAEstadoNew(procesoNuevo);
 	  pid ++;
 	  log_info(loggerKernel, "Se crea el proceso %d en New", procesoNuevo->PID);
 	  //Cerrando recursos
 	  close(consolaNueva);
 	  free(socket_cliente);
   }


 void asignarMemoria(t_pcb* procesoNuevo, t_list* tablaDeSegmento){
 	  procesoNuevo->tablaSegmentos = tablaDeSegmento;

  }

 void finalizarProceso(t_pcb* procesoAFinalizar){
 	 free(procesoAFinalizar->contexto->instrucciones);
 	 free(procesoAFinalizar->contexto);
 	// send(socketMemoria,&motivo, sizeof(uint32_t)); //Solicita a memoria que elimine la tabla de segmentos
 	 free(procesoAFinalizar);
 	 log_info(loggerKernel,"Finaliza el proceso %d - Motivo: SUCCESS", procesoAFinalizar->PID);

  }



