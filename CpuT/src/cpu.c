#include "./cpu.h"

int main(/*int argc, char** argv*/) {

	/*
	if(argc < 2){
		printf ("Faltan argumentos para poder ejecutar CPU. Revisar el llamado");
		return EXIT_FAILURE;
	}

	if(argc > 2){
		printf ("Se invoca a CPU con demasiados argumentos. Revisar el llamado");
		return EXIT_FAILURE;
	}
	*/

	t_contextoEjec* contextoRecibido = NULL;
	char* instr;
	t_instruccion* nuevaInstr = NULL;
	void* buffer = NULL;
	int tamanio = 0;
	t_paquete* paqueteI = NULL;
	t_paquete* paqueteC = NULL;

	if (iniciarCpu ("../CpuT/cpu.config") == 1) return EXIT_FAILURE;

	while (1){

		int codigo = recibir_operacion(cliente);

		if (cliente == 0){
			log_info(loggerCPU, "Kernel se acaba de desconectar");
			break;
		}

		if (codigo != CONTEXTO){
			log_info(loggerCPU, "No se recibio un contexto");
			break;
		}

		log_info(loggerCPU, "Se recibio el buffer del contexto");
		buffer = recibir_buffer(&tamanio, cliente);
		contextoRecibido = deserializarContexto(buffer, tamanio);
		log_info(loggerCPU, "Se recibio el proceso %d",contextoRecibido->pid);

		instr = fetch (contextoRecibido);
		nuevaInstr = decode (&instr);

				/*

						execute (&nuevaInstr, contextoRecibido);


				paqueteC = serializarContexto(contextoRecibido);
				enviar_paquete(paqueteC, cliente);


						if (nuevaInstr->nombre != SET && nuevaInstr->nombre != MOV_IN && nuevaInstr->nombre != MOV_OUT){
							paqueteI = serializarInstrucciones (nuevaInstr);
							enviar_paquete(paqueteI, servidorCpu);
						}
*/


	}






	free(buffer);

	log_info(loggerCPU, "Finalizando CPU...\n");
    terminarModulo(cliente,loggerCPU, configCPU);
    close(socketMemoria);
	close (servidorCpu);

	printf ("\n Finalizo CPU correctamente \n ");

	return EXIT_SUCCESS;

}




