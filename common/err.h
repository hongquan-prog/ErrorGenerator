#ifndef ERR_H
#define ERR_H

#include "err_def.h"

#define ENABLE_GLOBAL_DEBUG 0
#define ERR_STR_BUF_SIZE 100
extern char g_error_str_buf[ERR_STR_BUF_SIZE];

#define LOG(errno, format, ...)                                         \
  {                                                                     \
    const char *str = error_to_str(errno);                              \
    snprintf(g_error_str_buf, ERR_STR_BUF_SIZE, format, ##__VA_ARGS__); \
    printf("[%s:%d] %s: %s", __FILE__, __LINE__, str, g_error_str_buf); \
  }

#if ((1 == ENABLE_GLOBAL_DEBUG) || (defined(DEBUG) && (1 == DEBUG)))
#define DEBUG_ASSERT(p)                                                  \
  {                                                                     \
    if (!(p))                                                           \
    {                                                                   \
      LOG(ERR_CONSTRUCT(NullPointer), "please check the pointer!\r\n"); \
      while (1)                                                         \
        ;                                                               \
    }                                                                   \
  }
#define DEBUG_PRINT(code, format, ...) LOG(ERR_CONSTRUCT(code), format, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(code, format, ...)
#define DEBUG_ASSERT(p) \
  {                     \
    while (!(p))        \
      ;                 \
  }
#endif

typedef enum
{
  EXCEPTION_MODULE
} module_enum_t;

// error code begin

typedef enum
{
  Arithmetic = ERR_CODE_BEGIN(EXCEPTION_MODULE),
  NullPointer,
  IndexOutofBounds,
  NoEnoughMemory,
  InvalidParameter,
  InvalidOperation
} exception_def;

// error code end

const char *error_to_str(err_t errno);

#endif
