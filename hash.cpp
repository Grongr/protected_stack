#include "stack.h"
#include "hash.h"

size_t StkHash(const Stack* stk) {

    size_t h = 5381;

    unsigned char* start = (unsigned char*)stk + sizeof(canary_t);
    unsigned char* stop  = (unsigned char*)stk + sizeof(canary_t) +
                           sizeof(stk->data) + sizeof(stk->capacity) +
                           sizeof(stk->size) + sizeof(stk->element_size);

    for (; start != stop; ++start)
        h = ((h << 5) + h) + *start;

    h = (h & 0xffffffff);
    
    return h;
}

size_t DataHash(const Stack* stk) {

    size_t h = 5381;

    unsigned char* start = (unsigned char*)stk->data;
    unsigned char* stop  = (unsigned char*)stk->data +
                           stk->capacity * stk->element_size;

    for (; start != stop; ++start)
        h = ((h << 5) + h) + *start;

    h = (h & 0xffffffff);

    return h;
}

size_t HashFunc(const void* beg, size_t size) {

    size_t h = 5381;

    unsigned char* iter = (unsigned char*) beg;
    for (size_t i = 0; i < size; ++i)
        h = ((h << 5) + h) + iter[i];

    return (h & 0xffffffff);
}
