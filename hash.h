#ifndef HASH_INCLUDED
#define HASH_INCLUDED

#include "stack.h"

/*!
 * @brief Ti calculate hash sum of stack.
 *
 * It takes GnuHash func and makes hash from
 * all of stack params
 *
 * @param [in] <stk> pointer to the stack instance
 *
 * @return result hash sum
 */
size_t StkHash(const Stack* stk);

/*!
 * @brief It calculates hash sum of stack data.
 *
 * It takes GnuHash func and makes hash from all
 * the elements of stack data.
 *
 * @param [in] <stk> pointer to the stack instance
 *
 * @return result hash sum
 */
size_t DataHash(const Stack* stk);

/*!
 * @brief To hash all you want
 *
 * Uses GnuHash func to hash your data with size
 * <size> and start in <beg>
 *
 * @param [in] <beg>  Start of data to hash
 * @param [in] <size> Size of data to hash
 *
 * @return hash
 */
size_t HashFunc(const void* beg, size_t size);

#endif // HASH_INCLUDED
