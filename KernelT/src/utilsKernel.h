/*
 * utilsKernel.h
 *
 *  Created on: Apr 16, 2023
 *      Author: utnso
 */

#ifndef UTILSKERNEL_H_
#define UTILSKERNEL_H_

#include <utils/modulo.h>


extern t_log* loggerKernel;
extern t_config* configKernel;



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

#endif /* UTILSKERNEL_H_ */
