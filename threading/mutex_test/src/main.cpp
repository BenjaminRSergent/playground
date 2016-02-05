// A simple test of threading performance.

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <mutex>

std::mutex mut;

void* mutex_test(void* dat) {
    mut.lock(); 
    std::cout << "Locked\n";
    usleep(1000 * 1000);
    mut.unlock();
    std::cout << "Unlocked\n";

    return NULL;
}

int32_t main(int32_t argc, char *argv[]) {
    const int NUM_THREADS = 4;
    pthread_t* thread_ids = (pthread_t*)malloc(NUM_THREADS * sizeof(pthread_t));
    for(int index = 0; index < NUM_THREADS; index++) {
        pthread_create(&thread_ids[index], NULL, mutex_test, NULL);
    }

    for(int index = 0; index < NUM_THREADS; index++) {
        pthread_join(thread_ids[index], NULL);
    }
    
    free(thread_ids);
    return 0;
}
