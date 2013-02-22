#include "archive.h"

aie_ArcFormat aie_arcfmt(aie_ArcFormatKind kind)
{
  static aie_ArcFormat fmt;

  if(kind == aie_ARC_UNSUPPORTED)
    fmt.name = "unsupported";
  return fmt;
}
