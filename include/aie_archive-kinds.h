#pragma once

typedef enum aie_ArcFormatKind
{ // Format kinds enumeration
  aie_ARC_KID_ENGINE_LINK,

  aie_ARC_KIND_COUNT, // count of archive kinds available
  aie_ARC_KIND_UNKNOWN = -1 // format is unknown
} aie_ArcFormatKind;

