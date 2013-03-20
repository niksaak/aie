#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include <errno.h>

#include <aie_archive.h>
#include <aie_util.h>

#include "aie_KID_Engine_LiNK.h"

typedef struct arc_header_t {
  char magic[4];
  uint32_t fcount;
  uint64_t dummy;
} arc_header_t;

typedef struct arc_entry_t {
  uint32_t offset;
  uint32_t fsize2;
  char fname[24];
} arc_entry_t;

aie_Archive* create(const char* name) { return NULL; } // FIXME
bool extract(aie_Archive* archive) { return false; } // FIXME

aie_Archive* open(const char* name)
{
  FILE* file = fopen(name, "r");
  aie_Archive* arc = aie_mkarchive(&KID_Engine_LiNK, NULL, NULL);
  struct arc_header_t header;
  struct arc_entry_t entry;

  aie_arcfile_push(file, name, 0, &arc->files);

  if(file == NULL) {
    AIE_WARN("Unable to open file '%s'.", name);
    return NULL;
  }

  errno = 0;

  if(!fread(&header, sizeof header, 1, file) || feof(file) || ferror(file)) {
    AIE_ERROR("Unable to read from '%s'", name);
    aie_kmarchive(arc);
    return NULL;
  } else if(strncmp(header.magic, "LNK", 4)) {
    AIE_DEBUG("file %s is not of format \"KID Engine LiNK\"");
    aie_kmarchive(arc);
    return NULL;
  }

  size_t arc_offset = header.fcount * sizeof entry + sizeof header;

  for(size_t i = 0; i < header.fcount; i++) {
    aie_ArcUnitSegment* seg = NULL;

    if(!fread(&entry, sizeof entry, 1, file) || feof(file) || ferror(file)) {
      AIE_ERROR("Unable to read from '%s'", name);
      aie_kmarchive(arc);
      return NULL;
    }

    aie_arcsegment_push(arc->files, entry.offset + arc_offset,
        entry.fsize2 / 2, &seg);
    aie_arctable_put(entry.fname, seg, entry.fsize2 / 2, 0, &arc->table);
  }

  return arc;
}

aie_ArcFormat KID_Engine_LiNK = // format description
{
  .id               = aie_ARC_KID_ENGINE_LINK,
  .name             = "KID Engine LiNK",
  .subformat_num    = 0,
  .subformat_names  = NULL,
  .arc_ext          = "dat",
  .meta_ext         = NULL,
  .features         = aie_FMTOpen | aie_FMTUnsafe,
  .filename_len     = 24,
  .drv_version      = 20130224,

  .open             = &open,
  .create           = &create,
  .extract          = &extract,
};

