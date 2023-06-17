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

	while (1){
		int codigo = recibir_operacion(cliente);

		if (codigo == -1){
			log_info(loggerFS, "Kernel se desconecto, se procede a terminar el modulo");
			break;
		}

		log_info(loggerFS, "Se recibio una peticion");
		buffer = recibir_buffer(&tamanio, cliente);


		/*
		contextoRecibido = deserializarContexto(buffer, tamanio);

		if (contextoRecibido->pid != -1){

		log_info(loggerCPU, "Se recibio el proceso %d",contextoRecibido->pid);

		while (verificador == 0){
			instr = fetch (contextoRecibido);
			nuevaInstr = decode (instr);
			verificador = execute (nuevaInstr, contextoRecibido);
		}

		paqueteC = serializarContexto(contextoRecibido);
		validarEnvioDePaquete(paqueteC, cliente, loggerCPU, configCPU, "Contexto");
		paqueteI = serializarInstruccion(nuevaInstr);
		validarEnvioDePaquete(paqueteI, cliente, loggerCPU, configCPU, "Instruccion");

		} else {
			log_info(loggerCPU, "Se recibio un contexto sin PID. Revisar");
		}
		*/
	}
}

void ejecutarPeticiones(){

}
