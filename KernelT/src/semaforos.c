#include "semaforos.h"


sem_t planiLargoPlazo;
sem_t planiCortoPlazo;
sem_t multiprogramacion;///semaforo contador
sem_t cpuLibre;
pthread_mutex_t mutexReady;
pthread_mutex_t mutexNew;
pthread_mutex_t mutexPID;
pthread_mutex_t mutexUltimoEjecutado;
pthread_mutex_t mutexListaDeProcesos;


void inicializarSemaforos(){
	sem_init(&planiLargoPlazo ,0,0);
	sem_init(&planiCortoPlazo, 0,0);
	sem_init(&multiprogramacion,0, Multiprogramacion());
	sem_init(&cpuLibre,0,1);
	pthread_mutex_init(&mutexReady, NULL);
	pthread_mutex_init(&mutexNew, NULL);
	pthread_mutex_init(&mutexPID, NULL);
	pthread_mutex_init(&mutexUltimoEjecutado,NULL);
	pthread_mutex_init(&mutexListaDeProcesos,NULL);


}

void finalizarSemaforos(){
	sem_destroy(&planiLargoPlazo);
	sem_destroy(&planiCortoPlazo);
	sem_destroy(&multiprogramacion);
    sem_destroy(&cpuLibre);

	pthread_mutex_destroy(&mutexReady);
	pthread_mutex_destroy(&mutexNew);
	pthread_mutex_destroy(&mutexPID);
}
