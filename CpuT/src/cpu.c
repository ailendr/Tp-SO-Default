#include "./cpu.h"

int main(void) {

	printf ("Hola soy cpu y estoy queriendo recibir mensajes\n ");

	t_contextoEjec* contextoRecibido = NULL;
	char* instr;
	t_instruccion* nuevaInstr = NULL;
	void* buffer = NULL;
	int tamanio = 0;
	t_paquete* paqueteI = NULL;
	t_paquete* paqueteC = NULL;

	if (iniciarCpu () == 1) return EXIT_FAILURE;

	while (1){

		buffer = recibir_buffer(&tamanio, servidorCpu);
		contextoRecibido = deserializarContexto(buffer, tamanio);
		free(buffer);

		instr = fetch (&contextoRecibido);
		nuevaInstr = decode (&instr);
		execute (&nuevaInstr, contextoRecibido);

		paqueteC = serializarContexto(contextoRecibido);
		enviar_paquete(paqueteC, servidorCpu);


		if (nuevaInstr->nombre != SET && nuevaInstr->nombre != MOV_IN && nuevaInstr->nombre != MOV_OUT){
			paqueteI = serializarInstrucciones (nuevaInstr);
			enviar_paquete(paqueteC, servidorCpu);
		}

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

