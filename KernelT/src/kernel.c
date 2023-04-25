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

	///--------Kernel como Cliente-------///

	///Funcion para inicializar las conexiones con FS,CPU y MEMORIA//
		//Por el momento deje las funciones enviarMensaje adentro pero no deberian//
    iniciarConexionesDeKernel();

    ///------Kernel como Servidor------//

    log_info(loggerKernel, "Iniciando Servidor ... \n");
    	server_fd = iniciarServidor(ip, puerto);
    	if(verificarSocket (server_fd, loggerKernel, configKernel) == 1 ) return EXIT_FAILURE;
    	log_info(loggerKernel, "Servidor listo para recibir al cliente");
         atenderConsolas(server_fd);


	log_info(loggerKernel, "Finalizando Kernel...\n");
    terminarModulo(server_fd,loggerKernel, configKernel);
	close (server_fd);
	close (socketFs);
	close (socketMemoria);
	close (socketCPU);

	printf ("\n Finalizo Kernel correctamente\n ");

	return EXIT_SUCCESS;
}

void iterator(char* value) {
	log_info(loggerKernel,"%s", value);
}
