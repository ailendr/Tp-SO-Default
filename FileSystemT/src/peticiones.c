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
				break;
			case F_OPEN:
				break;
			case F_TRUNCATE:
				break;
			default:
				log_info(loggerFS, "No comprendo la instruccion mandada. Revisar");
		}

		newInstr = deserializarInstruccionEstructura(buffer, cantParam);

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
