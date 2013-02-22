#pragma once

#include <stdio.h>
#include <stdbool.h>

// Functions:

typedef enum aie_ArcFormatKind { // archive kinds enumeration
  aie_ARC_UNSUPPORTED
} aie_ArcFormatKind;

typedef enum aie_ArcFormatStatus {
  NORMAL, // everything OK with this one
  UNSAFE, // rtfm recomended
  EXPERIMENTAL, // result is not guaranteed
  HACK, // dirty, prolly not for general use
  WRITEONLY, // packing only
  READONLY, // unpacking only
  PLACEHOLDER = 0xF0000000; // placeholder, but may be of use
} aie_ArcFormatStatus;

typedef struct aie_ArcFile (*aie_ArcOpenFun)(const char* name);
typedef struct aie_ArcFile (*aie_ArcCreateFun)(const char* name);
typedef bool (*aie_ArcExtractFun)();

typedef struct aie_ArcFormat { // Struct with archive format description
  enum aie_ArcFormatKind id; // identifier.
  const char* name; // archiver name
  char*const* arc_ext; // pointer to array with file extensions for archive
  char*const* meta_ext; // ~~~ with file extensions for metadata
  enum aie_ArcFormatStatus status;
  unsigned filename_len; // max filename len
  unsigned drv_version; // version in format 0xYYYYmmdd

  aie_ArcOpenFun open;
  aie_ArcCreateFun create;
  aie_ArcExtractFun extract;

} aie_ArcFormat;

typedef struct aie_ArcAllocTableSegment {
  // File segmentation info.
  size_t offset;
  size_t size;
  struct aie_ArcAllocTableSegment* next;
} aie_ArcAllocTableSegment;

typedef struct aie_ArcAllocTable {
  // Archive allocaton table
  char* name; // filename
  struct aie_ArcAllocTableSegment* segments; // file segmentation
  unsigned size; // uncompressed size
  bool compressed;
  bool encrypted;
  void* data; // optional data

  struct aie_ArcAllocTable* next;
} aie_ArcAllocTable;

typedef struct aie_ArcFile {
  // Archive file
  const struct aie_ArcFormat* fmt; // pointer to archive format info
  unsigned subtype; // file subtype
  struct aie_ArcAllocTable table; // unified allocation table for archive
  FILE* file; // file descriptor
  char* name; // file name (prolly optional)

  struct aie_ArcFile* next; // pointer to next part of archive
} aie_ArcFile;

// Functions:

extern aie_ArcFormat aie_arcfmt(aie_ArcFormatKind kind);
// get format for archives of kind
