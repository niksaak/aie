#include "kid-engine-link.h"

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#include <aie_archive.h>
#include <aie_archive-formats.h>

static aie_Archive open(aie_ArcFile file, const char* opt);

aie_ArcFormat kid_engine_link =
{
  .id = aie_ARC_KID_ENGINE_LINK,
  .name = "KID-Engine-LiNK",
  .features = aie_FMTUnsafe,
  .subformat_names = NULL,
  .ext = "dat",
  .filename_len = 24,
  .version = "2013-04-30",

  .open = &open,
  .extract = NULL,
  .uextract = NULL,
  .umextract = NULL
};

typedef struct header_t {
  char magic[4];
  uint32_t fcount;
  uint64_t dummy;
} header_t;

typedef struct entry_r {
  uint32_t offset;
  uint32_t size2;
  char name[24];
} entry_t;

static aie_Archive open(aie_ArcFile file, const char* opt)
{ // TODO: check archive integrity
  AIE_ASSERT(file.stream != NULL, aie_ARCNIL);

  header_t header = {{0}};
  aie_ArcFileCons* files = NULL;
  aie_ArcUnitTable* table = NULL;
  size_t arc_offset;

  if(!fread(&header, sizeof header, 1, file.stream)) {
    AIE_ERROR(aie_EERRNO, file.name);
    return aie_ARCNIL;
  }
  if(strcmp(header.magic, "LNK")) {
    header.magic[3] = 0;
    AIE_ERROR(aie_EHEADER, header.magic);
    return aie_ARCNIL;
  }

  table = aie_mkarctable(0);
  arc_offset = header.fcount * sizeof (entry_t) + sizeof header;

  aie_arcfile_push(file, &files);

  for(int i = 0; i < header.fcount; i++) {
    entry_t entry;
    aie_ArcUnit unit;
    aie_ArcSegment segment;

    if(!fread(&entry, sizeof entry, 1, file.stream)) {
      AIE_ERROR(aie_EERRNO, file.name);
      return aie_ARCNIL;
    }

    unit = (aie_ArcUnit){ entry.name, NULL,
                          entry.size2 / 2, entry.size2 / 2, 0 };
    segment = (aie_ArcSegment)
              { &files->car, entry.offset + arc_offset, entry.size2 / 2 };
    aie_arcsegment_push(segment, &unit.segments);
    aie_arctable_put(unit, &table);
  }

  return (aie_Archive){ &kid_engine_link, table, files };
}

