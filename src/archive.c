#include <aie_archive.h>

#include <stdlib.h>
#include <string.h>

#include <aie_archive-formats.h>
#include <aie_error.h>
#include <aie_util.h>
#include <aie_fibonacci.h>

#define ARCTABLE_DEFAULT_SIZE 128


// ArcFormat:

aie_ArcFormat aie_arcfmt(aie_ArcFormatKind kind)
{
  if(kind >= aie_ARC_KIND_COUNT) {
    AIE_ERROR(aie_EINDEX, aie_mkstring("%i", kind));
    return aie_ARCFMTNIL;
  }

  aie_ArcFormat fmt = *aie_arcformats[kind];

  if(fmt.id == kind) return fmt;

  for(int i = 0; i < aie_ARC_KIND_COUNT; i++) {
    fmt = *aie_arcformats[i];
    if(fmt.id == kind) return fmt;
  }

  AIE_ERROR(aie_EFORMAT, NULL);
  return aie_ARCFMTNIL;
}

extern inline char* aie_arcfmt_name(const aie_ArcFormat* format);

extern inline char* aie_arcfmt_subformats(const aie_ArcFormat* format);

extern inline char* aie_arcfmt_extensions(const aie_ArcFormat* format);

extern inline aie_ArcFormatFeatures
aie_arcfmt_features(const aie_ArcFormat* format);

extern inline size_t aie_arcfmt_namelen(const aie_ArcFormat* format);

extern inline const char* aie_arcfmt_ver(const aie_ArcFormat* format);

// Archive:
// aie_ArcFormat* fmt
// aie_ArcUnitTable* table
// aie_ArcFileCons* files

extern inline aie_Archive aie_mkarchive(aie_ArcFormat* format,
    aie_ArcUnitTable* table, aie_ArcFileCons* files);

int aie_kmarchive(aie_Archive* archive)
{
  if(archive == NULL) {
    AIE_ERROR(aie_ENURUPO, "archive");
    return 1;
  }
  if(archive->table != NULL) {
    aie_kmarctable(archive->table);
  }
  if(archive->files != NULL) {
    aie_arcfile_delis(&archive->files);
  }
  *archive = aie_ARCNIL;

  return 0;
}


// ArcUnitTable:
// size_t unitc
// size_t allocc
// aie_ArcUnit unitv[]

aie_ArcUnitTable* aie_mkarctable(size_t size)
{
  if(size == 0) size = ARCTABLE_DEFAULT_SIZE;
  size_t allocc = size * sizeof (aie_ArcUnit) + sizeof (aie_ArcUnitTable);
  aie_ArcUnitTable *table = aie_malloc(allocc);

  table->unitc = 0;
  table->allocc = allocc;

  return table;
}

int aie_kmarctable(aie_ArcUnitTable* table)
{
  if(table == NULL) {
    AIE_ERROR(aie_ENURUPO, "table");
    return 1;
  }
  for(int i = 0; i < table->unitc; i++) {
    free(table->unitv[i].name);
    aie_arcsegment_delis(&table->unitv[i].segments);
  }
  free(table);

  return 0;
}

extern inline aie_ArcUnit aie_arctable_get(aie_ArcUnitTable* table,
    size_t index);

size_t aie_arctable_put(aie_ArcUnit unit, aie_ArcUnitTable** table)
{
  if(*table == NULL) {
    AIE_WARNING(aie_ENURUPO, "table");
    *table = aie_mkarctable(0);
  }

  size_t index = (*table)->unitc++;
  size_t allocc = (*table)->unitc * sizeof unit + sizeof **table;

  unit.name = strdup(unit.name);

  if(allocc > (*table)->allocc) {
    long fib = aie_nextfib((*table)->allocc);
    *table = aie_realloc(*table, sizeof **table + fib - fib % sizeof unit);
  }

  (*table)->unitv[index] = unit;

  return index;
}


// ArcUnit
// char* name
// aie_ArcSegmentCons* segments
// size_t size
// aie_ArcUnitFlags flags

extern inline aie_ArcUnit aie_mkarcunit(char* name,
    aie_ArcSegmentCons* segments, size_t size, aie_ArcUnitFlags flags);

extern inline int aie_kmarcunit(aie_ArcUnit* unit);


// ArcSegment
// aie_ArcFile* file
// size_t offset
// size_t size

extern inline aie_ArcSegment aie_mkarcsegment(aie_ArcFile* file, size_t offset,
    size_t size);

extern inline int aie_kmarcsegment(aie_ArcSegment* segment);

aie_ArcSegmentCons* aie_arcsegment_cons(aie_ArcSegment segment,
    aie_ArcSegmentCons* list)
{
  aie_ArcSegmentCons* cons = malloc(sizeof *cons);
  *cons = (aie_ArcSegmentCons){ segment, list };
  return cons;
}

aie_ArcSegmentCons* aie_arcsegment_last(aie_ArcSegmentCons* list)
{
  if(list == NULL) {
    AIE_WARNING(aie_ENURUPO, "list");
    return NULL;
  }

  for(list = NULL; list->cdr != NULL; list = list->cdr);
  return list;
}

aie_ArcSegmentCons* aie_arcsegment_push(aie_ArcSegment segment,
    aie_ArcSegmentCons* list)
{
  if(list == NULL) {
    AIE_WARNING(aie_ENURUPO, "list");
    return aie_arcsegment_cons(segment, NULL);
  }

  aie_arcsegment_last(list)->cdr = aie_arcsegment_cons(segment, NULL);
  return list;
}

int aie_arcsegment_delis(aie_ArcSegmentCons** list)
{
  if(list == NULL) {
    AIE_ERROR(aie_ENURUPO, "list");
    return -1;
  }

  if(*list == NULL) {
    return 0;
  }

  aie_ArcSegmentCons* cdr = (*list)->cdr;
  *list = aie_free(*list);

  return aie_arcsegment_delis(&cdr);
}

extern inline size_t aie_arcsegment_count(aie_ArcSegmentCons* list);


// ArcFile
// FILE* file
// char* name
// int role

extern inline aie_ArcFile aie_mkarcfile(FILE* file, char* name, int role);

extern inline int aie_kmarcfile(aie_ArcFile* file);

aie_ArcFileCons* aie_arcfile_cons(aie_ArcFile file, aie_ArcFileCons* list)
{
  aie_ArcFileCons* cons = malloc(sizeof *cons);
  *cons = (aie_ArcFileCons){ file, list };
  return cons;
}

aie_ArcFileCons* aie_arcfile_last(aie_ArcFileCons* list)
{
  if(list == NULL) {
    AIE_WARNING(aie_ENURUPO, "list");
    return NULL;
  }
  for(; list->cdr != NULL; list = list->cdr);
  return list;
}

aie_ArcFileCons* aie_arcfile_push(aie_ArcFile file, aie_ArcFileCons* list)
{
  if(list == NULL) {
    AIE_ERROR(aie_ENURUPO, "list");
    return NULL;
  }
  file.name = strdup(file.name);
  aie_arcfile_last(list)->cdr = aie_arcfile_cons(file, NULL);
  return list;
}

int aie_arcfile_delis(aie_ArcFileCons** list)
{
  if(list == NULL) {
    AIE_ERROR(aie_ENURUPO, "list");
    return 1;
  }

  if(*list == NULL) {
    return 0;
  }

  aie_ArcFileCons* cdr = (*list)->cdr;
  free((*list)->car.name);
  *list = aie_free(*list);

  return aie_arcfile_delis(&cdr);
}

