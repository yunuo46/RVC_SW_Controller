#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#include "rvc_sw.h"

void* input_thread(void* arg) {
    printf("Sensor Input (Front Left Right Back Dust)\n");
    printf("[1: true, 0: false] [Example: 1 0 1 0 1]\n");

    pthread_t current_thread = pthread_self();
    pthread_cleanup_push(thread_cleanup_handler, (void*)&current_thread);
    while (1) {
        sensors_input();
        nanosleep(&req, NULL);
    }
    pthread_cleanup_pop(0);
    return NULL;
}

void* controller_thread(void* arg) {
    pthread_t current_thread = pthread_self();
    pthread_cleanup_push(thread_cleanup_handler, (void*)&current_thread);
    while (1) {
        controller();
        nanosleep(&req, NULL);
    }
    pthread_cleanup_pop(0);
    return NULL;
}

int main() {
    init();

    // Create Threads
    if (pthread_create(&inputThread, NULL, input_thread, NULL) != 0 ||
        pthread_create(&controllerThread, NULL, controller_thread, NULL) != 0) {
        perror("Error creating thread");
        return 1;
    }

    // Wait for Threads
    pthread_join(inputThread, NULL);
    pthread_join(controllerThread, NULL);
    printf("Both threads have finished. Exiting main program.\n");
    return 0;
}