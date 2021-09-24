#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

enum Errors {
    STACK_OK = 0,
    NOT_A_STACK = 1,
    MEMORY_ERROR = 2,
    OVERFLOW = 3
};

typedef enum Errors Errors;


#endif // STACK_H_INCLUDED
