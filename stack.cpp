#include <stdlib.h>
#include "mem_copy.h"
#include "stack.h"

#define POINTER_UNINIT (void*)0x66600666
#define POINTER_POISON (void*)0x8BADF00D

#define CODE_CHECK(FUNC, OK_CODE)     \
{                                     \
    Errors CODE = FUNC;               \
    if (CODE != OK_CODE) return CODE; \
}

Errors StackCtor(Stack* stk, size_t element_size, size_t cap) {

    stk->capacity     = cap;
    stk->size         = 0;
    stk->element_size = element_size;
    
#ifdef CANARY_PROTECTION
    stk->start_canary = stk->end_canary = CANPROTECTION;
#endif

    if (stk->capacity) {

        stk->data = (void*)malloc(sizeof(canary_t) * 2 +
                                  element_size * stk->capacity);
        if (!stk->data)
            return MEMORY_ERROR;

        canary_t* tmp = (canary_t*)stk->data;
        *tmp = CANPROTECTION;

        stk->data = (void*) ((char*)stk->data + sizeof(canary_t));

        tmp  = (canary_t*)( (char*)stk->data + stk->capacity * element_size );
        *tmp = CANPROTECTION;


    } else {

        stk->data = POINTER_UNINIT;
    }

    return STACK_OK;
}

Errors StackDtor(Stack* stk) {

    if (stk->data == POINTER_POISON)
        return DTOR_ERROR;

    if (stk->data != POINTER_UNINIT)
        free(stk->data);

    stk->data = POINTER_POISON;

    stk->capacity     = 0;
    stk->size         = 0;
    stk->element_size = 0;

    return STACK_OK;
}

Errors StackPush(Stack* stk, void* elem) {

    if (stk->size >= stk->capacity)
        CODE_CHECK(StackResize(stk, stk->capacity * 2), STACK_OK);

    MemErrors err = mem_copy((void*)((char*)stk->data
             + stk->size * stk->element_size),
             elem, stk->element_size);

    ++stk->size;

    if (err != ALL_OK)
        return MEMORY_ERROR;

    return STACK_OK;
}

Errors StackPop(Stack* stk, void* value) {

    --stk->size;
    MemErrors err = mem_copy(value,
                             (void*)((char*)stk->data
                             + stk->size * stk->element_size),
                             stk->element_size);

    if (err != ALL_OK)
        return MEMORY_ERROR;

    if (stk->size / 4 <= stk->capacity)
        CODE_CHECK(StackResize(stk, stk->capacity / 2), STACK_OK);
    
    return STACK_OK;
}

Errors StackResize(Stack* stk, size_t new_cap) {

    stk->capacity = new_cap;
    stk->data = (void*)realloc(stk->data, sizeof(canary_t) * 2 +
                               stk->element_size * stk->capacity);

    stk->data = (void*) ((char*)stk->data + sizeof(canary_t));

    canary_t* tmp = (canary_t*)( (char*)stk->data +
                                stk->capacity * stk->element_size );
    *tmp = CANPROTECTION;

    return STACK_OK;
}
