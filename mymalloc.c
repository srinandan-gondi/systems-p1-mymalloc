#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"
#include <stdint.h>

#define MEMLENGTH 4096
// is this excluding or including the first 8 bytes of the heap?

static union {

    char bytes[MEMLENGTH];
    double not_used;

} heap;

//testing commits now

static int initialized = 0;

static void leak_detect(){

}

static void init(){
    
    initialized = 1;
    
    atexit(leak_detect);

    static union {

        char bytes[MEMLENGTH];
        double not_used;

    } heap;

    char* size = &heap.bytes[0];
    *(int*)size = 4088;
    
    size = size + 4;

    *(int*)size = 0;
    
}

void* mymalloc(size_t size, char* file, int line){
    if(!initialized){
        init();
    }

    size_t act_size = ((size+7) & ~7);

    if(act_size > 4088){
        printf("malloc: Unable to allocate %zu bytes (%s:%d)\n", size, file, line);
        return NULL;
    }

    int curr = MEMLENGTH;
    char* temp = heap.bytes;
    
    int curr_block_size = 0;
    
    while(curr > 0){
        
        if(act_size <= *(int*)temp && !*(int*)(temp+4)){
            
            curr_block_size = *(int*)temp;   

            *(int*)temp = (int)act_size;
            *(int*)(temp+4) = 1;

            temp = temp + 8 + *(int*)temp;

            *(int*)temp = curr_block_size - (int)act_size;
            *(int*)(temp+4) = 0;

            char* aligned_ptr = (char*)(((uintptr_t)(temp+4) + 7) & ~0x7);
            
            return (void*)aligned_ptr;
        }

        curr = curr - 8 - *(int*)temp;
        temp = temp + 8 + *(int*)temp;
    }

    printf("malloc: Unable to allocate %zu bytes (%s:%d)\n", size, file, line);
    return NULL;

}

void myfree(void* ptr, char* file, int line){
    
    if(!initialized){
        init();
    }

    if(ptr == NULL){
        printf("free: Inappropriate pointer (%s:%d)\n", file, line);
        return;
    }

    char* cptr = (char*)ptr;  // Convert to char pointer

    // Traverse the heap to find the original allocation
    char* heap_end = heap.bytes + MEMLENGTH;
    
    int checker = 0;

    for (char* curr = heap.bytes + 8; curr < heap_end; ) {
        // Align the pointer the same way as in mymalloc
        char* aligned = (char*)(((uintptr_t)curr + 7) & ~0x7);

        int* header = (int*)(aligned - 8);  // Get the header
        
        // If this aligned pointer matches the one passed to free, it's valid
        if (aligned == cptr) {
            
            checker = 1;

            int chunk_size = header[0];

            // Optional: Check if it's already freed or invalid
            if (header[1] == 0){
                printf("free: Inappropriate pointer (%s:%d)\n", file, line);
                // return;
            }   // Already freed
           
            // Mark it as free
            header[1] = 0;  
            
            // return;
        }

            
        int* temp = header + 8 + header[0];

        if(temp[1] == 0){
            header[0] = header[0] + temp[0];
        }
                
                
                
            



        // Move to the next chunk (use the size stored in the header)
        
        int chunk_size = header[0];

        // Move by the full chunk size (header + data)
        curr += chunk_size + 8;  
    }

    if(!checker){
        printf("free: Inappropriate pointer (%s:%d)\n", file, line);
    }

    // need to figure out how to coalesce adjacent free chunks after freeing
    
}



int main(){
    
    init();

    printf("heap.bytes[0] is %d\n", *(int*)&heap.bytes[0]);
    printf("heap.bytes[1] is %d\n", *(int*)&heap.bytes[4]);
    
    return EXIT_SUCCESS;
}





