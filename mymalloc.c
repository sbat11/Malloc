#include <stdlib.h>
#include <stdio.h>
#include "mymalloc.h"

#define NEXT_POINTER md + (md->size / sizeof(metadata))
#define MEMLENGTH 4096

// Create "heap"
static union {
char bytes[MEMLENGTH];
double not_used;
} heap;
int initialized = 0;

// Struct that handles metadata in 8 bytes
typedef struct metadata{
    int allocated;
    int size;
} metadata;

// Method for atexit to check for memory leaks
void leakCheck(void){
    char* startingAddress = (char*)(&heap);
    char* maxAddress = startingAddress + MEMLENGTH;
    int totalBytes = 0;
    int totalChunks = 0;

    metadata *md = &heap;
    // Iterate through all metadata and check for data that has not been freed
    while(md < maxAddress) {
        if(md->allocated){
            totalChunks++;
            totalBytes += md->size;
        }
        md = (metadata*)((char*)(md) + sizeof(metadata) + md->size);
    }

    if(totalChunks > 0 || totalBytes > 0){
            fprintf(stderr, "mymalloc: %d bytes leaked in %d object(s).\n", totalBytes, totalChunks);
    }
}
// Method to initialize memory
void initializeMemory(){
    initialized = 1;
    metadata* head = &heap.bytes;
    head->allocated = 0;
    head->size = MEMLENGTH-sizeof(double);

    // printf("MEMORY INITIALIZED\n");
    // printf("\n");

    atexit(leakCheck);
}


void *mymalloc(size_t size, char *file, int line){
    if(!initialized){
        initializeMemory();
    }
    char* startingAddress = (char*)(&heap);
    char* maxAddress = startingAddress + MEMLENGTH;
    

    // All data must be aligned to 8 bytes
    int allocatedSize = (size+7) & ~7;

    // Iterate through pointers until you find one that is both free and has enough space
    // If metadata pointer is outside of heap then display error and return null 
    metadata *md = &heap;
    while(md->allocated || md->size < allocatedSize) {
        // Go to next metadata
        md = (metadata*)((char*)(md) + md->size + sizeof(metadata));

        if(md >= maxAddress){
            fprintf(stderr, "malloc: Unable to allocate %d bytes (%s:%d)\n", size, file, line);
            return NULL;
        }
    }

    if(md >= maxAddress){
            fprintf(stderr, "malloc: Unable to allocate %d bytes (%s:%d)\n", size, file, line);
            return NULL;
    }
    
    // Precondition not allocated and md->size >= allocatedSize 
    md->allocated = 1;
    md->size = allocatedSize;
    char* a1 = (char*)(md) - startingAddress;

    // printf("Normalized Address: %d, ", a1);
    // printf("Allocated: %d, ", md->allocated);
    // printf("Size of payload: %d, ", md->size);
    // printf("Address: %p\n", md);
    // printf("\n");

    // Creating metadata that points to the free space 
    metadata* freemd = (metadata*)((char*)(md) + md->size + sizeof(metadata));
    freemd->allocated = 0;
    freemd->size = maxAddress - (char*)(freemd) - sizeof(metadata);
    
    char* a2 = (char*)(freemd) - startingAddress;
    // printf("Normalized Address: %d, ", a2);
    // printf("Allocated: %d, ", freemd->allocated);
    // printf("Size of payload: %d, ", freemd->size);
    // printf("Address: %p\n", freemd);
    // printf("\n");

    return md;

}

void coalesceFreeMemory(){
    char* startingAddress = (char*)(&heap);
    char* maxAddress = startingAddress + MEMLENGTH;

    // Keep track of previous freed metadata and combine contiguous freed chunks
    metadata *md = &heap;
    metadata* prev = NULL;
    while(md < maxAddress) {
        if(!md->allocated){
            if (prev != NULL){  
                prev->size = prev->size + md->size + sizeof(metadata);
            }
            else{
                prev = md;
            }
        }
        else {
            prev = NULL;
        }
        md = (metadata*)((char*)(md) + sizeof(metadata) + md->size);
    }
}


void myfree(void *ptr, char *file, int line){
    char* startingAddress = (char*)(&heap);
    char* maxAddress = startingAddress + MEMLENGTH;

    // initialize memory
    if(!initialized){
        initializeMemory();
        initialized = 1;
    }

    // null pointer
    if(ptr == NULL){
        fprintf(stderr, "free: Inappropriate pointer (null pointer) (%s:%d)\n", file, line);
        exit(2);
    }

    // freeing something not in heap
    if(((char*) ptr < startingAddress) || ((char*) ptr >= maxAddress)){
        fprintf(stderr, "free: Inappropriate pointer (out of bounds) (%s:%d)\n", file, line);
        exit(2);
    }

    // freeing pointer not aligned to 8 bytes
    if ((size_t)ptr % 8) {
        fprintf(stderr, "free: Inappropriate pointer (misaligned) (%s:%d)\n", file, line);
        exit(2);
    }
    
    // Precondition: valid pointer 
    metadata *md = &heap;
    while(md < maxAddress) {
        // Go to next metadata
        if(md == (metadata*)ptr){
            if (md->allocated){
                md->allocated = 0;
                coalesceFreeMemory();
            }
            else{
                fprintf(stderr, "free: Inappropriate pointer (double free) (%s:%d)\n", file, line);
                exit(2);
            }
        }
        md = (metadata*)((char*)(md) + sizeof(metadata) + md->size);
    }
}

// int main(){
//     char* max = (char*)(&heap) + MEMLENGTH;
//     printf("Starting Address: %p\n", &heap);
//     printf("Max Address: %p\n\n", max);

//     char* p1 = malloc(2000);
//     char* p2 =malloc(50);
//     char* p3 =malloc(2000);

//     printf("P1 pointer: %p\n", p1);
//     printf("P2 pointer: %p\n", p2);
//     printf("P3 pointer: %p\n", p3);

//     free(p1);
//     free(p2);
//     free(p3);

//     char* p4 = malloc(4000);
//     //free(p4);
//     return EXIT_SUCCESS;
// }