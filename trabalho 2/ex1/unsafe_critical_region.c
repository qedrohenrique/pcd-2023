/*
  Pedro Henrique de Almeida  - 148526
  Matheus Ferreira Santos - 148410
  Rafaela Cristine dos Santos Uchôas - 140351
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 4

int request = 0;
int respond = 0;
int soma = 0;

void* client(void *arg){
    int i = *((int*) arg) + 1; 
    while(1){
        // while(respond != i)
        //     request = i;

        // Região crítica
        int local = soma;
        sleep(rand()%2);
        soma = local + 1;
        printf("[+] cliente %d - antes: %d - depois: %d\n", i, local, soma);

        // respond = 0;
    }
}

void server(){
    printf("[+] server on!\n");
    while(1){
        // while(request == 0);
        respond = request;

        // Região Crítica   

        // while(respond != 0);
        // request = 0;
    }
}

void create_threads(pthread_t p_id[NUM_THREADS]){
    for(int i = 0; i < NUM_THREADS; i++){
        int* arg = (int*) malloc(sizeof(int));
		*arg = i;
		pthread_create(&(p_id[i]), NULL, client, (void*) arg);
    }
}

int main (){
    pthread_t p_id[NUM_THREADS];

    create_threads(p_id);
    server();

    return 0;
}