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


	int verificador = 0;


	if (iniciarCpu ("../CpuT/cpu.config") == 1) return EXIT_FAILURE;

	while (1){

		int codigo = recibir_operacion(cliente);

		if(codigo == -1) {
			log_info(loggerCPU, "Se cayo kernel");
			break;
		}
    
		if (codigo != CONTEXTO){
			log_info(loggerCPU, "No se recibio un contexto");

		}
		else{
		log_info(loggerCPU, "Se recibio el buffer del contexto");
		buffer = recibir_buffer(&tamanio, cliente);
		//
		int desplazamiento = 0; //el valor de desplazamiento va a cambiar despues de deserializarContexto pero aca no tiene valor util
		contextoRecibido = deserializarContexto(buffer, tamanio, &desplazamiento);
        free(buffer);
		//
		if (contextoRecibido->pid != -1){

			log_info(loggerCPU, "Se recibio el proceso %d",contextoRecibido->pid);

			while (verificador == 0){
				instr = fetch (contextoRecibido);
				nuevaInstr = decode (instr, contextoRecibido);
				if (strcmp(nuevaInstr->param1, "-1") == 0 || strcmp(nuevaInstr->param2, "-1") == 0){
					verificador = -1;
					}
				else {
					verificador = execute (nuevaInstr, contextoRecibido);
					}

				}

				verificador = 0;

				t_paquete* paqueteC = serializarContextoCompuesto(contextoRecibido, nuevaInstr);
				validarEnvioDePaquete(paqueteC, cliente, loggerCPU, configCPU, "Contexto");

				}
				else {
					log_info(loggerCPU, "Se recibio un contexto sin PID. Revisar");
					}
			}
	}


	log_info(loggerCPU, "Finalizando CPU...\n");
    terminarModulo(cliente,loggerCPU, configCPU);
    close(socketMemoria);
	close (servidorCpu);

	printf ("\n Finalizo CPU correctamente \n ");

	return EXIT_SUCCESS;

}




