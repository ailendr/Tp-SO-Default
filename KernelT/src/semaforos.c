#include "semaforos.h"


sem_t planiLargoPlazo;
sem_t planiCortoPlazo;
sem_t multiprogramacion;///semaforo contador
sem_t productorListaReady;
sem_t productorColaNew;
sem_t cpuOcupada;
pthread_mutex_t mutexReady;
pthread_mutex_t mutexNew;
pthread_mutex_t mutexPID;



void inicializarSemaforos(){
	sem_init(&planiLargoPlazo ,0,0);
	sem_init(&planiCortoPlazo, 0,0);
	sem_init(&productorListaReady,0, 0);
	sem_init(&productorColaNew,0,0);
	sem_init(&multiprogramacion,0, Multiprogramacion());
	sem_init(&cpuOcupada,0, 1);

	pthread_mutex_init(&mutexReady, NULL);
	pthread_mutex_init(&mutexNew, NULL);
	pthread_mutex_init(&mutexPID, NULL);

}

void finalizarSemaforos(){
	sem_destroy(&planiLargoPlazo);
	sem_destroy(&planiCortoPlazo);
	sem_destroy(&multiprogramacion);
	sem_destroy(&productorListaReady);
	sem_destroy(&productorColaNew);
	sem_destroy(&cpuOcupada);

	pthread_mutex_destroy(&mutexReady);
	pthread_mutex_destroy(&mutexNew);
	pthread_mutex_destroy(&mutexPID);
}
