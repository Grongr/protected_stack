#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include <stddef.h>
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
enum Errors {
    STACK_OK     = 0, //< All was ok
    NOT_A_STACK  = 1, //< If struct wasn't stack
    MEMORY_ERROR = 2, //< Not enough memory or another memerror
    OVERFLOW     = 3, //< Size > capacity error
    POP_ERROR    = 4, //< Something went wrong in Pop func
    PUSH_ERROR   = 5, //< Something went wrong in push func
    RESIZE_ERROR = 6, //< Something went wrong in resize func
    DTOR_ERROR   = 7  //< If dtor was called twice or another dtor error
};

typedef enum Errors Errors;

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
 * @param <end_canary>   Canary in the end of stack mem
 */
struct Stack {

#ifdef CANARY_PROTECTION
    canary_t start_canary;
#endif // CANARY_PROTECTION

    void*    data;
    size_t   capacity;
    size_t   size;
    size_t   element_size;
    
#ifdef CANARY_PROTECTION
    canary_t end_canary;
#endif // CANARY_PROTECTION

};

/*!
 * @brief Constructor of stack
 *
 * Constructs the stack with capacity == cap
 *
 * @param [out] <stk>           pointer to the stack we construct
 * @param [in]  <element_size>  sizeof one element
 * @param [in]  <cap>           new capacity pf the stack
 *
 * return Error code from <enum Errors>
 */
Errors StackCtor(Stack* stk, size_t element_size, size_t cap = 0);

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
 */
Errors StackPush(Stack* stk, void* elem);

/*!
 * @brief Cuts and returns the last element of stack
 *
 * @param [in]  <stk>    pointer to the stack instance
 * @param [out] <value>  where ans will be written
 *
 * @return The last Stack element
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
 * @brief Prints all the information in log file
 *
 * Prints where stack is now and availible
 * status of it
 *
 * @param [in] <stk>  Pointer to the stack instance
 *
 * @return error code
 */
Errors StackDump(Stack* stk, int line, char* func);

#undef DEBUG
#endif // STACK_H_INCLUDED
