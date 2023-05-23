#include <semaphore.h>
#include <pthread.h>
#include <stdint.h>

extern sem_t procesoNuevo;
extern sem_t procesoReady;

extern pthread_mutex_t mutexReady;
extern pthread_mutex_t mutexNew;
extern pthread_mutex_t mutexPID;
extern pthread_mutex_t mutexMultiprogramacion;



void inicializarSemaforos();
