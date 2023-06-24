#include <semaphore.h>
#include <pthread.h>
#include <stdint.h>
#include "kernelConfig.h"

extern sem_t planiLargoPlazo;
extern sem_t planiCortoPlazo;
extern sem_t multiprogramacion;///semaforo contador
extern sem_t cpuLibre;

extern pthread_mutex_t mutexReady;
extern pthread_mutex_t mutexNew;
extern pthread_mutex_t mutexPID;
extern pthread_mutex_t mutexUltimoEjecutado;


void inicializarSemaforos();
void finalizarSemaforos();
