#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>

#include <aie_types.h>
#include <aie_archive.h>

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

aie_Archive* open(const char* name)
{
  FILE* file = fopen(name, "r");
  aie_ArcFile* arcfile = malloc(sizeof(aie_ArcFile));
  aie_ArcAlloc* nodes = malloc(sizeof(aie_ArcAlloc));
  aie_Archive* arc = malloc(sizeof(aie_Archive));
  arc_header_t head;
  arc_entry_t entry;
  unsigned arc_offset;

  if(file == NULL || arcfile == NULL || nodes == NULL || arc == NULL) {
    fclose(file);
    return NULL;
  }

  fread(&head, sizeof(head), 1, file);

  if(strncmp(head.magic, "LNK", 4)) {
    fclose(file);
    return NULL;
  }

  arc_offset = head.fcount * sizeof entry + sizeof head;

  arcfile->file = file;
  arcfile->name = name;
  arcfile->subtype = 0;
  arcfile->next = NULL;

  for(int i; i < head.fcount; i++)
  {
    aie_ArcAlloc* node = malloc(sizeof(aie_ArcAlloc));
    aie_ArcAllocSegment* segment = malloc(sizeof(aie_ArcAllocSegment));

    if(node == NULL || segment == NULL) {
      fclose(file);
      break; // TODO: set aie_errno here
    }

    fread(&entry, sizeof entry, 1, file);
    node->name = malloc(strlen(entry.fname) + 1);

    if(node->name == NULL) {
      fclose(file);
      break;
    }

    segment->file = arcfile;
    segment->offset = entry.offset + arc_offset;
    segment->size = entry.fsize2 / 2;
    segment->next = NULL;
    memset(node->name, 0, sizeof entry.fname);
    strncpy(node->name, entry.fname, sizeof entry.fname);
    node->segments = segment;
    node->size = segment->size;
    node->compressed = false;
    node->encrypted = false;
    node->next = nodes;
    nodes = node;
  }

  arc = malloc(sizeof(aie_Archive));
  arc->fmt = &KID_Engine_LiNK;
  arc->table = nodes;
  arc->files = arcfile;

  return arc;
}

aie_Archive* create(const char* name) { return NULL; }

bool extract(aie_Archive* archive) { return false; }

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
// C standard requires static structs to be initially filled with zeros,
// so we actually can to omit the fields with NULLs/0s, but better
// make sure, in case of dumb compiler

