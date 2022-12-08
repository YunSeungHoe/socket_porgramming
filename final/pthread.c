#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

void* tmain(void* arg);

int sum = 0;
pthread_mutex_t mtx;

int main(int arg, char* argv){
    int cnt1 = 5000000;
    int cnt2 = 5000000;
    pthread_t t_id1, t_id2;
    void* ret;

    pthread_mutex_init(&mtx, NULL);
    pthread_create(&t_id1, NULL, &tmain, (void*)&cnt1);
    pthread_create(&t_id2, NULL, &tmain, (void*)&cnt2);
    pthread_join(t_id1, NULL);
    pthread_join(t_id2, NULL);
    printf("sum = %d\n", sum);
    return 0;
}

void* tmain(void* arg){
    int n = *((int*)arg);
    for(int i = 0; i < n; i++){
        pthread_mutex_lock(&mtx);
        sum += 1;
        pthread_mutex_unlock(&mtx);
    }
}