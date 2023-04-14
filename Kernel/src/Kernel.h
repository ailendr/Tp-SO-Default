#ifndef KERNEL_H_
#define KERNEL_H_

//#include "utilsServidor.h"

#include "sockets.h"
//SI LO INCLUYEN ACÁ NO VA EN UTILSKERNEL.H
#include "utilsKernel.h"
#include "modulo.h"


t_log* loggerKernel;
t_config* configKernel;

//IPS
char* IpMemoria();
char* IpFile();
char* IpCPU();
char* Ip();
//PUERTOS
char* PuertoMemoria();
char* PuertoFileSystem();
char* PuertoCPU();
char* Puerto();
//ALGORITMO
char* Algoritmo();
//ESTIMACION INICIAL
int Estimacion();
//ALFA
int Alfa();
//MULTIPROGRAMACION
int Multiprogramacion();
//RECURSOS
char** Recursos();
//INSTANCIAS
char** Instancias();

void iterator(char* value);

#endif /* CONSOLA_H_ */
