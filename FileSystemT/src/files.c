/*
 * files.c
 *
 *  Created on: Jun 17, 2023
 *      Author: utnso
 */

#include "files.h"

/*
//Escritura de Archivo: “Escribir Archivo: <NOMBRE_ARCHIVO> - Puntero: <PUNTERO ARCHIVO> - Memoria: <DIRECCION MEMORIA> - Tamaño: <TAMAÑO>”
void escribirArchivo(char* archivo){
    //Se deberá solicitar a la Memoria la información que se encuentra a partir de la dirección física y
//escribirlo en los bloques correspondientes del archivo a partir del puntero recibido.
//El tamaño de la información a leer de la memoria y a escribir en los bloques también deberá recibirse
//por parámetro desde el Kernel.

}
//Lectura de Archivo: “Leer Archivo: <NOMBRE_ARCHIVO> - Puntero: <PUNTERO ARCHIVO> - Memoria: <DIRECCION MEMORIA> - Tamaño: <TAMAÑO>”
void leerArchivo(nombreArchivo, direccionFisica, puntero, tamanio){
    //leer la información de los bloques a partir del puntero y el tamaño recibidos.
    //Esta información se deberá enviar a la Memoria para ser escrita a partir de la dirección física recibida por parámetro
    //y esperar su finalización para poder confirmar el éxito de la operación al Kernel.
    log_info(loggerFS, "Leer Archivo: %s - Puntero: %d - Memoria: %d - Tamaño: %d", nombreArchivo, punteroArchivo, direccionFisica, tamanio);

}
*/

//Truncate de Archivo: “Truncar Archivo: <NOMBRE_ARCHIVO> - Tamaño: <TAMAÑO>”
void truncareArchivo(char* archivo, uint32_t tamanio){
	t_fcb* fcb;
	truncate(archivo, tamanio); //o usar truncate
	if(tamanio >  fcb -> tamanioArchivo){ //Ampliar el tamaño del archivo (se agregan '/0')

		fcb -> tamanioArchivo = fcb -> tamanioArchivo + tamanio; //Al ampliar actualizar el tamaño del archivo en el FCB
		//se le deberán asignar tantos bloques como sea necesario para poder direccionar el nuevo tamaño.
	    uint32_t tamanioAAgregar = tamanio - fcb -> tamanioArchivo;
	    int cantidadBloques = cantidaddeBloques(tamanioAAgregar);
	    asignarBloques();
	}
	    }/*else{//Reducir el tamaño del archivo

	        //asignar el nuevo tamaño del archivo en el FCB
	        fcb -> tamanioArchivo = fcb -> tamanioArchivo - tamanio;
	        int cantidadDeBloquesLiberar = cantidadBloques(tamanio);
	        //deberán marcar como libres todos los bloques que ya no sean necesarios para direccionar el tamaño del archivo
	        //(descartando desde el final del archivo hacia el principio).
	        liberarBloque();//bitarray_clean_bit(bitMap, numeroBloque))
	    }
	    log_info(loggerFS, "Truncar Archivo: %s - Tamaño: %d", archivo, tamanio)

	}
*/
int cantidaddeBloques(uint32_t tamanio){
	int cantidadDeBloques = 0;
	t_superbloque* superBloque;
	   if (tamanio == superBloque -> blockSize || tamanio < superBloque -> blockSize){
	        cantidadDeBloques = 1;
	   }else{ //evalua que sea tamanio > bloquesize???
	        cantidadDeBloques = (tamanio / superBloque -> blockSize) + 1;
	   }
	return cantidadDeBloques;
}

/*asignarBloques(tamanio){
	int bloques = cantidadBloquesAsignar(uint32_t tamanio);

	recorrer el archivo de bloques para seleccionar nuevos bloques verificar que los bloques esten libres con el bitmap
	y luego haccer que nuevos punteros apunten a estos bloques
*/

