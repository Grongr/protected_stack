#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "mem_copy.h"
#include "stack.h"

#include "user_funcs.h"

#define POINTER_UNINIT (void*)0x66600666
#define POINTER_POISON (void*)0x8BADF00D

const char* log_file = "./log.txt";

#define CODE_CHECK(FUNC, OK_CODE)     \
{                                     \
    Errors CODE = FUNC;               \
    if (CODE != OK_CODE) return CODE; \
}

#define STACK_CHECK(stk)                                                        \
{                                                                               \
    Errors err = StackCheck(stk);                                               \
    if (err)                                                                    \
    {                                                                           \
        FILE* log = fopen(log_file, "a");                                       \
        assert("Cannot open logfile" && log);                                   \
        fprintf(log, "ERROR %d: file %s line %d ", err, __FILE__, __LINE__);    \
        fprintf(log, "function \'%s\'\n", __FUNCTION__);                        \
        fclose(log);                                                            \
        printf("ERROR: file %s line %d ", __FILE__, __LINE__);                  \
        printf("function \'%s\'\n", __FUNCTION__);                              \
        StackDump(stk, err);                                                    \
    }                                                                           \
}

/*****************************************************************/
/*!
 * @brief Fills all unused data memory
 *
 * Uses user pison which whould be defined in
 * <user_funcs> files
 */
static void FillElmPoison(Stack* stk) {

    for (size_t i = stk->size; i < stk->capacity; ++i) {

        mem_copy((void*)((char*)stk->data + i * stk->element_size),
                 (void*)&ELEMENT_POISON, stk->element_size);
    }
}

/*****************************************************************/
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

#ifdef ABSOLUTE_LOGGING
    FillElmPoison(stk);
#endif

    return STACK_OK;
}


/*****************************************************************/
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


/*****************************************************************/
Errors StackPush(Stack* stk, void* elem) {

    STACK_CHECK(stk)

    if (stk->size >= stk->capacity)
        CODE_CHECK(StackResize(stk, stk->capacity * 2), STACK_OK);

    MemErrors err = mem_copy((void*)((char*)stk->data +
                             stk->size * stk->element_size),
                             elem, stk->element_size);

    if (err != ALL_OK)
        return MEMORY_ERROR;

    ++stk->size;

    STACK_CHECK(stk)
    return STACK_OK;
}


/*****************************************************************/
Errors StackPop(Stack* stk, void* value) {

    STACK_CHECK(stk)

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

    err = mem_copy((void*)((char*)stk->data +
                           stk->size * stk->element_size),
                           (void*)&ELEMENT_POISON,
                           stk->element_size);
     
    if (err != ALL_OK)
        return MEMORY_ERROR;
    
    STACK_CHECK(stk)
    return STACK_OK;
}


/*****************************************************************/
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
    stk->data = (void*)realloc((char*)stk->data,
                               stk->element_size * stk->capacity);
#endif // CANARY_PROTECTION

#ifdef ABSOLUTE_LOGGING
    FillElmPoison(stk);
#endif

    return STACK_OK;
}


/*****************************************************************/
Errors StackCheck(Stack* stk) {

    if (stk == NULL)
        return NOT_A_STACK;

    if (stk->data == NULL || stk->data == POINTER_POISON)
        return DTOR_ERROR;

    if (stk->size > stk->capacity)
        return OVERFLOW;

    if (stk->capacity < 4)
        return UNDERFLOW;

#ifdef CANARY_PROTECTION
    canary_t* left  = &((canary_t*)stk->data)[-1];
    canary_t* right = (canary_t*)((char*)stk->data +
                                   stk->capacity * stk->element_size);

    if (stk->start_canary != CANPROTECTION || stk->end_canary != CANPROTECTION)
        return STK_CANARY;

    if (*left != CANPROTECTION || *right != CANPROTECTION)
        return DATA_CANARY;
#endif

    return STACK_OK;
}

/*****************************************************************/
void StackDump(Stack* stk, Errors err) {

    FILE* log = fopen(log_file, "a");

    fprintf(log, "###########################################################\n");

    fprintf(log, "Stack with elements <%d> size: [%p]. Error code: %d\n{\n",
           (int)stk->element_size, stk, err);

    fprintf(log, "Size : %d\n", (int)stk->size);
    fprintf(log, "Capacity : %d\n", (int)stk->capacity);
    fprintf(log, "Data : [%p]\n", stk->data);

    for (size_t i = 0; i < stk->size; ++i) {

        fprintf(log, "    * [%d] = ", (int)i);
        PrintElement(log, (char*)stk->data + stk->element_size * i);
        fprintf(log, "\n");
    }

    for (size_t i = stk->size; i < stk->capacity; ++i) {

        fprintf(log, "      [%d] = ", (int)i);
        PrintElement(log, (char*)stk->data + stk->element_size * i);
        if (!IsPoison((void*)((char*)stk->data + stk->element_size * i)))
            fprintf(log, "\n");
        else
            fprintf(log, " (poison)\n");
    }

    fputs("}\n", log);

    fflush(log);
    fclose(log);
}


#undef CODE_CHECK
