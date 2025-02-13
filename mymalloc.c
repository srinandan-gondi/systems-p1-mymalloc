#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"

#define MEMLENGTH 4096
// is this excluding or including the first 8 bytes of the heap?

static union {

    char bytes[MEMLENGTH];
    double not_used;

} heap;

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
    
    while(curr > 0){
        
        if(act_size <= *(int*)temp && !*(int*)(temp+4)){
            *(int*)temp = (int)act_size;
            temp = temp + 4;
            *(int*)temp = 1;

            return (void*)(temp+4);
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

    // need to figure out how to coalesce adjacent free chunks after freeing
    
}



int main(){
    
    init();

    printf("heap.bytes[0] is %d\n", *(int*)&heap.bytes[0]);
    printf("heap.bytes[1] is %d\n", *(int*)&heap.bytes[4]);
    
    return EXIT_SUCCESS;
}





// while(curr < MEMLENGTH){
    //     if(heap.bytes[curr] != 0){
    //         int i = 0;
    //         while(i < size){
    //             if(heap.bytes[curr + i] != 0){
    //                 break;
    //             }
    //             i++;
    //         }
    //         if(i == size){
    //             for(int j = 0; j < size; j++){
    //                 heap.bytes[curr + j] = 1;
    //             }
    //             return (void*) &heap.bytes[curr];
    //         }
    //     }
    //     curr++;
    // }