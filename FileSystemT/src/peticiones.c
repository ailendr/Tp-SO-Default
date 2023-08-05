/*
 * peticiones.c
 *
 *  Created on: Jun 17, 2023
 *      Author: utnso
 */

#include "peticiones.h"
//Agrego los sockets : Los extern estan en config.c
int servidorFS;
int socketMemoria;
int cliente;

int servidorFS;
int socketMemoria;
int cliente;

void atenderPeticiones(){
   log_info(loggerFS, "Recibiendo Peticiones de Kernel ");
	//void* buffer = NULL;
	//int tamanio = 0;
	int cantParam = 0;
	//int desplazamiento = 0;

	t_instruccion* newInstr = malloc(sizeof(t_instruccion));

	while (1){
		int codigo = recibir_operacion(cliente);

		if (codigo == -1){
			log_info(loggerFS, "Kernel se desconecto, no se atienden mas peticiones");
			newInstr->nombre = EXIT;
			newInstr->pid = -1;
			list_clean(peticiones);
			list_add(peticiones, newInstr);
			break;
		}

		log_info(loggerFS, "Se recibio una peticion");
		//buffer = recibir_buffer(&tamanio, cliente);

		switch (codigo){
			case F_READ:
				cantParam = 3;
				break;
			case F_WRITE:
				cantParam = 3;
				break;
			case F_OPEN:
				cantParam = 1;
				break;
			case F_CREATE:
				cantParam = 1;
				break;
			case F_TRUNCATE:
				cantParam = 2;
				break;
			case F_CLOSE:
				cantParam = 1;
				break;
			case F_SEEK:
				cantParam = 2; 
				break;
			default:
				cantParam = -1;
				log_info(loggerFS, "No comprendo la instruccion mandada. Revisar");
				break;
		}

		if (cantParam != -1) {
			//Le pongo otra variable porq cuando se deserializa ya se hace malloc y newInstruc ya hizo malloc al comienzo
			//newInstr
			t_instruccion* nuevaInstruc = obtenerInstruccion(cliente, cantParam); //Esta funcion recibi un buffer , deserializa la instruccion y libera el buffer ;)
			//deserializarInstruccionEstructura(buffer, cantParam, &desplazamiento);
			list_add(peticiones, nuevaInstruc);
			//free(buffer);
		}

		ejecutarPeticiones();

	}
}

void ejecutarPeticiones(){
 log_info(loggerFS, "Ejecutando Peticiones");

	t_instruccion* instruccion = malloc (sizeof(t_instruccion));
	int nombre;
	char* nombreArchivo;
	int valorOp = 0;
	t_paquete* paquete;
	t_fcb* fcb;

	instruccion = list_get(list_take_and_remove(peticiones,1), 0);

	nombre = instruccion->nombre;

	if (nombre == EXIT){
		log_info(loggerFS, "Finalice todas las peticiones pendientes. Finalizamos modulo");
	}

	nombreArchivo = instruccion -> param1;

	switch(nombre){
		case F_READ:
			int bytesRead = atoi(instruccion->param3);
			char* bufferLectura = malloc(bytesRead);
			//char mensaje_read[100]= "";
		    char* mensaje = NULL;
			leerArchivo(instruccion,(void*) bufferLectura, bytesRead);
			//sprintf(mensaje, "MOV_OUT %s %s %d", direccion_fisica_read, aLeer, tamanio_read);
		    memcpy(&mensaje, bufferLectura+0, bytesRead);
			//Armado de instruccion FRead y envio a Memoria//
		    instruccion->param1 = mensaje;
		    paquete = serializarInstruccion(instruccion);
		    validarEnvioDePaquete(paquete, socketMemoria, loggerFS, configFS, "Instruccion F Read a Memoria");
		    recv(socketMemoria,&valorOp, sizeof(int), MSG_WAITALL); //espera confirmacion a memoria
		    if(valorOp == OK){
			send(cliente, &valorOp, sizeof(int), 0);
		    }
		    free(bufferLectura);
			break;

		case F_WRITE:
			int bytesWrite = atoi(instruccion->param3);
			paquete = serializarInstruccion(instruccion);
			validarEnvioDePaquete(paquete, socketMemoria, loggerFS, configFS, "Instruccion F Write a Memoria");
			int codigo = recibir_operacion(socketMemoria);
			if(codigo != (-1) && codigo == MENSAJE){
			char* bufferEscritura = recibir_mensaje(socketMemoria);
			log_info(loggerFS, "Se recibió la informacion a Escribir desde Memoria; %s", bufferEscritura);
			escribirArchivo (instruccion, (void*)bufferEscritura, bytesWrite);
			}
			break;

		case F_OPEN:
			fcb = cargarFCB (nombreArchivo);
			if ( fcb != NULL ){
				abrirArchivo(fcb);
				valorOp = OK;
			} else {
				instruccion->param1 = "-1";
				log_info(loggerFS, "ERROR: ARCHIVO NO EXISTENTE   Operacion: ABRIR (OPEN) -> Archivo: %s", nombreArchivo);
				valorOp = ERROR;
			}
			send(cliente,&valorOp,sizeof(int),0 );

			break;

		case F_CREATE:
			crearArchivo(nombreArchivo);

			break;

		case F_TRUNCATE:
			 int res = truncarArchivo (nombreArchivo, atoi(instruccion -> param2));
			 if (res == 0){ valorOp = OK; }
			// else { valorOp = ERROR; }
			 send(cliente, &valorOp, sizeof(int), 0);
			break;

		case F_SEEK:
			posicionarPuntero (nombreArchivo, instruccion->param2);

			break;
	}

	log_info(loggerFS, "Peticion Finalizada de PID: %i", instruccion -> pid);
	free(instruccion);

}
