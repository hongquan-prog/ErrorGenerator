#ifndef ERR_H
#define ERR_H

#include "err_def.h"

#define LOG(errno, info)                                              \
  {                                                                   \
    const char *str = error_to_str(errno);                            \
    printf("[%s:%d] %s: %s", __FILE__, __LINE__, str, info); \
  } 

typedef enum
{
  EXCEPTION_MODULE,
  CHARGE_MODULE,
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

typedef enum
{
  NoError = ERR_CODE_BEGIN(CHARGE_MODULE),
  AdminLogTimeout,
  NoUidToAllocate,
  UidReadFailed,
  UidWriteFailed,
  NoMacToAllocate,
  MacReadFailed,
  MacWriteFailed,
  ParkLockExamineFailed,
  CardReaderExamineFailed,
} charge_status_def;

// error code end

const char *error_to_str(err_t errno);

#endif