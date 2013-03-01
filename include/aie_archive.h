#pragma once

#include <inttypes.h>
#include <stdio.h>
#include <stdbool.h>

#include <aie_archive-kinds.h>

// Types:

typedef enum aie_ArcFormatStatus
{ // Formatter statuses
  aie_NORMAL, // everything OK with formatters of this kind
  aie_UNSAFE, // rtfm recomended
  aie_EXPERIMENTAL, // result is not guaranteed
  aie_HACK, // dirty, prolly not for general use
  aie_WRITEONLY, // packing only
  aie_READONLY, // unpacking only
  aie_PLACEHOLDER = -1 // placeholder, but may be of use
} aie_ArcFormatStatus;

typedef enum aie_ArcUnitFlags
{
  aie_COMPRESSED = 1,
  aie_ENCRYPTED = 2
} aie_ArcUnitFlags;

typedef struct aie_Archive* (*aie_ArcOpenFun)(const char* name);
    // pointer to function that opens archive

typedef struct aie_Archive* (*aie_ArcCreateFun)(const char* name);
    // pointer to function that creates archive

typedef bool (*aie_ArcExtractFun)(struct aie_Archive* archive);
    // pointer to function that extracts archive

typedef struct aie_ArcFormat
{ // Archive format description
  aie_ArcFormatKind id; // identifier.
  const char* name; // formatter name
  unsigned subformat_num; // number of subformats
  const char* subformat_names; // subformat names, colon separated
  const char* arc_ext; // file extensions for archive, space separated
  const char* meta_ext; // file extensions for metadata, space separated
  enum aie_ArcFormatStatus status;
  size_t filename_len; // max filename len
  uint32_t drv_version; // version in format 0xYYYYmmdd

  aie_ArcOpenFun open;
  aie_ArcCreateFun create;
  aie_ArcExtractFun extract;

} aie_ArcFormat;

typedef struct aie_ArcUnitSegment
{ // Archived file segmentation info.
  struct aie_ArcFile* file; // file where segment starts
  size_t offset; // offset of segment, filewise
  size_t size; // segment size
  struct aie_ArcUnitSegment* next;
} aie_ArcUnitSegment;

typedef struct aie_ArcUnit
{ // Archive allocaton unit, represents one file in archive
  char* name; // unit name
  struct aie_ArcUnitSegment* segments; // file segmentation
  unsigned size; // uncompressed size
  aie_ArcUnitFlags flags; // unit flags, ORed
} aie_ArcUnit;

typedef struct aie_ArcUnitTable
{ // Archive allocation table
  unsigned unitc; // units count;
  size_t allocated;
  aie_ArcUnit unitv[]; // units array;
} aie_ArcUnitTable;

typedef struct aie_ArcFile
{ // Archive file
  FILE* file; // file descriptor
  const char* name; // filename including path
  unsigned subtype; // file subtype
  struct aie_ArcFile* next;
} aie_ArcFile;

typedef struct aie_Archive
{ // Archive abstraction
  const struct aie_ArcFormat* fmt; // pointer to archive format info
  struct aie_ArcUnitTable* table; // unified allocation table for archive
  struct aie_ArcFile* files; // files of archive
} aie_Archive;

// Getters:

/// ArcFormat

extern const aie_ArcFormat* aie_arcfmt(aie_ArcFormatKind kind);
    // get format for archives of kind

extern const char* aie_arcfmt_name(const aie_ArcFormat* format);
    // get format name

extern const char* aie_arcfmt_subformats(const aie_ArcFormat* format);
    // get string listing subformats of format, or NULL 
    // if there is no subformats.

extern const char* aie_arcfmt_extensions(const aie_ArcFormat* format);
    // get string listing acceptable fileextensions for 'format' 
    // space separated, or NULL if archives of this format 
    // can not be recognized by fileextension.
    // WARNING: returns static string which is modified on each call

extern aie_ArcFormatStatus aie_arcfmt_status(const aie_ArcFormat* format);
    // get status of a formatter

extern const char* aie_arcfmt_statusstr(const aie_ArcFormat* format);
    // get string representation for status of a formatter
    // WARNING: returns static string which is modified on each call

extern size_t aie_arcfmt_namelen(const aie_ArcFormat* format);
    // get maximum filename length in bytes for format

extern uint32_t aie_arcfmt_ver(const aie_ArcFormat* format);
    // get formatter version

/// Archive

extern const aie_ArcFormat* aie_arch_fmt(const aie_Archive* hive);
    // get archive format

extern aie_ArcUnitTable* aie_arch_table(aie_Archive* hive);
    // get archive allocation table

extern aie_ArcFile* aie_arch_parts(aie_Archive* hive);
    // get list of archive parts

/// ArcUnit and friends

extern aie_ArcUnit*
aie_arcunit_get(aie_ArcUnitTable* table, size_t index);
    // get pointer to Unit indexed at 'index' at 'hive's unittable.
    // returns NULL if there is no unit at this index

extern size_t aie_arcunit_push(aie_ArcUnit unit, aie_ArcUnitTable** tableptr);
    // push 'unit' to 'hive's unittable, return index of pushed 'unit'
    // possible TODO: move it to internal header not visible from outside

extern const char* aie_arcunit_name(const aie_ArcUnit* unit);
    // return namestring of 'unit'

extern unsigned aie_arcunit_uncompressed_size(const aie_ArcUnit* unit);
    // return size of uncompressed 'unit', if available

extern unsigned aie_arcunit_compressed_size(const aie_ArcUnit* unit);
    // return size of 'unit' in archive

extern unsigned aie_arcunit_segcount(const aie_ArcUnit* unit);
    // return number of segments of 'unit'

extern bool
aie_arcunit_getflags(const aie_ArcUnit* unit, aie_ArcUnitFlags flags);
    // return true if flags is set


