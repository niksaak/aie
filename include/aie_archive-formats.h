/* aie_archive-formats.h - array of formatter declarations and its size */

#pragma once

#include <stdlib.h>

struct aie_ArcFormat;

typedef enum aie_ArcFormatKind
{ // Format kinds enumeration
  aie_ARC_KID_ENGINE_LINK,

  aie_ARC_KIND_COUNT, // count of archive kinds available
  aie_ARC_KIND_UNKNOWN = -1 // format is unknown
} aie_ArcFormatKind;

extern const struct aie_ArcFormat* const aie_arcformats[];

extern const size_t aie_ARCFORMATS_COUNT;
