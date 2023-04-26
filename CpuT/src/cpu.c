#include "./cpu.h"

int main(void) {

	printf ("Hola soy cpu y estoy queriendo recibir mensajes\n ");

	loggerCPU = log_create("cpu.log", "CPU", 1, LOG_LEVEL_DEBUG);

	log_info(loggerCPU, "---------------------------------------------------------------------------");

	log_info(loggerCPU, "Iniciando CPU...");

	int servidorCpu = 0;

	configCPU = config_create("../CpuT/cpu.config");
	if(verificarConfig (servidorCpu, loggerCPU, configCPU) == 1 ) return EXIT_FAILURE;

	char* ip = IP_Escucha();
	char* puerto = puertoEscucha();

	printf ("\n El valor recuperado de la ip es %s con el puerto %s\n", ip, puerto);

	log_info(loggerCPU, "Iniciando Servidor ... \n");
    servidorCpu = iniciarServidor(ip, puerto);
    if(verificarSocket (servidorCpu, loggerCPU, configCPU) == 1 ) return EXIT_FAILURE;
    log_info(loggerCPU, "Servidor listo para recibir al cliente");

	log_info(loggerCPU ,"Esperando un Cliente ... \n");
    int cliente = esperar_cliente(servidorCpu, loggerCPU);
    if( verificarSocket (cliente, loggerCPU, configCPU) == 1 ){
    		close(servidorCpu);
    		return EXIT_FAILURE;
    	}
	recibirHandshake(cliente);
	log_info(loggerCPU, "---------------------------------------------------------------------------");

	log_info(loggerCPU, "Iniciando conexion con Memoria ... \n");

	char* ipM = IP_Memoria();
	char* puertoM = puertoMemoria();

	int socketMemoria = iniciarCliente(ipM, puertoM, loggerCPU);
	if( verificarSocket (socketMemoria, loggerCPU, configCPU) == 1 ) return EXIT_FAILURE;

	log_info(loggerCPU, "Enviando mensaje \n");
    enviarProtocolo(socketMemoria, loggerCPU);

	log_info(loggerCPU, "Finalizando CPU...\n");
    terminarModulo(cliente,loggerCPU, configCPU);
    close(socketMemoria);
	close (servidorCpu);


	printf ("\n Finalizo CPU correctamente \n ");

	return EXIT_SUCCESS;

}

