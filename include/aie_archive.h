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

typedef struct aie_ArcSegment
{ // Represents segment of archived file, can form linked list
  struct aie_ArcFile* file;     // file where segment starts
  size_t offset;                // offset of segment, filewise
  size_t size;                  // segment size
} aie_ArcUnitSegment;

typedef struct aie_ArcSegmentCons
{
  struct aie_ArcSegment car;
  struct aie_ArcSegment* cdr;
} aie_ArcSegmentCons;

typedef struct aie_ArcUnit
{ // Archive allocaton unit, represents one file in archive
  char* name;                   // unit name, freeable
  struct aie_ArcSegmentCons* segments; // file segmentation
  size_t size;                  // uncompressed size
  aie_ArcUnitFlags flags;       // unit flags, ORed
} aie_ArcUnit;

typedef struct aie_ArcUnitTable
{ // Archive allocation table
  size_t unitc;                 // units count;
  size_t allocated;
  aie_ArcUnit unitv[];          // units array;
} aie_ArcUnitTable;

typedef struct aie_ArcFile
{ // Represents part of archive, can form linked list
  FILE* file;                   // file descriptor
  char* name;                   // full filename, freeable
  int role;                     // file role
} aie_ArcFile;

typedef struct aie_ArcFileCons
{
  struct aie_ArcFile car;
  struct aie_ArcFile* cdr;
} aie_ArcFileCons;

typedef struct aie_Archive
{ // Abstraction for archives
  const struct aie_ArcFormat* fmt;  // pointer to archive format info
  struct aie_ArcUnitTable* table;   // unified allocation table for archive
  struct aie_ArcFile* files;        // files of archive
} aie_Archive;


// Archive:

inline aie_Archive aie_mkarchive(aie_ArcFormat* format,
                                 aie_ArcUnitTable* table,
                                 aie_ArcFileCons* files)
{   // construct archive information structure
  return (aie_Archive){ format, table, files };
}

int aie_kmarchive(aie_Archive* archive);
    // deinitialize archive and its contents

// ArcUnitTable

aie_ArcUnitTable* aie_mkarctable(size_t size);
    // allocate table with space for 'size' units preallocated,
    // or default amount if size == 0

int aie_kmarctable(aie_ArcUnitTable* table);
    // deallocate table and its units

inline aie_ArcUnit* aie_arctable_get(aie_ArcUnitTable* table, size_t index)
{
  return index < table->unitc ? table->unitv[index] : NULL;
}

size_t aie_arctable_put(aie_ArcUnit unit, aie_ArcUnitTable* table);
    // put unit into table;
    // unit.name will be duplicated to heap


// ArcUnit:

inline aie_ArcUnit aie_mkarcunit(char* name,
                                 aie_ArcSegmentCons* segments,
                                 size_t size,
                                 aie_ArcUnitFlags flags)
{ // construct arcunit
  return (aie_ArcUnit){ name, segments, size, flags };
}

inline int aie_kmarcunit(aie_ArcUnit* unit)
{ // deinit arcunit
  *unit = (aie_ArcUnit){ NULL, NULL, 0, 0 };
  return 0;
}


// ArcSegment:

inline aie_ArcSegment aie_mkarcsegment(aie_ArcFile* file,
                                       size_t offset,
                                       size_t size)
{
  return (aie_ArcSegment){ file, offset, size };
}

int aie_kmarcsegment(aie_ArcSegment* segment);
    // set segment fields to default values

aie_ArcSegmentCons* aie_arcsegment_push(aie_ArcSegment* segment,
                                        aie_ArcSegmentCons** list);
    // push segment to list

int aie_arcsegment_list_destroy(aie_ArcSegmentCons** list);
    // destroy segments list

size_t aie_arcsegment_sumsize(aie_ArcSegmentCons* list);
    // total size of segments in list


// ArcFile:

inline aie_ArcFile aie_mkarcfile(FILE* file,
                                 char* name,
                                 int role)
{
  return (aie_ArcFile){ file, name, role };
}

int aie_kmarcfile(aie_ArcSegment* segment);

aie_ArcFileCons* aie_arcfile_push(aie_ArcFile* segment,
                                  aie_ArcFileCons** list);
    // push file to list

int aie_arcfile_list_destroy(aie_ArcFileCons** list);
    // destroy file list

