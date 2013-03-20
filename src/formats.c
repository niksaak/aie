#include <aie_formats.h>

#include "formats/aie_KID_Engine_LiNK.h"

aie_ArcFormat unsupp =
{ .id = aie_ARC_UNSUPPORTED,
  .name = "Unsupported format",
  .subformat_num = 0,
  .subformat_names = NULL,
  .arc_ext = NULL,
  .meta_ext = NULL,
  .features = 0,
  .filename_len = 0,
  .drv_version = 0,
  .open = NULL,
  .create = NULL,
  .extract = NULL
};

const aie_ArcFormat * const aie_arcformats[] =
{
  &KID_Engine_LiNK,
  &unsupp
};
