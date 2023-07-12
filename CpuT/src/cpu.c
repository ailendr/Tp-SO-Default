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

	char* instr;
	t_instruccion* nuevaInstr = NULL;

	void* buffer = NULL;
	int tamanio = 0;

	t_contextoEjec* contextoRecibido;

	t_paquete* paqueteI;
	t_paquete* paqueteC;

	int verificador = 0;


	if (iniciarCpu ("../CpuT/cpu.config") == 1) return EXIT_FAILURE;

	while (1){

		int codigo = recibir_operacion(cliente);

		if(codigo == -1) {
			log_info(loggerCPU, "se cayo kernel");
			break;
		}
    
		if (codigo != CONTEXTO){
			log_info(loggerCPU, "No se recibio un contexto");

		}
		else{
		log_info(loggerCPU, "Se recibio el buffer del contexto");
		buffer = recibir_buffer(&tamanio, cliente);
		contextoRecibido = deserializarContexto(buffer, tamanio);

		if (contextoRecibido->pid != -1){

			log_info(loggerCPU, "Se recibio el proceso %d",contextoRecibido->pid);

			while (verificador == 0){
				instr = fetch (contextoRecibido);
				nuevaInstr = decode (instr, contextoRecibido);
				verificador = execute (nuevaInstr, contextoRecibido);
			}

			verificador = 0;

			paqueteC = serializarContexto(contextoRecibido);
			validarEnvioDePaquete(paqueteC, cliente, loggerCPU, configCPU, "Contexto");
			paqueteI = serializarInstruccion(nuevaInstr);
			validarEnvioDePaquete(paqueteI, cliente, loggerCPU, configCPU, "Instruccion");

		} else {
			log_info(loggerCPU, "Se recibio un contexto sin PID. Revisar");
		}
		}
	}
	free (paqueteI);
	free (paqueteC);

	//(free(buffer); ESTO NO VA. EL BUFFER SE LIBERA EN VALIDARENVIOPAQUETE

	log_info(loggerCPU, "Finalizando CPU...\n");
    terminarModulo(cliente,loggerCPU, configCPU);
    close(socketMemoria);
	close (servidorCpu);

	printf ("\n Finalizo CPU correctamente \n ");

	return EXIT_SUCCESS;

}




