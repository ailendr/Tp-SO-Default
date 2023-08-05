#include "FSConfig.h"

t_log* loggerFS;
t_config* configFS;

char* IP_Memoria(){
	return config_get_string_value(configFS, "IP_MEMORIA");
}

char* IP_Escucha(){
	return config_get_string_value(configFS, "IP_ESCUCHA");
}

char* puertoMemoria(){
	return config_get_string_value(configFS, "PUERTO_MEMORIA");
}

char* puertoEscucha(){
	return config_get_string_value(configFS, "PUERTO_ESCUCHA");
}

char* pathSuperBloque(){
	return config_get_string_value(configFS, "PATH_SUPERBLOQUE");
}

char* pathBitmap(){
	return config_get_string_value(configFS, "PATH_BITMAP");
}

char* pathBloques(){
	return config_get_string_value(configFS, "PATH_BLOQUES");
}

char* pathFCB(){
	return config_get_string_value(configFS, "PATH_FCB");
}

int retardoNumerico(){
	return config_get_int_value(configFS, "RETARDO_ACCESO_BLOQUE");
}
