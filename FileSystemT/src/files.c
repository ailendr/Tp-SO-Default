/*
 * files.c
 *
 *  Created on: Jun 17, 2023
 *      Author: utnso
 */



void abrirArchivo(char* nombreArchivo){
    //verificar existencia de FCB
    //pedir fcb a memoria?
	t_fcb fcb;
    if(fcb -> nombreDeArchivo == nombreArchivo){
        log_info(loggerFS, "Abrir Archivo: %s", nombreArchivo);
        return ok; //si existe devolver ok
    } else{
        //sino informar que el archivo no existe
        log_info(loggerFS, "No existe el archivo: %s ", nombreArchivo);

    }

}
//Crear Archivo: “Crear Archivo: <NOMBRE_ARCHIVO>”
void crearArchivo(nombreDelArchivo){
    //hacer malloc para el fcb nuevo??
    //malloc(sizeof(t_fcb));
    //crear un archivo FCB
    t_fcb fcb1;
    fcb1 -> nombreDeArchivo = nombreDelArchivo;
    fcb1 -> tamanioArchivo = 0;
    fcb1 -> punteroDirecto = NULL; //bloques asociados 0
    fcb1 -> punteroIndirecto = NULL;
    //se crea nuevo bloque

    log_info(loggerFS, "Crear Archivo: %s ", nombreDelArchivo);
    return ok;
    //esta operación deberá devolver OK.

}
//Escritura de Archivo: “Escribir Archivo: <NOMBRE_ARCHIVO> - Puntero: <PUNTERO ARCHIVO> - Memoria: <DIRECCION MEMORIA> - Tamaño: <TAMAÑO>”
void escribirArchivo(archivo){
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
//Truncate de Archivo: “Truncar Archivo: <NOMBRE_ARCHIVO> - Tamaño: <TAMAÑO>”
void truncarArchivo(archivo, uint32_t tamanio){
    //Ampliar el tamaño del archivo (se agregan '/0')
	t_fcb fcb;
    if(fcb -> tamanioArchivo > tamanio){
    	ftruncate(archivo, tamanio); //o usar truncate
        fcb -> tamanioArchivo = tamanio; //Al ampliar actualizar el tamaño del archivo en el FCB
        //se le deberán asignar tantos bloques como sea necesario para poder direccionar el nuevo tamaño.
        asignarBloques();

    }else{//Reducir el tamaño del archivo
    	ftruncate(archivo, tamanio); //o usar truncate
        //Se deberá asignar el nuevo tamaño del archivo en el FCB
        fcb -> tamanioArchivo = tamanio;
        //deberán marcar como libres todos los bloques que ya no sean necesarios para direccionar el tamaño del archivo
        //(descartando desde el final del archivo hacia el principio).
        liberarBloque();//bitarray_clean_bit(bitMap, numeroBloque))
    }
    log_info(loggerFS, "Truncar Archivo: %s - Tamaño: %d", archivo, tamanio);

}
