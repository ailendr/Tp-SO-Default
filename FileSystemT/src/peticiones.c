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
			log_info(loggerFS, "Kernel se desconecto, se procede a terminar el modulo");
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
		}

	}
}

void ejecutarPeticiones(){
	//sacar peticion de la lista
	t_instruccion* instruccion;
	int tamanioListaPeticiones = list_size (peticiones);
	for(int i = 0; i < tamanioListaPeticiones; i++){
		instruccion = list_get(peticiones, i);
	}
	switch(instruccion){
	//recuperar los parametros
	case F_READ:
		leerArchivo(archivo, direccionFisica, puntero, tamanio);
		break;
    case F_WRITE:
    	escribirArchivo();
    	break;
	case F_OPEN:
		if (/*si el archivo existe*/){
		abrirArchivo(archivo);
		}else{
		crearArchivo(archivo);
		}
		break;
	case F_TRUNCATE:
	    truncarArchivo(archivo);
	    break;
	case F_CLOSE:

		break;
    case F_SEEK:
	}
    paqueteI = serializarInstruccion(nuevaInstr);
	validarEnvioDePaquete(paqueteI, cliente, loggerCPU, configCPU, "Instruccion");
}
