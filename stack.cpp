#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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

    stk->capacity     = (cap <= 4) ? 4 : cap;
    stk->size         = 0;
    stk->element_size = element_size;
    
#ifdef CANARY_PROTECTION
    stk->start_canary = stk->end_canary = CANPROTECTION;
    
    stk->data = (void*)malloc(sizeof(canary_t) * 2 +
                              element_size * stk->capacity);
    if (!stk->data)
        return MEMORY_ERROR;

    canary_t* tmp = (canary_t*)stk->data;
    *tmp = CANPROTECTION;

    stk->data = (void*) ((char*)stk->data + sizeof(canary_t));

    tmp  = (canary_t*)( (char*)stk->data + stk->capacity * element_size );
    *tmp = CANPROTECTION;
#else
    stk->data = (void*)malloc(element_size * cap);

    if (!stk->data)
        return MEMORY_ERROR;
#endif // CANARY_PROTECTION

    return STACK_OK;
}

Errors StackDtor(Stack* stk) {

    if (stk->data == POINTER_POISON)
        return DTOR_ERROR;

#ifndef CANARY_PROTECTION
    free(stk->data);
#else
    free((char*)stk->data - sizeof(canary_t));
#endif // CANARY_PROTECTION

    stk->data = POINTER_POISON;

    stk->capacity     = 0;
    stk->size         = 0;
    stk->element_size = 0;

    return STACK_OK;
}

Errors StackPush(Stack* stk, void* elem) {

    if (stk->size >= stk->capacity)
        CODE_CHECK(StackResize(stk, stk->capacity * 2), STACK_OK);

    MemErrors err = mem_copy((void*)((char*)stk->data +
                             stk->size * stk->element_size),
                             elem, stk->element_size);

    if (err != ALL_OK)
        return MEMORY_ERROR;

    ++stk->size;

    return STACK_OK;
}

Errors StackPop(Stack* stk, void* value) {

    if (stk->size <= 0)
        return UNDERFLOW;

    --stk->size;
    MemErrors err = mem_copy(value,
                             (void*)((char*)stk->data
                             + stk->size * stk->element_size),
                             stk->element_size);

    if (err != ALL_OK)
        return MEMORY_ERROR;

    if (stk->size / 4 <= stk->capacity && stk->size > 8)
        CODE_CHECK(StackResize(stk, stk->capacity / 2), STACK_OK);
    
    return STACK_OK;
}

Errors StackResize(Stack* stk, size_t new_cap) {

    stk->capacity = (new_cap <= 4) ? 4 : new_cap;

#ifdef CANARY_PROTECTION

    stk->data = (void*) ((char*)stk->data - sizeof(canary_t));

    stk->data = (void*)realloc(stk->data, sizeof(canary_t) * 2 +
                               stk->element_size * stk->capacity);

    if (!stk->data)
        return MEMORY_ERROR;

    stk->data = (void*) ((char*)stk->data + sizeof(canary_t));

    canary_t* tmp = (canary_t*)( (char*)stk->data +
                                stk->capacity * stk->element_size );
    *tmp = CANPROTECTION;
#else
    stk->data = (void*)realloc((char*)stk->data, stk->element_size * stk->capacity);
#endif // CANARY_PROTECTION

    return STACK_OK;
}

#undef CODE_CHECK
