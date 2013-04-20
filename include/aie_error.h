#pragma once

typedef enum aie_Errno {
  aie_OK, // No error
  aie_EERRNO, // Error is determined by errno
  aie_EFORMAT, // Unrecognized format
  aie_EHEADER, // Bad header
  aie_EOFFSET, // Bad offsets
  aie_ESIZE, // Incorrect size
  aie_EUNKNOWN = -1 // Unknown error
} aie_Errno;

typedef struct aie_Error {
  aie_Errno e;
  char* file;
  char* info;
} aie_Error;

typedef void (*aie_ErrorHandlerF)(aie_Error error);

void aie_error(aie_Errno e, char* file, char* info);
    // set error possibly supplying additional information

aie_Error aie_geterror(void);
    // get last error

aie_ErrorHandlerF aie_set_error_handler(aie_ErrorHandlerF fun);
    // set error handler to be called by aie_error()

char* aie_errstr(aie_Errno e);
    // get string representation for err

