#ifndef USER_FUNCS_INCLUDED
#define USER_FUNCS_INCLUDED

#include <stdio.h>

#define TEST
#ifdef TEST
const int INT_ELEMENT_POISON = 666;
const double DBL_ELEMENT_POISON = 666.6;
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
void PrintElement_int(FILE* output, const void* elem);
void PrintElement_double(FILE* output, const void* elem);

/*!
 * @brief Checks if element os poisoned
 *
 * @param [in] <elem> void pointer to the element
 *
 * @return true if it is poison and false if not
 */
bool IsPoison_int(const void* elem);
bool IsPoison_double(const void* elem);

/*!
* @brief Fills all unused data memory
*
* Uses user poison which whould be defined in
* <user_funcs> files
*
* @param [in] <data>  beginning of stack data
* @param [in] <begin> beginning of poisoned data (size)
* @param [in] <end>   end of poisoned data (capacity)
*/
void FillElmPoison_int(const void* data, size_t begin, size_t end);
void FillElmPoison_double(const void* data, size_t begin, size_t end);

#endif // USER_FUNCS_INCLUDED
