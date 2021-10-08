#ifndef MEM_COPY_INCLUDED
#define MEM_COPY_INCLUDED

#include <stddef.h>

/*!
 * @brief Enum with Memory errors
 */
typedef enum MemErrors {
    ALL_OK          = 0, //< All was ok
    MEM_LACK        = 1, //< Lack of memory
    INVALID_POINTER = 2, //< Name says all you need
    ANOTHER         = 3  //< not understandable error
} MemErrors;

/*!
 * @brief This is my realisation of memcpy
 *
 * Copies <size> bytes from <pb> to <pa>
 *
 * @param [out] <pa>    where to copy bytes
 * @param [in]  <pb>    from where to copy bytes
 * @param [in]  <size>  count of bytes to copy
 *
 * @return error code
 */
MemErrors mem_copy(void* pa, void* pb, size_t size);

#endif // MEM_COPY_INCLUDED
