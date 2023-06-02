/*
 * estructurasfs.c
 *
 *  Created on: Jun 1, 2023
 *      Author: utnso
 */

#include "estructurasfs.h"

t_superbloque* iniciarSuperBloque(){
	t_superbloque* superBloque;
	char* rutaSuperBloque = pathSuperBloque();
	int tamBloque =  config_get_int_value(rutaSuperBloque, "BLOCK_SIZE");
	int cantBloques =  config_get_int_value(rutaSuperBloque, "BLOCK_COUNT");
	superBloque->blockSize=tamBloque;
	superBloque->blockSize=cantBloques;
	return superBloque;
}

