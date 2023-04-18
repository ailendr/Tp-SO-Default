#include "cpuConfig.h"

t_config* configCPU;
t_log* loggerCPU;

char* IP_Memoria(){
	return config_get_string_value(configCPU, "IP_MEMORIA");
}

char* puertoMemoria(){
	return config_get_string_value(configCPU, "PUERTO_MEMORIA");
}

char* IP_Escucha(){
	return config_get_string_value(configCPU, "IP_CPU");
}

char* puertoEscucha(){
	return config_get_string_value(configCPU, "PUERTO_ESCUCHA");
}

int retardo(){
	return config_get_int_value(configCPU, "RETARDO_INSTRUCCION");
}

int tamSegmento(){
	return config_get_int_value(configCPU, "TAM_MAX_SEGMENTO");
}
