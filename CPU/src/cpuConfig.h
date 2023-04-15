/*
 * cpuConfig.h
 *
 *  Created on: Apr 14, 2023
 *      Author: utnso
 */

#ifndef CPUCONFIG_H_
#define CPUCONFIG_H_
#include <commons/log.h>
#include <commons/config.h>

extern t_log* loggerCPU;
extern t_config* configCPU;

char* IP_Memoria();
int puertoMemoria();
char* IP_Filesystem();
int puertoFilesystem();
char* IP_CPU();
int puerto_CPU();
int puertoEscucha();
char* algoritmoPlanificacion();
int estimacionInicial();
int HRRRN_Alfa();
int maxGradoMultiprogramacion();
char** recursos();
char** instanciasRecursos();

#endif /* CPUCONFIG_H_ */
