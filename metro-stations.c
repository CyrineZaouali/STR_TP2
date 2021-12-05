#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>



#define AB 0
#define BC 1
#define CD 2
#define EB 3
#define FA 4

const int NB_METROS = 10;

struct thread_data {
    int train_id;
    int segment;
};

struct thread_data thread_data_array[10];

pthread_mutex_t mutex_seg[5];

void* metro(void* thread_arg);
void wait(double time);

void* metro(void* thread_arg) {
    struct thread_data *data;
    data = (struct thread_data*) thread_arg; 
    pthread_mutex_lock(&mutex_seg[data->segment]);
    printf("Metro %d engaging segment %d\n", data->train_id, data->segment);
    wait(6.0);
    printf("Metro %d leaving segment %d\n", data->train_id, data->segment);
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
    pthread_t t;

    for(i=0; i<5; i++) {
        pthread_mutex_init(&mutex_seg[i],0);
    }
    
    for(i=0; i<NB_METROS; i++) {
        random_segment = (rand() % 5);
        thread_data_array[i].train_id = i;
        thread_data_array[i].segment = random_segment;
        pthread_create(&t, NULL, metro, &thread_data_array[i]);
    }
    pthread_join(t, NULL);
    
    return 0;
}