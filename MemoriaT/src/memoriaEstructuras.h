/*
 * memoriaEstructuras.h
 *
 *  Created on: Jun 10, 2023
 *      Author: utnso
 */

#ifndef SRC_MEMORIAESTRUCTURAS_H_
#define SRC_MEMORIAESTRUCTURAS_H_

#include "memoriaConfig.h"
#include <stdlib.h>
#include <stdint.h>
#include <commons/collections/list.h>
#include <utils/estructuras.h>

extern void* memoriaContigua;
extern t_segmento* segmentoCero;
void iniciarEstructuras();
void crearTablasDeSegmentos();



#endif /* SRC_MEMORIAESTRUCTURAS_H_ */
