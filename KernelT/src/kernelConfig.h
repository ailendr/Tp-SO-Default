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
extern t_config* configIPs;


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
double Estimacion();
//ALFA
double Alfa();
//MULTIPROGRAMACION
int Multiprogramacion();
//RECURSOS
char** Recursos();
//INSTANCIAS
char** Instancias();

#endif /* UTILSKERNEL_H_ */
