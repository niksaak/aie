#pragma once

#include <inttypes.h>
#include <stdio.h>
#include <stdbool.h>

#include <aie_archive-format.h>
#include <aie_util.h>

// Types:

typedef enum aie_ArcUnitFlags
{
  aie_COMPRESSED = 1,
  aie_ENCRYPTED = 2
} aie_ArcUnitFlags;

typedef struct aie_ArcUnitSegment
{ // Represents segment of archived file, can form linked list
  struct aie_ArcFile* file;     // file where segment starts
  size_t offset;                // offset of segment, filewise
  size_t size;                  // segment size
  struct aie_ArcUnitSegment* next;
} aie_ArcUnitSegment;

typedef struct aie_ArcUnit
{ // Archive allocaton unit, represents one file in archive
  char* name;                   // unit name, freeable
  struct aie_ArcUnitSegment* segments; // file segmentation
  unsigned size;                // uncompressed size
  aie_ArcUnitFlags flags;       // unit flags, ORed
} aie_ArcUnit;

typedef struct aie_ArcUnitTable
{ // Archive allocation table
  unsigned unitc;               // units count;
  size_t allocated;
  aie_ArcUnit unitv[];          // units array;
} aie_ArcUnitTable;

typedef struct aie_ArcFile
{ // Represents part of archive, can form linked list
  FILE* file;                   // file descriptor
  char* name;                   // full filename, freeable
  int role;                     // file role
  struct aie_ArcFile* next;
} aie_ArcFile;

typedef struct aie_Archive
{ // Abstraction for archives
  const struct aie_ArcFormat* fmt;  // pointer to archive format info
  struct aie_ArcUnitTable* table;   // unified allocation table for archive
  struct aie_ArcFile* files;        // files of archive
} aie_Archive;

// Archive

aie_Archive* aie_mkarchive(aie_ArcFormat* format,
                           aie_ArcUnitTable* table,
                           aie_ArcFile* files);
    // make archive

int aie_kmarchive(aie_Archive* archive);
    // deallocate archive and its contents

const aie_ArcFormat* aie_arch_fmt(const aie_Archive* hive);
    // get archive format

aie_ArcUnitTable* aie_arch_table(aie_Archive* hive);
    // get archive allocation table

aie_ArcFile* aie_arch_parts(aie_Archive* hive);
    // get list of archive parts

// ArcUnitTable

aie_ArcUnitTable* aie_mkarctable(unsigned units_count);
    // make ArcTable with place for 'units_count' units preallocated,
    // or default amount if 'units_count' == 0

int aie_kmarctable(aie_ArcUnitTable* table);
    // kill table and its units

aie_ArcUnit* aie_arctable_get(aie_ArcUnitTable* table, size_t index);
    // get ArcUnit placed at 'index' in table.
    // returns NULL if index is too big

aie_ArcUnit* aie_arctable_unitv(aie_ArcUnitTable* table);
    // get units array from 'table'
    // same as aie_arctable_get(table, 0) for now

size_t aie_arctable_put(const char* name,
                        aie_ArcUnitSegment* segments,
                        size_t size, aie_ArcUnitFlags flags,
                        aie_ArcUnitTable** table);
    // put 'unit' into table, return 'unit's index in table
    // create new table if *table is NULL

// ArcUnit

int aie_arcunit_clean(aie_ArcUnit* unit);
    // clean unit, deleting its segments list

const char* aie_arcunit_name(const aie_ArcUnit* unit);
    // return namestring for 'unit'

unsigned aie_arcunit_uncompressed_size(const aie_ArcUnit* unit);
    // return size of uncompressed 'unit', if available

unsigned aie_arcunit_compressed_size(const aie_ArcUnit* unit);
    // return size of 'unit' in archive

aie_ArcUnitSegment* aie_arcunit_segments(aie_ArcUnit* unit);
    // get segments list from 'unit'

int aie_arcunit_flags(const aie_ArcUnit* unit);
    // return flags for unit. Same as unit->flags

// ArcUnitSegment

aie_ArcUnitSegment* aie_arcsegment_push(aie_ArcFile* file,
                                        size_t offset,
                                        size_t size,
                                        aie_ArcUnitSegment** list);
    // push new ArcUnitSegment to list

int aie_arcsegment_destroy(aie_ArcUnitSegment** list);
    // destroy segments list

size_t aie_arcsegment_sizesum(const aie_ArcUnitSegment* list);
    // return sum of sizes of all the segments in list

// ArcFile

aie_ArcFile* aie_arcfile_push(FILE* file,
                              const char* name,
                              int role,
                              aie_ArcFile** list);
    // push new ArcFile to list

int aie_arcfile_destroy(aie_ArcFile** list);
    // destroy archive parts list

