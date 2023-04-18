#include "./cpu.h"

int main(void) {

	printf ("Hola soy cpu y estoy queriendo recibir mensajes\n ");

	loggerCPU = log_create("cpu.log", "CPU", 1, LOG_LEVEL_DEBUG);

	log_info(loggerCPU, "---------------------------------------------------------------------------");
	//Esto es capricho perdooon, asi queda visualmente mas facil de identificar las ejecuciones
	log_info(loggerCPU, "Iniciando CPU...");

	int server_fd = 0;

	configCPU = config_create("../CpuT/cpu.config");
	if(verificarConfig (server_fd, loggerCPU, configCPU) == 1 ) return EXIT_FAILURE;

	char* ip = IP_Escucha();
	char* puerto = puertoEscucha();

	printf ("\nEl valor recuperado de la ip es %s con el puerto %s\n", ip, puerto);

	server_fd = iniciarServidor(ip, puerto);

	if(verificarSocket (server_fd, loggerCPU, configCPU) == 1 ) return EXIT_FAILURE;

	log_info(loggerCPU, "Servidor listo para recibir al cliente");

	int cliente_fd = esperar_cliente(server_fd);

	if(cliente_fd == -1){
		log_error(loggerCPU,"Error al conectar con el cliente");
		terminarModulo(server_fd,loggerCPU, configCPU);
		return EXIT_FAILURE;
	}

	log_info(loggerCPU, "Se conecto un cliente");

	/*
	t_list* lista;

	while (1) {
		int cod_op = recibir_operacion(cliente_fd);

		switch (cod_op) {
			case MENSAJE:
				log_info(loggerCPU, "\nMe llego el mensaje: %s", recibir_mensaje(cliente_fd));
				break;

			case PAQUETE:
				lista = recibir_paquete(cliente_fd);
				log_info(loggerCPU, "Me llegaron los siguientes valores:\n");
				list_iterate(lista, (void*) iterator);
				break;

			case -1:
				log_info(loggerCPU, "el cliente se desconecto.");
				return EXIT_FAILURE;

			default:
				log_warning(loggerCPU,"Operacion desconocida. No quieras meter la pata");
				break;
		}
	}*/

	log_info(loggerCPU, "Finalizando CPU...\n");

	terminarModulo(cliente_fd,loggerCPU, configCPU);
	close (server_fd);

	printf ("Finalizo CPU correctamente\n ");

	return EXIT_SUCCESS;

}

