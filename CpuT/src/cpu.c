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

	printf ("\nEl valor recuperado de la ip es %s con el puerto %s\n", ip, puerto);

	log_info(loggerCPU, "Iniciando Servidor ... \n");
    servidorCpu = iniciarServidor(ip, puerto);
    if(verificarSocket (servidorCpu, loggerCPU, configCPU) == 1 ) return EXIT_FAILURE;
    log_info(loggerCPU, "Servidor listo para recibir al cliente");

	log_info(loggerCPU ,"Iniciando Cliente ... \n");
    int cliente = esperar_cliente(servidorCpu);
    if( verificarSocket (cliente, loggerCPU, configCPU) == 1 ){
    		close(servidorCpu);
    		return EXIT_FAILURE;
    	}
    	log_info(loggerCPU, "Se conecto un cliente");



	//t_list* lista;

	while (1) {
		int cod_op = recibir_operacion(cliente);

		switch (cod_op) {
			case MENSAJE:
				log_info(loggerCPU, "\nMe llego el mensaje: %s", recibir_mensaje(cliente));
				break;

			/*case PAQUETE:
				lista = recibir_paquete(cliente_fd);
				log_info(loggerCPU, "Me llegaron los siguientes valores:\n");
				list_iterate(lista, (void*) iterator);
				break;*/

			case -1:
				log_info(loggerCPU, "el cliente se desconecto.");
				break;

			default:
				log_warning(loggerCPU,"Operacion desconocida. No quieras meter la pata");
				break;
		}

		if ( cod_op == -1 ) break;
      }


	log_info(loggerCPU, "Iniciando conexion con Memoria ... \n");

	char* ipM = IP_Memoria();
	char* puertoM = puertoMemoria();

	int socketMemoria = iniciarCliente(ipM, puertoM);
	if( verificarSocket (socketMemoria, loggerCPU, configCPU) == 1 ) return EXIT_FAILURE;

	log_info(loggerCPU, "Conexion exitosa");
	log_info(loggerCPU, "Enviando mensaje");
	enviar_mensaje("Hola Memoria soy CPU", socketMemoria);


	log_info(loggerCPU, "Finalizando CPU...\n");
    terminarModulo(cliente,loggerCPU, configCPU);
    close(socketMemoria);
	close (servidorCpu);


	printf ("Finalizo CPU correctamente\n ");

	return EXIT_SUCCESS;

}

