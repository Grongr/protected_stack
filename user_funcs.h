#ifndef USER_FUNCS_INCLUDED
#define USER_FUNCS_INCLUDED

#include <stdio.h>

#define TEST
#ifdef TEST
const int ELEMENT_POISON = 666;
#endif

/*!
 * @brief Func to print one element
 *
 * User must define this func for dump to work without
 * problems. Write your definition of this func in 
 * <user_funcs.cpp>
 *
 * @param [in] <elem>   Pointer to the beginning of element
 * @param [in] <output> Where to write the result
 */
void PrintElement(FILE* output, const void* elem);

/*!
 * @brief Checks if element os poisoned
 *
 * @param [in] <elem> void pointer to the element
 *
 * @return true if it is poison and false if not
 */
bool IsPoison(void* elem);

#endif // USER_FUNCS_INCLUDED
