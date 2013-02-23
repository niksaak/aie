#pragma once

#include <./archive.h>

typedef enum aie_ArcFormatKind
{ // archive kinds enumeration
  aie_ARC_UNSUPPORTED,
  aie_ARC_FORMAT_KIND_MAX
} aie_ArcFormatKind;

extern const aie_ArcFormat aie_arcformats[];
