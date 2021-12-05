#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


#define NB_TRAINS 10;

#define AB 0
#define BC 1
#define CD 2
#define EB 3
#define FA 4

struct thread_data {
    int train_id;
    int segment;
};

struct thread_data thread_data_array[10];

pthread_mutex_t mutex_seg[5];

void* train(void* thread_arg);
void wait(double time);

void* train(void* thread_arg) {
    struct thread_data *data;
    data = (struct thread_data*) thread_arg; 
    pthread_mutex_lock(&mutex_seg[data->segment]);
    printf("Train %d engaging metro segment %d\n", data->train_id, data->segment);
    wait(6.0);
    printf("Train %d leaving metro segment %d\n", data->train_id, data->segment);
    pthread_mutex_unlock(&mutex_seg[data->segment]);
    pthread_exit(NULL);
}


void wait(double time){
    struct timespec delay;
    delay.tv_sec=time;
    delay.tv_nsec=0;
    nanosleep(&delay,NULL);
}

int main(int argc, char* argv[]) {
    int i, random_segment;

    for(i=0; i<5; i++) {
        pthread_mutex_init(&mutex_seg[i],0);
    }
    
    for(i=0; i<10; i++) {
        pthread_t t;
        random_segment = (rand() % 5);
        thread_data_array[i].train_id = i;
        thread_data_array[i].segment = random_segment;
        pthread_create(&t, NULL, train, &thread_data_array[i]);
        pthread_join(t, NULL);
    }
    
    return 0;
}