#pragma once

#include <inttypes.h>
#include <stdio.h>
#include <stdbool.h>

#include <aie_archive-kinds.h>

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

typedef struct aie_ArcAllocSegment
{ // Archived file segmentation info.
  struct aie_ArcFile* file; // file where segment starts
  size_t offset; // offset of segment, filewise
  size_t size; // segment size
  struct aie_ArcAllocSegment* next;
} aie_ArcAllocSegment;

typedef struct aie_ArcAlloc
{ // Archive allocaton unit
  char* name; // unit name
  struct aie_ArcAllocSegment* segments; // file segmentation
  unsigned size; // uncompressed size
  bool compressed;
  bool encrypted;
  struct aie_ArcAlloc* next;
} aie_ArcAlloc;

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
  struct aie_ArcAlloc* table; // unified allocation table for archive
  struct aie_ArcFile* files; // files of archive
} aie_Archive;

