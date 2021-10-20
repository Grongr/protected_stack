#include <cmath>
#include "mem_copy.h"
#include "user_funcs.h"

#define epsilon 1e-7
/*!
 * @brief Simple comparator func
 *
 * @param [in] <a> first to compare
 * @param [in] <b> second to compare
 *
 * @return 1 if bigger, 0 if eq and -1 if less
 */
static int dcmp(double a, double b) {

    if (fabs(a - b) < epsilon)
        return 0;
    else if (a > b)
        return 1;
    else
        return -1;
}

void PrintElement_int(FILE* output, const void* elem) {

    int* element = (int*)elem;
    
    fprintf(output, "%d", *element);
}

void PrintElement_double(FILE* output, const void* elem) {

    double* element = (double*)elem;

    fprintf(output, "%lg", *element);
}

bool IsPoison_int(const void* elem) {

    int* element = (int*)elem;
    return (*element == INT_ELEMENT_POISON);
}

bool IsPoison_double(const void* elem) {

    double* element = (double*)elem;
    return (!dcmp(*element, DBL_ELEMENT_POISON));
}

void FillElmPoison_int(const void* data, size_t begin, size_t end) {

    for (size_t i = begin; i < end; ++i) {

        mem_copy((void*)((char*)data + i * sizeof(int)),
                 (void*)&INT_ELEMENT_POISON, sizeof(int));
    }
}

void FillElmPoison_double(const void* data, size_t begin, size_t end) {
    
    for (size_t i = begin; i < end; ++i) {

        mem_copy((void*)((char*)data + i * sizeof(double)),
                 (void*)&DBL_ELEMENT_POISON, sizeof(double));
    }
}
