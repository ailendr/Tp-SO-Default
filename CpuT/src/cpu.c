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

	t_contextoEjec* contextoRecibido;
	char* instr;
	t_instruccion* nuevaInstr = NULL;
	void* buffer = NULL;
	int tamanio = 0;
	t_paquete* paqueteI = NULL;
	t_paquete* paqueteC = NULL;

	if (iniciarCpu ("../CpuT/cpu.config") == 1) return EXIT_FAILURE;

	while (1){

		int codigo = recibir_operacion(cliente);

		if (codigo != CONTEXTO){
			log_info(loggerCPU, "No se recibio un contexto");
			break;
		}

		log_info(loggerCPU, "Se recibio el buffer del contexto");
		buffer = recibir_buffer(&tamanio, cliente);
		contextoRecibido = deserializarContexto(buffer, tamanio);

		if (contextoRecibido->pid != -1){

			log_info(loggerCPU, "Se recibio el proceso %d",contextoRecibido->pid);
			instr = fetch (contextoRecibido);
			nuevaInstr = decode (&instr);
			execute (nuevaInstr, contextoRecibido);

			paqueteC = serializarContexto(contextoRecibido);
			enviar_paquete(paqueteC, cliente);
			paqueteI = serializarInstrucciones (nuevaInstr);
			enviar_paquete(paqueteI, servidorCpu);

		} else {
			log_info(loggerCPU, "Se recibio un contexto sin PID. Revisar");
		}

	}

	free(buffer);

	log_info(loggerCPU, "Finalizando CPU...\n");
    terminarModulo(cliente,loggerCPU, configCPU);
    close(socketMemoria);
	close (servidorCpu);

	printf ("\n Finalizo CPU correctamente \n ");

	return EXIT_SUCCESS;

}




