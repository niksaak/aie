#include <aie_error.h>

#include <stdio.h>
#include <errno.h>
#include <string.h>

aie_Error err;

static void tacit_hook_f(aie_Error e)
{ return; }

static void debug_hook_f(aie_Error e)
{
  char* errname = aie_errstr(e.e);
  char* errlvl = "???";

  switch(e.level) {
    case aie_ELMessage: errlvl = "MSG"; break;
    case aie_ELWarning: errlvl = "WRN"; break;
    case aie_ELError: errlvl = "ERR"; break;
    case aie_ELPanic: errlvl = "PANIC"; break;
    default: errlvl = "???"; break;
  }

  fprintf(stderr, "%s", errlvl);
  if(e.function != NULL) fprintf(stderr, " @ %s", e.function);
  fprintf(stderr, ": %s", errname);
  if(e.datum != NULL) fprintf(stderr, ", datum: %s", e.datum);
}

const aie_ErrorHookF aie_tacit_error_hook = &tacit_hook_f;
const aie_ErrorHookF aie_debug_error_hook = &debug_hook_f;

#ifdef DEBUG_MODE
aie_ErrorHookF hook = &debug_hook_f;
#else
aie_ErrorHookF hook = &tacit_hook_f;
#endif

void aie_error(aie_Errno e, aie_ErrorLevel level,
               const char* function, char* datum)
{
  err = (aie_Error){ e, level, function, datum };
  (*hook)(err);
}

void aie_esuccess(const char* func)
{
  err = (aie_Error){ aie_ESUCCESS, 0, func, NULL };
}

int aie_iserror(void)
{
  if(err.level >= aie_ELError)
    return 1;
  return 0;
}

aie_Error aie_geterror(void)
{
  return err;
}

aie_ErrorHookF aie_set_error_hook(aie_ErrorHookF fun)
{
  aie_ErrorHookF oldhook = hook;

  if(fun == NULL) {
    hook = aie_debug_error_hook;
  } else {
    hook = fun;
  }

  return oldhook;
}

char* aie_errstr(aie_Errno e)
{
  switch(e) {
    case aie_ESUCCESS: return "Success";
    case aie_EERRNO: return strerror(errno);
    case aie_ENOWAY: return "Function not implemented";
    case aie_ENURUPO: return "Null pointer";
    case aie_EFORMAT: return "Unrecognized format";
    case aie_EHEADER: return "Bad header";
    case aie_EOFFSET: return "Bad offset";
    case aie_EINTEGRITY: return "Questionamble integrity";
    case aie_EENCRYPTION: return "Encryption";
    case aie_EINDEX: return "Bad index";
    case aie_EASSERT: return "Assertion failed";
    case aie_EUSR: return "User message";
    case aie_EUNKNOWN: return "Unknown error";
    default: return "???";
  }
}
