#include <stdlib.h>
#include <stdio.h>
#include "mymalloc.h"

int main(){
    // Call Functions to test methods
    return 0;
}

// Test basic functionality of free 
void basicFreeTest(){
    void *ptr1 = malloc(100);
    void *ptr2 = malloc(100);

    free(ptr1);
    free(ptr2);

    void *ptr3 = malloc(200);
    free(ptr3);
}

// Test basic functionality of malloc 
void basicMallocTest(){
    int* ptr1 = malloc(10);
    *ptr1 = 10;
    printf("Value of ptr: %d\n", *ptr1);
}

// Test freeing a misaligned pointer (not at the start of an allocated block).
// Should trigger an "invalid address" error.
void misalignedPointerTest(){
    int *ptr2 = malloc(1000);
    free((char*)ptr2 + 1); //Return invalid address error
    free(ptr2);
}

// Test freeing a pointer that wasn't allocated by malloc().
// Should trigger an "invalid pointer" error.
void pointerNotInHeapTest(){
    int x;
    free(&x); //Return invalid pointer error
}

// Test freeing the same pointer twice.
// Should trigger a "double free" error.
void doubleFreeTest(){
    int *ptr = malloc(10);
    free(ptr);
    free(ptr); //Double free error
}

// Test for memory leaks by allocating without freeing.
// Leak detection should report allocated but unreleased memory.
void leakCheckerTest(){
    malloc(10);
}

// Test that two allocated blocks do not overlap.
// Should print correct, non-overlapping values for ptr1 and ptr2.
void overlapTest(){
    int* ptr1 = malloc(10);
    int* ptr2 = malloc(10);

    *ptr1 = 0;
    *ptr2 = 1;

    printf("Value of ptr1: %d\n", *ptr1);
    printf("Value of ptr2: %d\n", *ptr2);
}

// Test coalescing of adjacent free blocks into a larger block.
// Allocates and frees several blocks, then checks large allocation.
void coalesceTest(){
    int size = 4;
    int* ptrs[size];
    for( int i = 0; i<size; i++){
        ptrs[i] = malloc(1000);
    }
    
    for(int i = 0; i<size; i++){
        free(ptrs[i]);
    }

    int* ptr = malloc(4000);
    free(ptr);
}

// Test allocating nearly the entire heap and then a small chunk.
// Ensures the allocator prevents exceeding the heap boundary.
void restrictedAreaTest(){
    int* ptr = malloc(4088);
    malloc(8);
}

// Test allocating and freeing non-contiguous blocks to create fragmentation.
// Verifies whether fragmented free space can be coalesced or reused efficiently.
void fragmentedTest(){
    int* ptr1 = malloc(2000);
    int* ptr2 = malloc(50);
    int* ptr3 = malloc(2000);

    free(ptr1);
    free(ptr3);

    int* ptr4 = malloc(3000);
    free(ptr2);
}