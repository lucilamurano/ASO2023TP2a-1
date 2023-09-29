#include <pthread.h>
#include <stdio.h>
#define NUMBER_OF_THREADS 2
#define CANTIDAD_INICIAL_HAMBURGUESAS 20

int cantidad_restante_hamburguesas = CANTIDAD_INICIAL_HAMBURGUESAS;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int turno = 0;

void *comer_hamburguesa(void *tid) {
    int comensal_id = (int)tid;

    while (1) {
        pthread_mutex_lock(&mutex);

        if (cantidad_restante_hamburguesas > 0) {
            if (comensal_id == turno) {
                printf("Hola! soy el hilo(comensal) %d , me voy a comer una hamburguesa ! ya que todavia queda/n %d \n", comensal_id, cantidad_restante_hamburguesas);
                cantidad_restante_hamburguesas--; // Me como una hamburguesa
                turno = (turno + 1) % NUMBER_OF_THREADS; // Cambiamos el turno
            }
        } else {
            printf("SE TERMINARON LAS HAMBURGUESAS :( \n");
            pthread_mutex_unlock(&mutex);
            pthread_exit(NULL);
        }

        pthread_mutex_unlock(&mutex);
    }
}

int main() {
    pthread_t threads[NUMBER_OF_THREADS];
    int status, i, ret;

    for (int i = 0; i < NUMBER_OF_THREADS; i++) {
        printf("Hola!, soy el hilo principal. Estoy creando el hilo %d \n", i);
        status = pthread_create(&threads[i], NULL, comer_hamburguesa, (void *)i);
        if (status != 0) {
            printf("Algo salio mal, al crear el hilo recibi el codigo de error %d \n", status);
            exit(-1);
        }
    }

    for (i = 0; i < NUMBER_OF_THREADS; i++) {
        void *retval;
        ret = pthread_join(threads[i], &retval); // Espero por la terminacion de los hilos que cree
    }

    pthread_exit(NULL);
}