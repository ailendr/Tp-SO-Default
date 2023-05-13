#include "./cpu.h"

int main(void) {

	printf ("Hola soy cpu y estoy queriendo recibir mensajes\n ");

	t_contextoEjec* contextoRecibido = NULL;
	t_instruccion* instr = NULL;
	void* buffer = NULL;
	int tamanio = 0;

	if (iniciarCpu () == 1) return EXIT_FAILURE;

	while (1){

		buffer = recibir_buffer(&tamanio, servidorCpu);
		contextoRecibido = deserializarContexto(buffer, tamanio);
		free(buffer);

		instr = fetch (contextoRecibido);

	}

    /*
     ----------------------------------------------------
     TODO
     Decode()
     Execute()

     Traducir direcciones logicas a fisicas

     Actualizando el contexto de ejecucion
     ----------------------------------------------------
     */

	log_info(loggerCPU, "Finalizando CPU...\n");
    terminarModulo(cliente,loggerCPU, configCPU);
    close(socketMemoria);
	close (servidorCpu);

	printf ("\n Finalizo CPU correctamente \n ");

	return EXIT_SUCCESS;

}

