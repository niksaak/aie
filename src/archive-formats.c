#include <aie_archive-formats.h>

#include "formats/kid-engine-link.h"

const aie_ArcFormat* const aie_arcformats[] =
{
  &kid_engine_link
};

const size_t aie_ARCFORMATS_COUNT = sizeof aie_arcformats / sizeof (void *);
