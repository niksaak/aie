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
  uint64_t fcount;
  uint64_t dummy;
} arc_header_t;

typedef struct arc_entry_t {
  uint64_t offset;
  uint64_t fsize2;
  char fname[24];
} arc_entry_t;

aie_Archive* open(const char* name);
aie_Archive* create(const char* name);
bool extract(aie_Archive* archive);

/* TODO: REDO
aie_Archive* open(const char* name)
{
  FILE* file = fopen(name, "r");
  aie_Archive* ar = aie_mkarchive();
  struct arc_header_t header = {{0}};

  errno = 0;

  if(file == NULL) {
    AIE_ERROR("Unable to open file at %s: %s", name, strerror(errno));
    return NULL;
  }

  fread(&header, sizeof header, 1, file);

  if(strncmp(header.magic, "LNK", 4)) {
    AIE_DEBUG("file %s is not of format \"KID Engine LiNK\"");
    fclose(file);
    return NULL;
  }

  {
    aie_ArcFile* arcfile = aie_mkarcfile();
    aie_ArcUnitTable* table = aie_mkarctable();
    arc_entry_t entry = {0};
    unsigned arc_offset = header.fcount * sizeof entry + sizeof header;

    arcfile->name = name;
    arcfile->file = file;

    for(size_t i = 0; i < header.fcount; i++) {
      aie_ArcUnitSegment* seg = aie_mkarcunitseg();
      aie_ArcUnit unit = {entry.fname, seg, entry.fsize2, 0};

      fread(&entry, sizeof entry, 1, file);
      seg->file = arcfile;
      seg->offset = entry.offset + arc_offset;
      seg->size = entry.fsize2 / 2;
      seg->next = NULL;
      aie_arcunit_push(unit, &table);
    }

    ar->fmt = &KID_Engine_LiNK;
    ar->table = table;
    ar->files = arcfile;
  }

  return ar;
}

aie_Archive* create(const char* name) { return NULL; }

bool extract(aie_Archive* archive) { return false; }
*/

aie_ArcFormat KID_Engine_LiNK = // format description
{
  .id               = aie_ARC_KID_ENGINE_LINK,
  .name             = "KID Engine LiNK",
  .subformat_num    = 0,
  .subformat_names  = NULL,
  .arc_ext          = "dat",
  .meta_ext         = NULL,
  .status           = aie_PLACEHOLDER,
  .filename_len     = 24,
  .drv_version      = 0x20130224,

  .open             = &open,
  .create           = &create,
  .extract          = &extract,
};
// C standard requires static structs to be initially filled with zeroes,
// so we actually can to omit the fields with NULLs/0s, but better
// make sure, in case of dumb compiler

