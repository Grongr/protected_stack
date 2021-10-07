#include "user_funcs.h"

void PrintElement(FILE* output, const void* elem) {

    int* element = (int*)elem;
    
    fprintf(output, "%d", *element);
}

bool IsPoison(void* elem) {

    int* element = (int*)elem;
    return (*element == ELEMENT_POISON);
}
