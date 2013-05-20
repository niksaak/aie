/* aie_archive.h - general archive tools */

#pragma once

#include <inttypes.h>
#include <stdio.h>
#include <stdbool.h>

#include <aie_archive-formats.h>
#include <aie_error.h>
#include <aie_util.h>

// Types:

typedef enum aie_ArcUnitFlags
{
  aie_COMPRESSED = 1,
  aie_ENCRYPTED = 2
} aie_ArcUnitFlags;

typedef enum aie_ArcFormatFeatures
{ // Formatter features
  aie_FMTUnsafe       = 0x0100,
  aie_FMTHack         = 0x0200,
  aie_FMTPlaceholder  = 0x0400
} aie_ArcFormatFeatures;

typedef struct aie_ArcSegment
{ // Represents segment of archived unit
  struct aie_ArcFile* file;     // file where segment starts
  size_t offset;                // offset of segment, filewise
  size_t size;                  // segment size
} aie_ArcSegment;

typedef struct aie_ArcSegmentCons
{ // Forms list of aie_ArcSegments
  struct aie_ArcSegment car;
  struct aie_ArcSegmentCons* cdr;
} aie_ArcSegmentCons;

typedef struct aie_ArcUnit
{ // Archive allocaton unit, represents file in archive
  char* name;                   // unit name, freeable
  struct aie_ArcSegmentCons* segments; // unit segmentation
  size_t size;                  // uncompressed size
  aie_ArcUnitFlags flags;       // unit flags, ORed
} aie_ArcUnit;

typedef struct aie_ArcUnitTable
{ // Archive allocation table
  size_t unitc;                 // units count;
  size_t allocc;                // bytes allocated;
  aie_ArcUnit unitv[];          // units array;
} aie_ArcUnitTable;

typedef struct aie_ArcFile
{ // Represents file in filesystem
  FILE* stream;                   // file stream
  char* name;                   // full filename, freeable
  int role;                     // file role
} aie_ArcFile;

typedef struct aie_ArcFileCons
{ // Forms list of aie_ArcFiles
  struct aie_ArcFile car;
  struct aie_ArcFileCons* cdr;
} aie_ArcFileCons;

typedef struct aie_Archive
(*aie_ArcOpenF)(struct aie_ArcFile file, const char* opt);
    // pointer to function that opens archive

/*
typedef struct aie_Archive
(*aie_ArcCreateF)(const char* target, char** files, const char* opt);
    // pointer to function that creates archive
*/

typedef int
(*aie_ArcExtractF)(const struct aie_Archive* archive, const char* target,
                   const char* opt);
    // pointer to function that extracts archive

typedef int
(*aie_ArcUnitExtractF)(aie_ArcUnit unit, const char* target, const char* opt);
    // pointer to function that extracts unit.
    // target can be filename, in which case unit must be extracted to
    // file with that name, or directory, in which case unit must be extracted
    // to file in that directory named unit->name

typedef size_t
(*aie_ArcUnitMemExtractF)(aie_ArcUnit unit,
                          char* buf, size_t offset, size_t size,
                          const char* opt);
    // pointer to function that extracts unit to memory buffer.
    // offset is offset from start of unit and size is maximum of bytes
    // which can be written to buf.
    // Return value is index of next unwritten byte or amount of bytes written
    // if EOF is encountered.

typedef struct aie_ArcFormat
{ // Archive format description
  aie_ArcFormatKind id;         // identifier.
  char* name;                   // formatter name
  enum aie_ArcFormatFeatures features;

  unsigned subformat_num;       // number of subformats
  char* subformat_names;        // subformat names, semicolon separated
  char* ext;                    // file extensions for archive, space separated
  size_t filename_len;          // max filename len
  const char* version;          // formatter version

  aie_ArcOpenF open;
  // aie_ArcCreateF create;
  aie_ArcExtractF extract;
  aie_ArcUnitExtractF uextract;
  aie_ArcUnitMemExtractF umextract;

} aie_ArcFormat;

typedef struct aie_Archive
{ // Abstraction for archives
  const struct aie_ArcFormat* fmt;  // pointer to archive format info
  struct aie_ArcUnitTable* table;   // unified allocation table for archive
  struct aie_ArcFileCons* files;        // files of archive
} aie_Archive;


