#include "./cpu.h"

int main(void) {

	printf ("Hola soy cpu y estoy queriendo recibir mensajes\n ");

	if (iniciarCpu () == 1) return EXIT_FAILURE;

	//t_contextoEjec* contextoRecibido = deserializarContexto(....)
	//t_instruccion* instr = fetch (contextoRecibido);

    /*
     ----------------------------------------------------
     TODO
     fetch()
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

