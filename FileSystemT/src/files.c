/*
 * files.c
 *
 *  Created on: Jun 17, 2023
 *      Author: utnso
 */

#include "files.h"

t_list* listaFCB;

void abrirArchivo(char* nombreArchivo){
    //verificar existencia de FCB
    //pedir fcb a memoria?
	//t_list* list_filter(t_list*, bool(*condition)(void*));
	//int i = 0;
	listaFCB = list_create();
	t_fcb *fcb1; //  = listaFCB[i];
	int tamanioLista = list_size(listaFCB);
	char* nombre = fcb1 -> nombreDeArchivo;

// sacar un elemento, guardarlo que la fcb1 y comparar el nombre de fcb1 con nombreArchivo
	for(int j = 0; j < tamanioLista; j++){
		fcb1 = list_get(listaFCB, j);
		if(nombre == nombreArchivo){
			log_info(loggerFS, "Abrir Archivo: %s", nombreArchivo);
	    } else{
	        //sino informar que el archivo no existe
	        log_info(loggerFS, "No existe el archivo: %s ", nombreArchivo);
	    }
/*	for(int j = 0; j < tamanioLista; j++){
		if(strcmp(nombre , nombreArchivo) == 0){
        log_info(loggerFS, "Abrir Archivo: %s", nombreArchivo);
    } else{
        //sino informar que el archivo no existe
        log_info(loggerFS, "No existe el archivo: %s ", nombreArchivo);
    }
*/
    }
}

//Crear Archivo: “Crear Archivo: <NOMBRE_ARCHIVO>”
void crearArchivo(char* nombreDelArchivo){
    //hacer malloc para el fcb nuevo??
    //malloc(sizeof(t_fcb));
    //crear un archivo FCB
    t_fcb* fcb1 = malloc(sizeof(t_fcb));
    fcb1 -> nombreDeArchivo = nombreDelArchivo;
    fcb1 -> tamanioArchivo = 0;
    fcb1 -> punteroDirecto = NULL; //bloques asociados 0
    fcb1 -> punteroIndirecto = NULL; // si pongo 0 el puntero apunta al bloque 0?
    list_add(listaFCB, fcb1);
    //se crea nuevo bloque

    log_info(loggerFS, "Crear Archivo: %s ", nombreDelArchivo);
    //esta operación deberá devolver OK.

}

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
void truncarArchivo(char* archivo, uint32_t tamanio){
	t_fcb* fcb = fcbArchivo(archivo);
	truncate(archivo, tamanio); //o usar truncate
	if(tamanio >  fcb -> tamanioArchivo){ //Ampliar el tamaño del archivo (se agregan '/0')

		fcb -> tamanioArchivo = fcb -> tamanioArchivo + tamanio; //Al ampliar actualizar el tamaño del archivo en el FCB
		//se le deberán asignar tantos bloques como sea necesario para poder direccionar el nuevo tamaño.
	    uint32_t tamanioAAgregar = tamanio - fcb -> tamanioArchivo;
	    int cantidadDeBloquesAsignar = cantidadBloques(tamanioAAgregar);
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
int cantidadBloques(uint32_t tamanio){
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
t_fcb* fcbArchivo (char* nombreArchivo){
	//listaFCB = list_create();
	t_fcb* fcb1; //  = listaFCB[i];
	int tamanioLista = list_size(listaFCB);
	char* nombre = fcb1 -> nombreDeArchivo;
	for(int j = 0; j < tamanioLista; j++){
		fcb1 = list_get(listaFCB, j);
		if(nombre == nombreArchivo){
			return fcb1;
		}
		//crearFCB();
	}
}
