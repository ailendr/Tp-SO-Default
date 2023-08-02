#include "./cpu.h"


int main(int argc, char** argv) {
	if(argc < 2){
			return EXIT_FAILURE;
		}

	char* pathConfig = argv[1];

	char* instr;
	t_instruccion* nuevaInstr = NULL;

	void* buffer = NULL;
	int tamanio = 0;

	t_contextoEjec* contextoRecibido;


	int verificador = 0;


	if (iniciarCpu (pathConfig) == 1) return EXIT_FAILURE;

	while (1){

		int codigo = recibir_operacion(cliente);

		if(codigo == -1) {
			log_info(loggerCPU, "Se Desconecto kernel");
			break;
		}
    
		if (codigo != CONTEXTO){
			log_info(loggerCPU, "No se recibio un contexto");
		} else {

			log_info(loggerCPU, "Se recibio el buffer del contexto");
			buffer = recibir_buffer(&tamanio, cliente);

			int desplazamiento = 0;
			contextoRecibido = deserializarContexto(buffer, tamanio, &desplazamiento);
			free(buffer);

			if (contextoRecibido->pid != -1){

				log_info(loggerCPU, "Se recibio el proceso %d",contextoRecibido->pid);

				while (verificador == 0){
					instr = fetch (contextoRecibido);
					nuevaInstr = decode (instr, contextoRecibido);
					if (posibleSegFault (nuevaInstr) == 1){
						verificador = -1;
					} else {
						verificador = execute (nuevaInstr, contextoRecibido);
					}
				}

				verificador = 0;

				t_paquete* paqueteC = serializarContextoCompuesto(contextoRecibido, nuevaInstr);
				validarEnvioDePaquete(paqueteC, cliente, loggerCPU, configCPU, "Contexto");

			} else {
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





