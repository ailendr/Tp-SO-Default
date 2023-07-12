/*
 * sockets.h
 *
 *  Created on: Apr 16, 2023
 *      Author: utnso
 */

#ifndef SRC_UTILS_SOCKETS_H_
#define SRC_UTILS_SOCKETS_H_

//#include "modulo.h"
#include "estructuras.h"

typedef struct
{
	int size;
	void* stream;
} t_buffer;

typedef struct
{
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;

///----Handshakes---//
typedef enum {
    HANDSHAKE_Consola,
    HANDSHAKE_Cpu,
    HANDSHAKE_Kernel,
	HANDSHAKE_Fs,
	HANDSHAKE_PedirMemoria,
    HANDSHAKE_Ok,
} t_handshake;

int iniciarServidor(char*  ip, char* puerto);
int iniciarCliente(char *ip, char* puerto,t_log* logger);

int esperar_cliente(int socket_servidor, t_log* logger);
char* recibir_mensaje(int socket_cliente);
void* recibir_buffer(int* size, int socket_cliente);
int recibir_operacion(int socket_cliente);
t_list* recibir_paquete(int socket_cliente);
void recibirProtocolo (int* socket_cliente);
void recibirHandshake(int socket_cliente,t_handshake handshake,t_log* logger);


void enviar_mensaje(char* mensaje, int socket_cliente);
int enviarProtocolo(int conexion,t_handshake handshake,t_log* logger);
void* serializar_paquete(t_paquete* paquete, int bytes);
void validarEnvioBuffer(t_buffer* buffer, int socket, char* nombreBuffer, t_log* logger, t_config* config);
int enviarBuffer(t_buffer* buffer, int socket, char* nombreBuffer, t_log* logger);


void crear_buffer(t_paquete* paquete);
t_paquete* crear_paquete(void);
void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio);
void enviar_paquete(t_paquete* paquete, int socket_cliente);
int enviarPaquete(t_paquete* paquete, int socket_cliente, t_log* logger, char* nombrePaq);
void eliminar_paquete(t_paquete* paquete);
void validarEnvioDePaquete(t_paquete* paquete, int socket_cliente, t_log* logger,t_config* config, char* nombrePaq);

void liberar_conexion(int socket);

//-----Funcion de modulo porq se estaban llamando modulo.h y sockets.h de forma circulas----//
void terminarModulo(int conexion,t_log* log , t_config* config);

#endif /* SRC_UTILS_SOCKETS_H_ */
