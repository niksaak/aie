/* aie_archive-sort.h - sorting functions */

#pragma once

#include <stdlib.h>

#include <aie_archive.h>

typedef enum aie_ArcUnitSortBy {
  aie_SORTBY_UNIT_NAME,
  aie_SORTBY_UNIT_SEGC,
  aie_SORTBY_UNIT_COMPRESSED_SIZE,
  aie_SORTBY_UNIT_SIZE
} aie_ArcUnitSortBy;

void aie_arctable_sort(const aie_ArcUnitTable* table, aie_ArcUnitSortBy by,
                       const aie_ArcUnit** buf, size_t size);
    // fill buf with pointers to units of table, no more than size and
    // in order specified by;
    // if size is zero, length of buff is assumed to be equal to table->unitc,
    // if size is greater than table->unitc, terminating NULL will be written
    // to buf

