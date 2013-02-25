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
  aie_PLACEHOLDER = 0x80000000 // placeholder, but may be of use
} aie_ArcFormatStatus;

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
  bool compressed;
  bool encrypted;
  struct aie_ArcUnit* next;
} aie_ArcUnit;

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
  struct aie_ArcUnit* table; // unified allocation table for archive
  struct aie_ArcFile* files; // files of archive
} aie_Archive;

// Getters:

extern const aie_ArcFormat* aie_arcfmt(aie_ArcFormatKind kind);
// get format for archives of kind

extern const char* aie_arcfmt_name(const aie_ArcFormat* format);
// get format name

extern const char* aie_arcfmt_subformats(const aie_ArcFormat* format);
// get string listing subformats of format, or NULL if there is no subformats.

extern const char* aie_arcfmt_extensions(const aie_ArcFormat* format);
// get string listing acceptable fileextensions for format, space separated,
// or NULL if archives of this format can not be recognized by fileextension.
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

