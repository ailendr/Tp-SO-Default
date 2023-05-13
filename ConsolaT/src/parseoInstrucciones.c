/*
 * parseoInstrucciones.c
 *
 *  Created on: May 11, 2023
 *      Author: utnso
 */


#include "consola.h"
#include "estructuras.h"

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

/*
parseoDeInstrucciones(cont char *pathPseudo, ){
    FILE *archivoDeInstrucciones;
    archivoDeInstrucciones = fopen(pathPseudo, "r");
    char *auxInstrucciones; // para guardar las instrucciones leidas del archivo
    t_list listaDeInstrucciones = list_create();

    if (archivoDeInstrucciones == NULL) {
        printf("Error al abrir el archivo de pseudocodigo\n");
        return EXIT_FAILURE;
    }
    HAY QUE VERIFICAR QUE CADA INSTRUCCION SEA LAS QUE SE PIDE
    fscanf(archivoDeInstrucciones, "%s", auxInstrucciones);
    string_split (auxInstrucciones, "\n")








    agregar_a_paquete((t_paquete* paquete, void* valor, int tamanio);

    fclose(archivoInstrucciones);
    free(paquete)


}

   string_from_format // pasa a un string
     string_split
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
