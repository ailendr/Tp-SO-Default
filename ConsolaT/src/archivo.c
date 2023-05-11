/*
 * archivo.c
 *
 *  Created on: May 9, 2023
 *      Author: utnso
 */
#include "archivo.h"


	char* leerArchivo(char* pathPseudo){
		FILE *pseudocodigo = fopen(pathPseudo, "r");

			if (pseudocodigo == NULL) {
				return NULL;
			}

			//Estructura para tener las estadísticas del archivo
			//(lo usamos para saber su tamaño en bytes)
			struct stat stat_file;
			stat(pathPseudo, &stat_file);

			//Creo un buffer auxiliar para trabajar sobre él y no sobre el archivo
			char* bufferAuxiliar =  calloc(1, stat_file.st_size + 1);

			fread(bufferAuxiliar, stat_file.st_size, 1, pseudocodigo);


			fclose(pseudocodigo);
			return bufferAuxiliar;
	}

	t_list* crearLista(char* pathPseudo){
		t_list* listaDeInstrucciones = list_create();
		char* archivoAuxiliar = leerArchivo(pathPseudo);
		char** vectorInstrucciones = string_split(archivoAuxiliar, "\n"); //escribe un null al final del array
        //de esto no estoy segura para obtener el tamaño del vector
		int tamanioVector = sizeof(vectorInstrucciones);
		for(int i=0; i< tamanioVector; i++){
			char*instruccion = vectorInstrucciones[i];
			list_add(listaDeInstrucciones, instruccion);

		}
		return listaDeInstrucciones;
	}

	void enviarListaDeIntrucciones(t_list* listaInstrucciones, int conexion){
		t_paquete* paqueteInstrucciones = crear_paquete();
		 int tamanioInstrucciones = list_size(listaInstrucciones);
		     for(int i = 0; i < tamanioInstrucciones ;i++){
		         char* instruccion = list_get(listaInstrucciones, i);
		        	agregar_a_paquete(paqueteInstrucciones, instruccion, strlen(instruccion)+1); //para q cpu pueda usar recibirPaquete del otro lado
		       }

		     enviar_paquete(paqueteInstrucciones, conexion);
		     eliminar_paquete(paqueteInstrucciones);
		     list_destroy(listaInstrucciones);
	}

