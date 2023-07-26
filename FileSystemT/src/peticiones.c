/*
 * peticiones.c
 *
 *  Created on: Jun 17, 2023
 *      Author: utnso
 */

#include "peticiones.h"

sem_t nuevoPedido;

void atenderPeticiones(){

	void* buffer = NULL;
	int tamanio = 0;
	int cantParam = 0;
	int desplazamiento = 0;

	t_instruccion* newInstr;

	while (1){
		int codigo = recibir_operacion(cliente);

		if (codigo == -1){
			log_info(loggerFS, "Kernel se desconecto, no se atienden mas peticiones");
			newInstr->nombre = EXIT;
			newInstr->pid = -1;
			queue_clean(peticiones);
			queue_push(peticiones, newInstr);
		    sem_post(&nuevoPedido);
			break;
		}

		log_info(loggerFS, "Se recibio una peticion");
		buffer = recibir_buffer(&tamanio, cliente);

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
			newInstr = deserializarInstruccionEstructura(buffer, cantParam, &desplazamiento);
			queue_push(peticiones, newInstr);
			free(buffer);
		    sem_post(&nuevoPedido);
		}
	}
}

void ejecutarPeticiones(){

	t_instruccion* instruccion;
	op_code nombre;
	char* nombreArchivo;

	t_paquete* paqueteI;

	while(1){

		sem_wait(&nuevoPedido);

		instruccion = queue_peek(peticiones);
		queue_pop(peticiones);

		nombre = instruccion->nombre;

		if (nombre == EXIT){
			log_info(loggerFS, "Finalice todas las peticiones pendientes. Finalizamos modulo");
			break;
		}

		nombreArchivo = instruccion -> param1;

		switch(nombre){
			case F_READ:
				//TODO leerArchivo(instruccion);
				break;
			case F_WRITE:
				//TODO escribirArchivo (instruccion);
				break;
			case F_OPEN:
				if (posicionFCB (nombreArchivo) != -1){
					abrirArchivo(nombreArchivo);
				} else {
					instruccion->param1 = "-1";
				}
				break;
			/*
			case F_CREATE:
				//TODO Crear el f_create
				crearArchivo(nombreArchivo);
				break;
			*/
			case F_TRUNCATE:
				//TODO truncarArchivo (nombreArchivo, instruccion -> param2);
				break;
			case F_CLOSE:
				cerrarArchivo(nombreArchivo);
				break;
			case F_SEEK:
				posicionarPuntero (nombreArchivo, instruccion->param2);
				break;
		}

		paqueteI = serializarInstruccion(instruccion);
		validarEnvioDePaquete(paqueteI, cliente, loggerFS, configFS, "Instruccion de File System");
		//TODO Ver que kernel lo reciba y empezar a trabajar desde ahi
	}

}
