/*
 * peticiones.c
 *
 *  Created on: Jun 17, 2023
 *      Author: utnso
 */

#include "peticiones.h"

void atenderPeticiones(){

	void* buffer = NULL;
	int tamanio = 0;
	int cantParam = 0;

	t_instruccion* newInstr;

	while (1){
		int codigo = recibir_operacion(cliente);

		if (codigo == -1){
			log_info(loggerFS, "Kernel se desconecto, no se atienden mas peticiones");
			newInstr->nombre = EXIT;
			newInstr->pid = -1;
			list_add(peticiones, newInstr);
		    //Activamos el semaforo debe ser contador
		    //signal del contador
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
			newInstr = deserializarInstruccionEstructura(buffer, cantParam);
		    list_add(peticiones, newInstr);
		    //Activamos el semaforo debe ser contador
		    //signal del contador
		}
	}
}

void ejecutarPeticiones(){

	t_list* aEjecutar;
	t_instruccion* instruccion;
	op_code nombre;

	t_paquete* paqueteI;

	aEjecutar = list_create();

	while(1){
		//wait del contador
		//sacar peticion de la lista

		aEjecutar = list_take_and_remove(peticiones, 1);
		instruccion = list_get(aEjecutar, 0);

		nombre = instruccion->nombre;

		if (nombre == EXIT){
			log_info(loggerFS, "Finalice todas las peticiones pendientes. Finalizamos modulo");
			break;
		}

		switch(nombre){
			case F_READ:
				//leerArchivo(archivo, direccionFisica, puntero, tamanio);
				break;
			case F_WRITE:
				//escribirArchivo();
				break;
			case F_OPEN:
				if (1 /*si el archivo existe*/){
					//abrirArchivo(archivo);
				}else{
					//crearArchivo(archivo);
				}
				break;
			case F_TRUNCATE:
				//truncarArchivo(archivo);
				break;
			case F_CLOSE:
				break;
			case F_SEEK:
				break;
		}

		paqueteI = serializarInstruccion(instruccion);
		validarEnvioDePaquete(paqueteI, cliente, loggerFS, configFS, "Instruccion de File System");

	}

	list_destroy(aEjecutar);

}
