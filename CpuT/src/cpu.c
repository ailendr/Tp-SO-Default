#include "./cpu.h"

int main(void) {

	printf ("Hola soy cpu y estoy queriendo recibir mensajes\n ");

	loggerCPU = log_create("cpu.log", "CPU", 1, LOG_LEVEL_DEBUG);

	log_info(loggerCPU, "---------------------------------------------------------------------------");

	log_info(loggerCPU, "Iniciando CPU...");

	int servidorCpu = 0;

	configCPU = config_create("../CpuT/cpu.config");
	if(verificarConfig (servidorCpu, loggerCPU, configCPU) == 1 ) return EXIT_FAILURE;

	printf ("\n El valor recuperado de la ip es %s con el puerto %s\n", IP_Escucha(), puertoEscucha());

	log_info(loggerCPU, "Iniciando conexion con Memoria ...");

	int socketMemoria = iniciarCliente(IP_Memoria(), puertoMemoria(), loggerCPU);
	if( verificarSocket (socketMemoria, loggerCPU, configCPU) == 1 ) return EXIT_FAILURE;

	log_info(loggerCPU, "Enviando mensaje a Memoria para corroborar conexion");
    if(enviarProtocolo(socketMemoria,HANDSHAKE_Cpu, loggerCPU) == -1){
    	terminarModulo(socketMemoria,loggerCPU, configCPU);
    	return EXIT_FAILURE;
    }

    log_info(loggerCPU, "---------------------------------------------------------------------------");
    log_info(loggerCPU, "Iniciando Servidor para la conexion con el Kernel...");
    servidorCpu = iniciarServidor(IP_Escucha(), puertoEscucha());
    if(verificarSocket (servidorCpu, loggerCPU, configCPU) == 1 ){
    	close(socketMemoria);
    	return EXIT_FAILURE;
    }
    log_info(loggerCPU, "Servidor listo para recibir al Kernel");

 	log_info(loggerCPU ,"Esperando un Cliente ...");
    int cliente = esperar_cliente(servidorCpu, loggerCPU);
    if( verificarSocket (cliente, loggerCPU, configCPU) == 1 ){
    	close(servidorCpu);
    	close(socketMemoria);
        return EXIT_FAILURE;
    }
    recibirHandshake(cliente, HANDSHAKE_Kernel, loggerCPU);

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

