#include <aie_archive-sort.h>

#include <string.h>

#define ARCUNIT(val) ((const aie_ArcUnit*) val)

static int arcunit_name_cmp(const void* a, const void* b)
{
  const char* aname = ARCUNIT(a)->name;
  const char* bname = ARCUNIT(b)->name;

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
  const size_t ac = aie_arcsegment_count(ARCUNIT(a)->segments);
  const size_t bc = aie_arcsegment_count(ARCUNIT(b)->segments);

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
  const size_t asize = aie_arcsegment_sumsize(ARCUNIT(a)->segments);
  const size_t bsize = aie_arcsegment_sumsize(ARCUNIT(b)->segments);

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
  const size_t asize = ARCUNIT(a)->size;
  const size_t bsize = ARCUNIT(b)->size;

  if(asize < bsize) {
    return -1;
  }
  if(asize > bsize) {
    return 1;
  }
  return 0;
}

void aie_arctable_sort(const aie_ArcUnitTable* table, aie_ArcUnitSortBy by,
  const aie_ArcUnit** buf, size_t size)
{
  AIE_ASSERT(table != NULL,);
  AIE_ASSERT(buf != NULL,);
  AIE_ASSERT(by < aie_SORTBY_UNIT_SIZE,);

  int i;

  if(size == 0)
    size = table->unitc;

  for(i = 0; i < table->unitc && i <= size; i++)
    buf[i] = &table->unitv[i];

  if(i < size)
    buf[i + 1] = NULL;

  switch(by) { // FIXME: Implement me!
    case aie_SORTBY_UNIT_NAME:
      qsort(buf, i, sizeof *buf, &arcunit_name_cmp);
      return;
    case aie_SORTBY_UNIT_SEGC:
      qsort(buf, i, sizeof *buf, &arcunit_segc_cmp);
      return;
    case aie_SORTBY_UNIT_COMPRESSED_SIZE:
      qsort(buf, i, sizeof *buf, &arcunit_csiz_cmp);
    case aie_SORTBY_UNIT_SIZE:
      qsort(buf, i, sizeof *buf, &arcunit_size_cmp);
      return;
    default: break;
  }
}

