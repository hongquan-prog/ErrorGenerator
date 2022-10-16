#ifndef ERR_H
#define ERR_H

#include "err_def.h"

//error-code-begin

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

//error-code-end

#endif