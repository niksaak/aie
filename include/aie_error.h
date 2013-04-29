#pragma once

#include <stdarg.h>

#define AIE_MESSAGE(string) \
  aie_error(aie_EUSR, aie_ELMessage, __func__, string)

#define AIE_WARNING(errno, datum) \
  aie_error(errno, aie_ELWarning, __func__, datum)

#define AIE_ERROR(errno, datum) \
  aie_error(errno, aie_ELError, __func__, datum)

#define AIE_PANIC(errno, datum) \
  aie_error(errno, aie_ELPanic, __func__, datum)

#define AIE_WASSERT(assertion)                                    \
  if(!(assertion))                                                \
    aie_error(aie_EASSERT, aie_ELWarning, __func__, #assertion)

#define AIE_ASSERT(assertion, ret)                                \
  do {                                                            \
    if(!(assertion)) {                                            \
      aie_error(aie_EASSERT, aie_ELError, __func__, #assertion);  \
      return ret;                                                 \
    }                                                             \
  } while(0)

#define AIE_PASSERT(assertion, ret)                               \
  do {                                                            \
    if(!(assertion)) {                                            \
      aie_error(aie_EASSERT, aie_ELPanic, __func__, #assertion);  \
      return ret;                                                 \
    }                                                             \
  } while(0)

#define AIE_ERETURN(val) \
  if(aie_iserror())      \
    return val

#define AIE_ERESET() \
  aie_esuccess(__func__)

#define AIE_WITH_ERROR_HOOK(var) \
  var = aie_set_error_hook(var)

#define AIE_WITH_ERROR_HOOK_END(var) \
  aie_set_error_hook(var)

typedef enum aie_Errno {
  aie_ESUCCESS, // No error
  aie_EERRNO, // Error is determined by errno
  aie_ENOWAY, // Function is not implemented
  aie_ENURUPO, // Null pointer error
  aie_EFORMAT, // Unrecognized format
  aie_EHEADER, // Bad header
  aie_EOFFSET, // Bad offsets
  aie_EINTEGRITY, // Questionable integrity
  aie_EENCRYPTION, // Encryption error
  aie_EINDEX, // Bad index
  aie_EASSERT, // Assertion failed
  aie_EUSR, // User error i.e. custom message
  aie_EUNKNOWN = -1 // Unknown error
} aie_Errno;

typedef enum aie_ErrorLevel {
  aie_ELMessage, // message, mostly for use with EUSR
  aie_ELWarning, // warning, warn but proceed as usual
  aie_ELError, // error, serious business, breaks loops, pillages villages
  aie_ELPanic // panic, cease all work and exit, failure
} aie_ErrorLevel;

typedef struct aie_Error {
  aie_Errno e;
  aie_ErrorLevel level;
  const char* function;
  char* datum;
} aie_Error;

typedef void (*aie_ErrorHookF)(aie_Error error);

void aie_error(aie_Errno e, aie_ErrorLevel level,
               const char* function, char* datum);
    // set error possibly supplying additional information

void aie_esuccess(const char* func);
    // reset error

int aie_iserror(void);
    // returns nonzero if error occured

aie_Error aie_geterror(void);
    // get last error

aie_ErrorHookF aie_set_error_hook(aie_ErrorHookF fun);
    // set error hook to be called by aie_error()

char* aie_errstr(aie_Errno e);
    // get string representation for err

extern const aie_ErrorHookF aie_tacit_error_hook;
extern const aie_ErrorHookF aie_default_error_hook;
