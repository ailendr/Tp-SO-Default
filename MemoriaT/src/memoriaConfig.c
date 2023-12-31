#include "memoriaConfig.h"

t_log* loggerMemoria;
t_config* configMemoria;
t_config* configIPs;

char* puertoEscucha(){
	return config_get_string_value(configMemoria, "PUERTO_ESCUCHA");
}

char* ipEscucha(){
	return config_get_string_value(configIPs, "IP_MEMORIA");
}

int tam_memoria(){
	return config_get_int_value(configMemoria, "TAM_MEMORIA");
}

int cantSegmentos(){
	return config_get_int_value(configMemoria, "CANT_SEGMENTOS");
}
int tam_segmento_cero(){
	return config_get_int_value(configMemoria, "TAM_SEGMENTO_0");
}
int retardoMemoria(){
	return config_get_int_value(configMemoria, "RETARDO_MEMORIA");
}

int retardoCompactacion(){
	return config_get_int_value(configMemoria, "RETARDO_COMPACTACION");
}

char* algoritmoAsignacion(){
	return config_get_string_value(configMemoria, "ALGORITMO_ASIGNACION");
}
