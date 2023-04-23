#include <stdio.h>
#include <stdlib.h>
#include "kernel.h"


int main(void) {

	printf ("Hola soy kernel y estoy queriendo recibir mensajes\n ");

	loggerKernel = log_create("kernel.log", "Kernel", 1, LOG_LEVEL_DEBUG);

	log_info(loggerKernel, "---------------------------------------------------------------------------");

	log_info(loggerKernel, "Iniciando Kernel...");

	int server_fd = 0;

	configKernel = config_create("../KernelT/kernel.config");

	if(verificarConfig (server_fd, loggerKernel, configKernel) == 1 ) return EXIT_FAILURE;

	char* ip = Ip();
	char* puerto = Puerto();

	printf ("El valor recuperado de la ip es %s con el puerto %s\n", ip, puerto);

	log_info(loggerKernel, "Iniciando Servidor ... \n");
	server_fd = iniciarServidor(ip, puerto);
	if(verificarSocket (server_fd, loggerKernel, configKernel) == 1 ) return EXIT_FAILURE;
	log_info(loggerKernel, "Servidor listo para recibir al cliente");

	log_info(loggerKernel, "Iniciando conexion con un Cliente ... \n");
	int cliente_fd = esperar_cliente(server_fd);
	if( verificarSocket (cliente_fd, loggerKernel, configKernel) == 1 ){
			close(server_fd);
			return EXIT_FAILURE;
	}
	log_info(loggerKernel, "Se conecto un cliente");

	t_list* lista;

		while (1) {
			int cod_op = recibir_operacion(cliente_fd);

			switch (cod_op) {
				case MENSAJE:
					log_info(loggerKernel, "\nMe llego el mensaje: %s", recibir_mensaje(cliente_fd));
					//Lo pase a un log porque a la larga necesitamos recuperarlo
					break;

				case PAQUETE:
					lista = recibir_paquete(cliente_fd);
					log_info(loggerKernel, "Me llegaron los siguientes valores:\n");
					list_iterate(lista, (void*) iterator);
					break;

				case -1:
					log_info(loggerKernel, "el cliente se desconecto.");
					break;

				default:
					log_warning(loggerKernel,"Operacion desconocida. No quieras meter la pata");
					break;
			}

			if ( cod_op == -1 ) break;
		}

	///Funcion para inicializar las conexiones con FS,CPU y MEMORIA//
		//Por el momento deje las funciones enviarMensaje adentro pero no deberian//
    iniciarConexionesDeKernel();


	log_info(loggerKernel, "Finalizando Kernel...\n");

	terminarModulo(cliente_fd,loggerKernel, configKernel);
	close (server_fd);
	close (socketFs);
	close (socketMemoria);
	close (socketCPU);

	printf ("Finalizo Kernel correctamente\n ");

	return EXIT_SUCCESS;
}

void iterator(char* value) {
	log_info(loggerKernel,"%s", value);
}
