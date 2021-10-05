#include <stdio.h>
#include "mem_copy.h"

MemErrors mem_copy(void* pa, void* pb, size_t size) {

    if (!pa || !pb)
        return INVALID_POINTER;

    size_t s_tmp = size;

    do {
        
        if (s_tmp >= sizeof(size_t)) {

            s_tmp -= sizeof(size_t);

            size_t* a = (size_t*)pa;
            size_t* b = (size_t*)pb;

            *a = *b;

            pa = (void*)(a + 1);
            pb = (void*)(b + 1);

        } else if (s_tmp >= sizeof(int)) {
            
            s_tmp -= sizeof(int);

            int* a = (int*) pa;
            int* b = (int*) pb;

            *a = *b;

            pa = (void*)(a + 1);
            pb = (void*)(b + 1);

        } else if (s_tmp >= sizeof(short int)){

            s_tmp -= sizeof(short int);

            short int* a = (short int*) pa;
            short int* b = (short int*) pb;

            *a = *b;

            pa = (void*)(a + 1);
            pb = (void*)(b + 1);


        } else {

            s_tmp--;

            char* a = (char*) pa;
            char* b = (char*) pb;

            *a = *b;

            pa = (void*)(a + 1);
            pb = (void*)(b + 1);
        }

    } while (s_tmp > 0);

    return ALL_OK;
}
