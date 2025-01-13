#include <stdlib.h>
#include <stdio.h>
#include "mymalloc.h"
#include <sys/time.h>

#define RUNS 50
#define COUNT 120


long get_time(struct timeval start, struct timeval end){
    return ((end.tv_sec - start.tv_sec) * 1000000L + (end.tv_usec - start.tv_usec));
}

void workload1() {
    for (int i = 0; i < COUNT; i++) {
        char *ptr = (char *)malloc(1);
        free(ptr);
    }
}

void workload2(){
    char *array[COUNT];
    for(int i = 0; i < COUNT; i++){
        array[i] = (char*)malloc(1);
    }
    for(int i = 0; i < COUNT; i++){
        free(array[i]);
    }
}
void workload3(){
    char *array[COUNT];
    int allocCount = 0;
    for(int i = 0; i < COUNT; i++){
        for(int i = 0; i < COUNT; i++){
            if((int)rand() % 2 == 0 && allocCount < COUNT){
                array[allocCount] = (char*)malloc(1);
                if(array[allocCount] != NULL){
                    allocCount++;
                }
            }
            else if(allocCount > 0){
                free(array[--allocCount]);
            }
        }
    }
    for(int i = 0; i < allocCount; i++){
        if(array[i] != NULL){
            free(array[i]);
        }
    }
}

// Allocate and deallocate progressively larger blocks of memory
void workload4(){
    for(int i = 0; i < 50; i++){
        char *ptr = (char *)malloc(i);
        free(ptr);
    }
}

// Allocate a random number of bytes between 1 and 100 40 times and then deallocate
void workload5(){
    for(int i = 0; i < 40; i++){
        char *ptr = (char*)malloc(rand() % (41));
        free(ptr);
    }
}

int main(){
    struct timeval start;
    struct timeval end;

    long total_time;

    total_time = 0;
    gettimeofday(&start, NULL);
    for(int i = 0; i < RUNS; i++){
        
        workload1();
        workload2();
        workload3();
        workload4();
        workload5();
    }
    gettimeofday(&end, NULL);
    total_time = get_time(start, end);
    printf("Workload time: %1d microseconds\n", total_time/RUNS);
}