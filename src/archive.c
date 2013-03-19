#include <stdlib.h>
#include <string.h>

#include <aie_formats.h>
#include <aie_archive.h>
#include <aie_util.h>
#include <aie_fibonacci.h>

#define TABLE_UNITS_DEFAULT 256
    // default quantity of preallocated units for aie_aloarctable();

// Archive

aie_Archive* aie_mkarchive(
    aie_ArcFormat* format, aie_ArcUnitTable* table, aie_ArcFile* files)
{
  aie_Archive* arc = aie_malloc(sizeof (aie_Archive));

  arc->fmt = format;
  arc->table = table;
  arc->files = files;

  return arc;
}

int aie_kmarchive(aie_Archive* archive)
{
  aie_kmarctable(archive->table);
  aie_arcfile_destroy(&archive->files);
  free(archive);

  return 0;
}

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

// ArcUnitTable

aie_ArcUnitTable* aie_mkarctable(unsigned units_count)
{
  unsigned quantity = units_count ? units_count : TABLE_UNITS_DEFAULT;
  size_t units_alloc = quantity * sizeof (aie_ArcUnit);
  aie_ArcUnitTable* aut = aie_malloc(sizeof (aie_ArcUnitTable) + units_alloc);

  aut->unitc = 0;
  aut->allocated = sizeof (aie_ArcUnitTable) + units_alloc;

  return aut;
}

int aie_kmarctable(aie_ArcUnitTable* table)
{
  for(size_t i = 0; i < table->unitc; i++)
    aie_arcunit_clean(&table->unitv[i]);
  free(table);

  return 0;
}

aie_ArcUnit* aie_arctable_get(aie_ArcUnitTable* table, size_t index)
{
  return &table->unitv[index];
}

aie_ArcUnit* aie_arctable_unitv(aie_ArcUnitTable* table)
{
  return table->unitv;
}

size_t aie_arctable_put(const char* name, aie_ArcUnitSegment* segments,
    size_t size, aie_ArcUnitFlags flags, aie_ArcUnitTable** table)
{
  if(*table == NULL) {
    *table = aie_mkarctable(0);
  }

  char* namestr = aie_malloc(strlen(name) + 1);
  size_t index = (*table)->unitc;
  size_t new_count = (*table)->unitc + 1;
  aie_ArcUnit* unit = &(*table)->unitv[index];

  if(new_count * sizeof unit > (*table)->allocated) {
    unsigned fib = aie_nextfib((*table)->allocated);

    *table =
      aie_realloc(*table, sizeof (aie_ArcUnitTable) + fib - fib % sizeof *unit);
  }

  unit->name = strncpy(namestr, name, strlen(name) + 1);
  unit->segments = segments;
  unit->size = size;
  unit->flags = flags;

  return ++(*table)->unitc;
}

// ArcUnit

int aie_arcunit_clean(aie_ArcUnit* unit)
{
  free(unit->name);
  unit->name = NULL;
  aie_arcsegment_destroy(&unit->segments);

  return 0;
}

const char* aie_arcunit_name(const aie_ArcUnit* unit)
{
  return unit->name;
}

unsigned aie_arcunit_uncompressed_size(const aie_ArcUnit* unit)
{
  return unit->size;
}

unsigned aie_arcunit_compressed_size(const aie_ArcUnit* unit)
{
  // FIXME or even REMOVEME
  return -1;
}

aie_ArcUnitSegment* aie_arcunit_segments(aie_ArcUnit* unit)
{
  return unit->segments;
}

bool aie_arcunit_getflags(const aie_ArcUnit* unit, aie_ArcUnitFlags flags)
{
  return (unit->flags & flags) == flags;
}

// ArcUnitSegment

aie_ArcUnitSegment* aie_arcsegment_push(aie_ArcFile* file,
    size_t offset, size_t size, aie_ArcUnitSegment** list)
{
  aie_ArcUnitSegment* aus = aie_malloc(sizeof (aie_ArcUnitSegment));

  aus->file = file;
  aus->offset = offset;
  aus->size = size;
  aus->next = *list;
  *list = aus;

  return aus;
}

int aie_arcsegment_destroy(aie_ArcUnitSegment** list)
{
  static aie_ArcUnitSegment* cdr = NULL;
      // static because ai am not sure about this befriending TCO otherwise

  if(*list == NULL)
    return 0;

  cdr = (*list)->next;
  free(*list);
  *list = NULL;

  return aie_arcsegment_destroy(&cdr);
}

// ArcFile

aie_ArcFile* aie_arcfile_push(FILE* file, const char* name, int role,
                              aie_ArcFile** list)
{
  aie_ArcFile* arf = aie_malloc(sizeof (aie_ArcFile));
  char* namestr = aie_malloc(strlen(name) + 1);

  arf->file = file;
  arf->name = strncpy(namestr, name, strlen(name) + 1);
  arf->role = role;
  arf->next = *list;
  *list = arf;

  return arf;
}

int aie_arcfile_destroy(aie_ArcFile** list)
{
  static aie_ArcFile* cdr = NULL;

  if(*list == NULL)
    return 0;

  cdr = (*list)->next;
  free(*list);
  *list = NULL;

  return aie_arcfile_destroy(&cdr);
}

