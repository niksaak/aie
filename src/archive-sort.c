#include <aie_archive-sort.h>

#include <string.h>

static int arcunit_name_cmp(const void* a, const void* b)
{
  const char* aname = ((aie_ArcUnit*)a)->name;
  const char* bname = ((aie_ArcUnit*)b)->name;

  if(aname == NULL && bname == NULL) {
    return 0;
  }
  if(aname == NULL) {
    return 1;
  }
  if(bname == NULL) {
    return -1;
  }

  return strcmp(aname, bname);
}

static int arcunit_segc_cmp(const void* a, const void* b)
{
  const size_t ac = aie_arcsegment_count(((aie_ArcUnit*)a)->segments);
  const size_t bc = aie_arcsegment_count(((aie_ArcUnit*)b)->segments);

  if(ac < bc) {
    return -1;
  }
  if(ac > bc) {
    return 1;
  }
  return 0;
}

static int arcunit_csiz_cmp(const void* a, const void* b)
{
  const size_t asize = ((aie_ArcUnit*)a)->csize;
  const size_t bsize = ((aie_ArcUnit*)b)->csize;

  if(asize < bsize) {
    return -1;
  }
  if(asize > bsize) {
    return 1;
  }
  return 0;
}

static int arcunit_size_cmp(const void* a, const void* b)
{
  const size_t asize = ((aie_ArcUnit*)a)->size;
  const size_t bsize = ((aie_ArcUnit*)b)->size;

  if(asize < bsize) {
    return -1;
  }
  if(asize > bsize) {
    return 1;
  }
  return 0;
}

size_t aie_arctable_sort(const aie_ArcUnitTable* table, aie_ArcUnitSortBy by,
    const aie_ArcUnit** buf, size_t size)
{
  if(table == NULL) {
    AIE_ERROR(aie_ENURUPO, "table");
    return 0;
  }
  if(buf == NULL) {
    AIE_ERROR(aie_ENURUPO, "buf");
    return 0;
  }
  if(by > aie_SORTBY_UNIT_SIZE) {
    AIE_ERROR(aie_EENUM, "by");
    return 0;
  }

  size_t i;

  if(size == 0)
    size = table->unitc;

  for(i = 0; i < table->unitc && i <= size; i++)
    buf[i] = &table->unitv[i];

  switch(by) {
    case aie_SORTBY_UNIT_NAME:
      qsort(buf, i, sizeof *buf, &arcunit_name_cmp);
      return i;
    case aie_SORTBY_UNIT_SEGC:
      qsort(buf, i, sizeof *buf, &arcunit_segc_cmp);
      return i;
    case aie_SORTBY_UNIT_COMPRESSED_SIZE:
      qsort(buf, i, sizeof *buf, &arcunit_csiz_cmp);
      return i;
    case aie_SORTBY_UNIT_SIZE:
      qsort(buf, i, sizeof *buf, &arcunit_size_cmp);
      return i;
    default: return 0; // will never happen
  }
}

