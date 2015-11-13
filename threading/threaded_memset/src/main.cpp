// A simple test of threading performance.

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <pthread.h>
#include <string.h>
#include <time.h>

// TODO: Add semaphores so the threading test does not start it's next memset until all thread are finished.

struct test_info {
    uint8_t* start;
    uint32_t length;
    uint8_t value;
    uint32_t times_to_set;  
};

uint64_t get_time_ms() {
    struct timespec te;
    clock_gettime(CLOCK_MONOTONIC, &te);

    return te.tv_sec * 1000L + te.tv_nsec/1000000L;
}

void memset_test(void* arr_info) {
    // Cast here to avoid giving the non-pthread version an advantage.
    test_info* info = (test_info*)arr_info;
    for(int test = 0; test < info->times_to_set; test++) {
        memset((void*)info->start, info->value, info->length);
    }   
}

void* memset_pthread(void* info) {
    memset_test(info);
    pthread_exit(NULL);
}

int32_t main(int32_t argc, char *argv[]) {
    const uint32_t NUM_RUNS = 1000;
    // Arbitrarily large number
    uint32_t arr_size = (1 << 24);
    max_conn_threads = sysconf(_SC_NPROCESSORS_ONLN); 

    // Testing at the max number of threads that benefit from true concurrency
    if(max_conn_threads < 1) {
        std::cout << "Error reading threading capabilities, exiting...\n";
        exit(EXIT_FAILURE);
    }

    //Ensure arr_size is divisible by the number of threads
    arr_size -= arr_size % max_conn_threads;
    uint8_t* array = (uint8_t*)malloc(arr_size);
    std::cout << "\n++++\nTesting 1 thread versus " << max_conn_threads << " for memset with " << NUM_RUNS << " runs for an array of size " << arr_size << " with frequent thread creation\n\n";
    

    // First test a loop where each iteration creates a new thread and the function memset's once.
    test_info info;
    info.start = array;
    info.length = arr_size;
    info.value = 0;
    info.times_to_set = 1;

    uint64_t start = get_time_ms();
    for(int unused = 0; unused < NUM_RUNS; unused++) {
        memset_test((void*)&info);
    }
    uint64_t end = get_time_ms();
    std::cout << "Single-threaded ran in " << end - start << "ms\n";

    test_info* info_arr = (test_info*)malloc(max_conn_threads * sizeof(test_info));
    uint32_t len_per_thread = arr_size/max_conn_threads;
    for(int index = 0; index < max_conn_threads; index++) {
        info_arr[index].start = array + index * len_per_thread;
        info_arr[index].length = len_per_thread;
        info_arr[index].value = 0;
        info_arr[index].times_to_set = 1;
    }

    pthread_t* thread_ids = (pthread_t*)malloc(max_conn_threads * sizeof(pthread_t));
    start = get_time_ms();
    for(int unused = 0; unused < NUM_RUNS; unused++) {
        for(int index = 0; index < max_conn_threads; index++) {
            pthread_create(&thread_ids[index], NULL, memset_pthread, &info_arr[index]);
        }

        for(int index = 0; index < max_conn_threads; index++) {
            pthread_join(thread_ids[index], NULL);
        }
    }
    end = get_time_ms();

    uint64_t many_creation_time = end - start;

    std::cout << "Multi-threaded ran in " << many_creation_time << "ms with " << max_conn_threads << " threads\n";


    // Now test a single call where the function loops to reduce thread creation overhead
    std::cout << "\n++++\nTesting 1 thread versus " << max_conn_threads << " for memset with " << NUM_RUNS << " runs for an array of size " << arr_size << " with upfront thread creation\n\n";
    info.times_to_set = NUM_RUNS;
    start = get_time_ms();
    memset_test((void*)&info);
    end = get_time_ms();

    std::cout << "Single-threaded ran in " << end - start << "ms\n";

    for(int index = 0; index < max_conn_threads; index++) {
        info_arr[index].times_to_set = NUM_RUNS;
    }

    start = get_time_ms();
    for(int index = 0; index < max_conn_threads; index++) {
        pthread_create(&thread_ids[index], NULL, memset_pthread, &info_arr[index]);
    }
    for(int index = 0; index < max_conn_threads; index++) {
        pthread_join(thread_ids[index], NULL);
    }
    end = get_time_ms();

    uint64_t single_creation_time = end - start;
    std::cout << "Multi-threaded ran in " << single_creation_time << "ms with " << max_conn_threads << " threads\n\n";

    std::cout << "Thread creation overhead was approximately " << many_creation_time - single_creation_time << "ms\n";
    
    free(thread_ids);
    free(info_arr);
    free(array);
    return 0;
}
