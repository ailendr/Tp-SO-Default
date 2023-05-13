/*
 * parseoInstrucciones.c
 *
 *  Created on: May 11, 2023
 *      Author: utnso
 */


#include "consola.h"


t_paquete* parseoDeInstrucciones(char *pathInstrucciones){
    FILE *archivoDeInstrucciones;
    archivoDeInstrucciones = fopen(pathInstrucciones, "r");
    char *auxInstrucciones; // para guardar las instrucciones leidas del archivo
    t_list* listaDeInstrucciones = list_create();
    t_paquete* paqueteConInstrucciones = crear_paquete();

    if (archivoDeInstrucciones == NULL) {
        printf("Error al abrir el archivo de pseudocodigo\n");
        return EXIT_FAILURE;
    }

    fscanf(archivoDeInstrucciones, "%s", auxInstrucciones);
    string_split (auxInstrucciones, "\n");
    int tamanioAuxInstruciones = sizeof(auxInstrucciones);
    for (int i = 0; i < tamanioAuxInstruciones; i++){
    	list_add(listaDeInstrucciones, auxInstrucciones[i]);
    }
    //luego agrego la lista a un paquete
    int tamanioListaInstrucciones = list_size(listaDeInstrucciones);
    for (int i = 0; i < tamanioListaInstrucciones; i++){

    	//agregar_a_paquete(paqueteConInstrucciones, void* valor, tamanio);
    }

    fclose(archivoDeInstrucciones);
    eliminar_paquete(paqueteConInstrucciones);

    return paqueteConInstrucciones;

}

/*
typedef struct {
    t_instruc tipoInstruccion;
    uint32_t operando1;
    uint32_t operando2;
    uint32_t operando3;
}instruccion;

instruccion* crearInstruccion(t_instruc tipoDeInstruccion,uint32_t parametro1, uint32_t parametro2, uint32_t parametro3){
    instruccion* nuevaInstruccion = malloc(sizeof(instruccion));
    nuevaInstruccion -> tipoInstruccion = tipoDeInstruccion;
    nuevaInstruccion -> operando1 = parametro1;
    nuevaInstruccion -> operando2 = parametro2;
    nuevaInstruccion -> operando3 = parametro3;
    return nuevaInstruccion;
}

*/

/*
	Lee archivo y crea un string largo con todas las instrucciones (despues se podría usar string_split y usar \n como separador)
	char* leerArchivo(char* pathPseudo){
		FILE *pseudocodigo = fopen(pathPseudo, "r");
		if (pseudocodigo == NULL){
			return NULL;
		}

		fseek(pseudocodigo, 0, SEEK_END);
		int largo = ftell(pseudocodigo);
		fseek(pseudocodigo, 0, SEEK_SET);

		char *instrucciones = malloc(sizeof(char) * (length+1));

		char c;
		int i = 0;
		while ( (c = fgetc(pseudocodigo)) != EOF){
			string[i] = c;
			i++;
		}
		instrucciones[i]= '\0';

		fclose(pseudocodigo);

		return instrucciones;
	}
*/
