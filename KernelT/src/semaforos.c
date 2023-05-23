#include "semaforos.h"


sem_t procesoNuevo;
sem_t procesoReady;

pthread_mutex_t mutexReady;
pthread_mutex_t mutexNew;
pthread_mutex_t mutexPID;
pthread_mutex_t mutexMultiprogramacion;

void inicializarSemaforos(){
	sem_init(&procesoNuevo, 0,0);
	sem_init(&procesoReady, 0,0);

	pthread_mutex_init(&mutexReady, NULL);
	pthread_mutex_init(&mutexNew, NULL);
	pthread_mutex_init(&mutexPID, NULL);
	pthread_mutex_init(&mutexMultiprogramacion, NULL);
}
