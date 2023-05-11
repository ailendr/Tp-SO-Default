/*
 * archivo.h
 *
 *  Created on: May 9, 2023
 *      Author: utnso
 */

#ifndef SRC_ARCHIVO_H_
#define SRC_ARCHIVO_H_

#include <commons/string.h>
#include <commons/collections/list.h>
#include <utils/sockets.h>
#include <sys/stat.h>


char* leerArchivo(char* pathPseudo);
t_list* crearLista(char* pathPseudo);
void enviarListaDeIntrucciones(t_list* listaInstrucciones, int socket);




#endif /* SRC_ARCHIVO_H_ */
