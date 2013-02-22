#pragma once

#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// Functions:

typedef enum aie_ArcFormatKind { // archive kinds enumeration
  aie_ARC_UNSUPPORTED
} aie_ArcFormatKind;

typedef enum aie_ArcFormatStatus {
  NORMAL, // normal format
  UNSAFE, // rtfm recomended
  EXPERIMENTAL, // result is not guaranteed
  HACK, // dirty, prolly not for general use
  WRITEONLY, // packing only
  PLACEHOLDER, // but may be of use~
  READONLY // unpacking only
} aie_ArcFormatStatus;

typedef struct aie_ArcFile (*aie_ArcOpenFun)();
typedef struct aie_ArcFile (*aie_ArcCreateFun)();
typedef bool (*aie_ArcExtractFun)();

typedef struct aie_ArcFormat { // Struct with archive format description
  enum aie_ArcFormatKind id; // identifier.
  const char* name; // archiver name
  const char* extension; // file extension for archive
  enum aie_ArcFormatStatus status;
  unsigned filename_len;
  unsigned drv_version; // version in format 0xYYYYmmdd

  aie_ArcOpenFun open;
  aie_ArcCreateFun create;
  aie_ArcExtractFun extract;

} aie_ArcFormat;

typedef struct aie_ArcAllocTableSegment {
  // File segmentation info. If 'next' == NULL, file is one-piece
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
  const struct aie_ArcFormat* fmt; // pointer to archive format info
  struct aie_ArcAllocTable table; // unified allocation table for archive
  FILE* file; // file descriptor
  char* name; // file name (prolly optional)

  struct aie_ArcFile* next; // pointer to next part of archive
} aie_ArcFile;

// Functions:

extern aie_ArcFormat aie_arcfmt(aie_ArcFormatKind kind);
// get format for archives of kind
