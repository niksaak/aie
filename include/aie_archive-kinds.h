#pragma once

typedef enum aie_ArcFormatKind
{ // Format kinds enumeration
  aie_ARC_KID_ENGINE_LINK,

  aie_ARC_KIND_COUNT, // count of archive kinds available
  aie_ARC_KIND_AUTO = -1 // signals that format must be deduced from file
} aie_ArcFormatKind;

