#pragma once

#include <inttypes.h>
#include <stdio.h>
#include <stdbool.h>

#include <aie_archive-kinds.h>
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
  unsigned offset;              // offset of segment, filewise
  unsigned size;                // segment size
  struct aie_ArcUnitSegment* next;
} aie_ArcUnitSegment;

typedef struct aie_ArcUnit
{ // Archive allocaton unit, represents one file in archive
  char* name;                   // unit name
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
  char* name;                   // filename including path
  int role;                     // file role
  struct aie_ArcFile* next;
} aie_ArcFile;

typedef struct aie_Archive
{ // Abstraction for archives
  const struct aie_ArcFormat* fmt;  // pointer to archive format info
  struct aie_ArcUnitTable* table;   // unified allocation table for archive
  struct aie_ArcFile* files;        // files of archive
} aie_Archive;

/* TODO: REDO
// Allocators:

/// Archive

extern aie_Archive* aie_aloarchive(void);
    // allocate Archive and set its fields to default values

extern aie_Archive*
aie_mkarchive(aie_ArcFormatKind kind, 
              aie_ArcUnitTable* table,
              aie_ArcFile* files);
    // allocate Archive and set its fields to corresponding args

extern void aie_frearchive(aie_Archive* archive);
    // free 'archive'

extern int aie_kmarchive(aie_Archive* archive);
    // kill murderously 'archive' and those of its fields that need killing
    // TODO: think about making this to return void

/// ArcFile

extern aie_ArcFile* aie_aloarcfile(void);
    // allocate ArcFile

extern aie_ArcFile*
aie_mkarcfile(FILE* file,
              char* name,
              int role,
              aie_ArcFile* next);
    // allocate ArcFile and set its fields to corresponding args

extern void aie_frearcfile(aie_ArcFile* archive);
    // free 'archive'

extern int aie_kmarchive(aie_ArcFile* arcfile);
    // kill murderously 'arcfile' and those of its fields that need killing

/// ArcUnitTable

extern aie_ArcUnitTable* aie_aloarctable(void);
    // allocate ArcUnitTable with default amount of place for units

extern aie_ArcUnitTable*
aie_mkarctable(unsigned units_count);
    // allocate ArcUnitTable with space for 'units_count' units

extern void aie_frearctable(aie_ArcTable* table);
    // free 'table'

extern int aie_kmarctable(aie_ArcUnitTable* table);
    // kill murderously 'table' and those of its fields that need killing

/// ArcUnit

extern aie_ArcUnit
aie_arcunit(char* name,
            aie_ArcUnitSegment* segments,
            unsigned size,
            aie_ArcUnitFlags);

/// ArcUnitSegment

extern aie_ArcUnitSegment* aie_aloarcunit_seg(void);
    // allocate ArcUnitSegment and set its fields to default values

extern aie_ArcUnitSegment*
aie_mkarcunit_seg(aie_ArcFile* file,
                     size_t offset,
                     size_t size,
                     aie_ArcUnitSegment* next);
    // allocate ArcUnitSegment and set its fields to corresponding arguments

extern void aie_frearcunit_seg(aie_ArcUnitSegment* segment);
    // free 'segment'

extern int aie_kmarcunit_seg(aie_ArcUnitSegment* segment);
    // kill murderously 'segment' and those of its fields that need killing
*/

// Archive

extern aie_Archive* aie_mkarchive(aie_ArcFormatKind kind,
                                  aie_ArcUnitTable* table,
                                  aie_ArcFile* files);
    // make archive

extern int aie_kmarchive(aie_Archive* archive);
    // deallocate archive and its contents

extern const aie_ArcFormat* aie_arch_fmt(const aie_Archive* hive);
    // get archive format

extern aie_ArcUnitTable* aie_arch_table(aie_Archive* hive);
    // get archive allocation table

extern aie_ArcFile* aie_arch_parts(aie_Archive* hive);
    // get list of archive parts

// ArcUnitTable

extern aie_ArcUnitTable* aie_mkarctable(unsigned units_count);
    // make ArcTable with place for 'units_count' units preallocated,
    // or default amount if 'units_count' == 0

extern int aie_kmarctable(aie_ArcUnitTable* table);
    // kill table and its units

extern aie_ArcUnit* aie_arctable_get(aie_ArcUnitTable* table, size_t index);
    // get ArcUnit placed at 'index' in table.
    // returns NULL if index is too big

extern aie_ArcUnit* aie_arctable_unitv(aie_ArcUnitTable* table);
    // get units array from 'table'
    // same as aie_arctable_get(table, 0) for now

extern unsigned aie_arctable_push(aie_ArcUnitTable* table, aie_ArcUnit unit);
    // push 'unit' to table, return 'unit's index in table

// ArcUnit

extern aie_ArcUnit aie_arcunit(char* name,
                               aie_ArcUnitSegment* segments,
                               unsigned size,
                               aie_ArcUnitFlags flags);
    // create unit

extern const char* aie_arcunit_name(aie_ArcUnit* unit);
    // return namestring of 'unit'

extern unsigned aie_arcunit_uncompressed_size(aie_ArcUnit* unit);
    // return size of uncompressed 'unit', if available

extern unsigned aie_arcunit_compressed_size(aie_ArcUnit* unit);
    // return size of 'unit' in archive

extern aie_ArcUnitSegment* aie_arcunit_segments(aie_ArcUnit* unit);
    // get segments list from 'unit'

extern bool
aie_arcunit_getflags(const aie_ArcUnit* unit, aie_ArcUnitFlags flags);
    // return true if flags is set

// ArcUnitSegment

extern aie_ArcUnitSegment* aie_arcsegment_push(aie_ArcFile* file,
                                               unsigned offset,
                                               unsigned size,
                                               aie_ArcUnitSegment** list);
    // push new ArcUnitSegment to list

extern int aie_arcsegment_destroy(aie_ArcUnitSegment** list);
    // destroy segments list

// ArcFile

extern aie_ArcFile* aie_arcfile_push(FILE* file,
                                     char* name,
                                     unsigned role,
                                     aie_ArcFile** list);
    // push new ArcFile to list

extern int aie_arcfile_destroy(aie_ArcFile** list);
    // destroy archive parts list
