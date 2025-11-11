#ifndef _M3U8_MEMORY_H_
#define _M3U8_MEMORY_H_

#include "logger.h"
#include "m3u8.h"

#include <stdlib.h>

#define M3U8_MALLOC(ptr, size)                                              \
  do {                                                                      \
    if ((ptr = calloc(1, size)) == NULL) {                                  \
      M3U8_RAISE(M3U8_STATUS_MEM_ALLOC_ERROR, "Failed to allocate memory"); \
    }                                                                       \
  } while (0)

#define M3U8_CHECK_DEALLOC(ptr)                                                        \
  do {                                                                                 \
    if ((ptr) == NULL) {                                                               \
      M3U8_RAISE(M3U8_STATUS_MEM_ALLOC_ERROR, "Attempt to deallocate a NULL pointer"); \
    } else {                                                                           \
      free(ptr);                                                                       \
      ptr = NULL;                                                                      \
    }                                                                                  \
  } while (0)

#endif  // _M3U8_MEMORY_H_
