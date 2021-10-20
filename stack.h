#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include <stdio.h>
#include <stddef.h>

#include "user_funcs.h"

/*!
 * @brief Running mods
 *
 * Use definition of <DEBUG>, <CANARY_PROTECTION>
 * and <HASH_SUM_PROTECTION> macroes to turn
 * protection on
 *
 * <DEBUG> turns all protection in one command
 * <CANARY_PROTECTION> uses canary_t typedef to
 * understand what type canary is and CANPROTECTION
 * constant to understand what value should be in
 * <start_canary> and <end_canary> valuables
 *
 * <log_file> is variable where you can specify
 * a peth to log file
 */
#define DEBUG

#ifdef DEBUG
#define CANARY_PROTECTION
#define HASH_SUM_PROTECTION
#define ABSOLUTE_LOGGING
#endif // DEBUG

#ifdef CANARY_PROTECTION

    typedef unsigned long long canary_t;
    const canary_t CANPROTECTION = 0xABADDEAC;

#endif // CANARY_PROTECTION

/*!
 * List of constants. Each one means
 * different type of error.
 */
typedef enum {
    STACK_OK     =  0, //< All was ok
    NOT_A_STACK  =  1, //< If struct wasn't stack
    MEMORY_ERROR =  2, //< Not enough memory or another memerror
    OVERFLOW     =  3, //< Size > capacity or needed size error
    UNDERFLOW    =  4, //< Size <= 0
    POP_ERROR    =  5, //< Something went wrong in Pop func
    PUSH_ERROR   =  6, //< Something went wrong in push func
    RESIZE_ERROR =  7, //< Something went wrong in resize func
    DTOR_ERROR   =  8, //< If dtor was called twice or another dtor error
    CTOR_ERROR   =  9, //< If ctor was called twice or another ctor error
    STK_CANARY   = 10, //< If one of stack canaries have been changed
    DATA_CANARY  = 11, //< If one of data canaries have been changed
    DATA_HASH    = 12, //< If data hash has been changed without reason
    STK_HASH     = 13  //< If stack hash has been changed without reason
} Errors;

/*!
 * @brief Not another simple stack
 *
 * Stack protected with canary protection algorithm
 * and hash sum algorithm
 *
 * Use definition of <DEBUG>, <CANARY_PROTECTION>
 * and <HASH_SUM_PROTECTION> macroes to turn
 * protection on
 *
 * <DEBUG> turns all protection in one command
 * <CANARY_PROTECTION> uses canary_t typedef to
 * understand what type canary is and CANPROTECTION
 * constant to understand what value should be in
 * <start_canary> and <end_canary> valuables
 *
 * @param <start_canary> Canary in the start of stack mem
 * @param <data>         Dinamic array of elements
 * @param <capacity>     memeory / sizeof(element)
 * @param <size>         now elements in array
 * @param <element_size> sizeof(element)
 * @param <type>         tyoe of stack
 * @param <end_canary>   Canary in the end of stack mem
 */
typedef struct {

#ifdef CANARY_PROTECTION
    canary_t start_canary;
#endif // CANARY_PROTECTION

    void*          data;
    size_t         capacity;
    size_t         size;
    size_t         element_size;
    const char*    type;

    void (*PrintElement) (FILE*, const void*);
    bool (*IsPoison) (const void*);
    void (*FillElmPoison) (const void*, size_t, size_t);

#ifdef HASH_SUM_PROTECTION
    size_t   hash;
    size_t   datah;
#endif // HASH_SUM_PROTECTION
    
#ifdef CANARY_PROTECTION
    canary_t end_canary;
#endif // CANARY_PROTECTION

} Stack;

/*!
 * @brief Constructor of stack
 *
 * Constructs the stack with capacity == cap
 * Use it with <StackCtor_> definition if you wanna
 * use Dumping
 *
 * @param [out] <stk>           pointer to the stack we construct
 * @param [in]  <element_size>  sizeof one element
 * @param [in]  <cap>           new capacity pf the stack
 *
 * return Error code from <enum Errors>
 */
Errors StackCtor(Stack* stk, size_t element_size, size_t cap = 0);

/*****************************************************************/
#ifdef ABSOLUTE_LOGGING

#define CAT(X, Y) X ## _ ## Y
#define TEMPLATE(X, Y) CAT(X, Y)
/*!
 * @brief To comfortly using stack use it
 *
 * @param [in] <stk>  this is a stack instance
 * @param [in] <type> type of stack elements
 */
#define StackCtor_(stk, type_)                          \
{                                                       \
    stk.type = #type_;                                  \
    stk.PrintElement = TEMPLATE(PrintElement, type_);   \
    stk.IsPoison = TEMPLATE(IsPoison, type_);           \
    stk.FillElmPoison = TEMPLATE(FillElmPoison, type_); \
    StackCtor(&stk, sizeof(type_));                     \
}

#else // ABSOLUTE_LOGGING

#define StackCtor_ StackCtor

#endif // ABSOLUTE_LOGGING
/*****************************************************************/

/*!
 * @brief Destrucor of stack
 *
 * Should be called only every time StackCtor is called
 * If called more then one time, does nothing and returns Error
 *
 * @param [in] <stk> Stack to destruct
 */
Errors StackDtor(Stack* stk);

/*!
 * @brief Func to add elem to the Satck
 *
 * Adds elem to the end of stack and resizes it
 * if needed
 *
 * @param [in] <stk>  Pointer to the stack instance
 * @param [in] <elem> pointer to add in the end of stack
 *
 * @return error code
 */
Errors StackPush(Stack* stk, void* elem);

/*!
 * @brief Cuts and returns the last element of stack
 *
 * @param [in]  <stk>    pointer to the stack instance
 * @param [out] <value>  where ans will be written
 *
 * @return error code
 */
Errors StackPop(Stack* stk, void* value);

/*!
 * @brief Resizes Stack
 *
 * Changes Stack capacity in depence of <new_cap>
 * is less or bigger then <capacity> 
 *
 * @param [in] <stk>     Pointer to the stack instance
 * @param [in] <new_cap> New capacity
 *
 * @return error code
 */
Errors StackResize(Stack* stk, size_t new_cap);

/*!
 * @brief Func to check if stack is ok
 *
 * Depends on what it has in canary params, 
 * data pointer and size, capacity params
 * returns error code.
 *
 * @param [in] <stk> Pointer to the stack instance
 *
 * @return error code
 */
Errors StackCheck(Stack* stk);

/*!
 * @brief Prints all the information in log file
 *
 * Prints where stack is now and availible
 * status of it
 *
 * @param [in] <stk>  Pointer to the stack instance
 * @paran [in] <err>  Code of error
 *
 */
void StackDump(Stack* stk, Errors err);

#undef DEBUG
#endif // STACK_H_INCLUDED
