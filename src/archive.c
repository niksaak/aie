#include <stdlib.h>
#include <string.h>

#include <aie_formats.h>
#include <aie_archive.h>
#include <aie_util.h>

// ArcFormat

const aie_ArcFormat* aie_arcfmt(aie_ArcFormatKind kind)
{
  for(size_t i = 0; i <= (aie_ARC_FORMAT_KIND_MAX); i++) {
    if(aie_arcformats[i]->id == kind)
      return aie_arcformats[i];
  }

  return aie_arcfmt(aie_ARC_UNSUPPORTED);
}

const char* aie_arcfmt_name(const aie_ArcFormat* format)
{
  return format->name;
}

const char* aie_arcfmt_subformats(const aie_ArcFormat* format)
{
  if(format->subformat_names[0] == 0)
    return NULL;
  return format->subformat_names;
}

const char* aie_arcfmt_extensions(const aie_ArcFormat* format)
{
  static char extensions[512] = {0};

  strncat(extensions, format->arc_ext, sizeof extensions - 1);
  sprintf(extensions, " ");
  strncat(extensions, format->meta_ext, 
          sizeof extensions - strlen(extensions));

  return extensions;
}

aie_ArcFormatStatus aie_arcfmt_status(const aie_ArcFormat* format)
{
  return format->status;
}

const char* aie_arcfmt_statusstr(const aie_ArcFormat* format)
{
  aie_ArcFormatStatus status = aie_arcfmt_status(format);
  static char ret[512] = {0};
  char* str = NULL;

  switch(status & 0x7FFFFFFF)
  {
    case aie_NORMAL:
      str = "Normal";
      break;
    case aie_UNSAFE:
      str = "Unsafe";
      break;
    case aie_EXPERIMENTAL:
      str = "Experimental";
      break;
    case aie_HACK:
      str = "Hack";
      break;
    case aie_WRITEONLY:
      str = "Write only";
      break;
    case aie_READONLY:
      str = "Read only";
      break;
  }

  snprintf(ret, sizeof ret, str);
  if(status & aie_PLACEHOLDER)
    snprintf(ret, sizeof ret - strlen(ret) - 1, ", Placeholder");

  return ret;
}

size_t aie_arcfmt_namelen(const aie_ArcFormat* format)
{
  return format->filename_len;
}

uint32_t aie_arcfmt_ver(const aie_ArcFormat* format)
{
  return format->drv_version;
}

// Archive

const aie_ArcFormat* aie_arch_fmt(const aie_Archive* hive)
{
  return hive->fmt;
}

aie_ArcUnitTable* aie_arch_table(aie_Archive* hive)
{
  return hive->table;
}

aie_ArcFile* aie_arch_parts(aie_Archive* hive)
{
  return hive->files;
}

// ArcUnit and friends

aie_ArcUnit* aie_arcunit_get(aie_ArcUnitTable* table, size_t index)
{
  if(index >= table->unitc)
    return NULL;
  return &table->unitv[index];
}

size_t aie_arcunit_push(aie_ArcUnit unit, aie_ArcUnitTable** tableptr)
{ // FIXME by using aie_realloc
  aie_ArcUnitTable* table = *tableptr;
  if((table->unitc + 1) > table->allocated)
    tableptr = realloc (
        table->unitv, sizeof(aie_ArcUnitTable) + aie_nextfib(table->allocated)
    );
  table->unitv[table->unitc] = unit;

  return table->unitc++;
}

const char* aie_arcunit_name(const aie_ArcUnit* unit)
{
  return unit->name;
}

unsigned aie_arcunit_uncompressed_size(const aie_ArcUnit* unit)
{
  return unit->size;
}

static unsigned impl_arcunit_segments_size_sum(aie_ArcUnitSegment* seg)
{
  if(seg) {
    return seg->size + impl_arcunit_segments_size_sum(seg->next);
  }

  return 0;
}

unsigned aie_arcunit_compressed_size(const aie_ArcUnit* unit)
{
  return impl_arcunit_segments_size_sum(unit->segments);
}

unsigned aie_arcunit_segcount(const aie_ArcUnit* unit)
{
  unsigned i = 0;

  for(aie_ArcUnitSegment* seg = unit->segments; seg != NULL; seg = seg->next)
    i++;

  return i;
}

bool aie_arcunit_getflags(const aie_ArcUnit* unit, aie_ArcUnitFlags flags)
{
  return (unit->flags & flags) == flags;
}