// ArcFormat:

static const aie_ArcFormat aie_ARCFMTNIL = {0};

aie_ArcFormat aie_arcfmt(aie_ArcFormatKind kind);
    // get format of kind

inline char* aie_arcfmt_name(const aie_ArcFormat* format)
{   // get format name
  if(format == NULL) {
    AIE_ERROR(aie_ENURUPO, "format");
    return NULL;
  }
  return format->name;
}

inline char* aie_arcfmt_subformats(const aie_ArcFormat* format)
{   // get string listing subformats of format, colon separated, or NULL
    // if there is no subformats.
  if(format == NULL) {
    AIE_ERROR(aie_ENURUPO, "format");
    return NULL;
  }
  return format->subformat_names;
}

inline char* aie_arcfmt_extensions(const aie_ArcFormat* format)
{   // get string listing acceptable fileextensions for 'format'
    // space separated, or NULL if archives of this format
    // can not be recognized by fileextension.
  if(format == NULL) {
    AIE_ERROR(aie_ENURUPO, "format");
    return NULL;
  }
  return format->ext;
}

inline aie_ArcFormatFeatures aie_arcfmt_features(const aie_ArcFormat* format)
{   // get formatter features
  if(format == NULL) {
    AIE_ERROR(aie_ENURUPO, "format");
    return 0;
  }
  return format->features;
}

inline size_t aie_arcfmt_namelen(const aie_ArcFormat* format)
{   // get maximum filename length in bytes for format
  if(format == NULL) {
    AIE_ERROR(aie_ENURUPO, "format");
    return 0;
  }
  return format->filename_len;
}

inline const char* aie_arcfmt_ver(const aie_ArcFormat* format)
{   // get formatter version
  if(format == NULL) {
    AIE_ERROR(aie_ENURUPO, "format");
    return NULL;
  }
  return format->version;
}

// Archive:

static const aie_Archive aie_ARCNIL = { 0, 0, 0 };

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

inline aie_ArcUnit aie_arctable_get(aie_ArcUnitTable* table, size_t index)
{
  if(table == NULL) {
    AIE_ERROR(aie_ENURUPO, "table");
    return (aie_ArcUnit){0};
  }
  if(index >= table->unitc) {
    AIE_ERROR(aie_EINDEX, aie_mkstring("%z", index));
    return (aie_ArcUnit){0};
  }
  return table->unitv[index];
}

size_t aie_arctable_put(aie_ArcUnit unit, aie_ArcUnitTable** table);
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
  if(unit == NULL) {
    AIE_ERROR(aie_ENURUPO, "unit");
    return 1;
  }
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

inline int aie_kmarcsegment(aie_ArcSegment* segment)
{   // set segment fields to default values
  if(segment == NULL) {
    AIE_ERROR(aie_ENURUPO, "segment");
    return 1;
  }
  *segment = (aie_ArcSegment){ 0, 0, 0 };
  return 0;
}

aie_ArcSegmentCons* aie_arcsegment_push(aie_ArcSegment segment,
                                        aie_ArcSegmentCons** list);
    // push segment to list

int aie_arcsegment_list_destroy(aie_ArcSegmentCons** list);
    // destroy segments list

size_t aie_arcsegment_sumsize(aie_ArcSegmentCons* list);
    // total size of segments in list

inline size_t aie_arcsegment_count(aie_ArcSegmentCons* list)
{  // count segments in list
  if(list == NULL) {
    return 0;
  }
  return 1 + aie_arcsegment_count(list->cdr);
}


// ArcFile:

inline aie_ArcFile aie_mkarcfile(FILE* file, char* name, int role)
{
  return (aie_ArcFile){ file, name, role };
}

inline int aie_kmarcfile(aie_ArcFile* file)
{
  *file = (aie_ArcFile){ 0, 0, 0 };
  return 0;
}

aie_ArcFileCons* aie_arcfile_push(aie_ArcFile file, aie_ArcFileCons** list);
    // push file to list

int aie_arcfile_list_destroy(aie_ArcFileCons** list);
    // destroy file list

