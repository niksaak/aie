#pragma once

#include <inttypes.h>
#include <stdio.h>
#include <stdbool.h>

// Functions:

typedef enum aie_ArcFormatKind
{ // archive kinds enumeration
  aie_ARC_UNSUPPORTED
} aie_ArcFormatKind;

typedef enum aie_ArcFormatStatus
{
  aie_NORMAL, // everything OK with this one
  aie_UNSAFE, // rtfm recomended
  aie_EXPERIMENTAL, // result is not guaranteed
  aie_HACK, // dirty, prolly not for general use
  aie_WRITEONLY, // packing only
  aie_READONLY, // unpacking only
  aie_PLACEHOLDER = 0x80000000 // placeholder, but may be of use
} aie_ArcFormatStatus;

typedef struct aie_ArcFile (*aie_ArcOpenFun)(const char* name);
typedef struct aie_ArcFile (*aie_ArcCreateFun)(const char* name);
typedef bool (*aie_ArcExtractFun)();

typedef struct aie_ArcFormat
{ // Struct with archive format description
  enum aie_ArcFormatKind id; // identifier.
  const char* name; // archiver name
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
  size_t offset;
  size_t size;
  struct aie_ArcAllocSegment* next;
} aie_ArcAllocSegment;

typedef struct aie_ArcAlloc
{ // Archive allocaton unit
  char* name; // filename
  struct aie_ArcAllocSegment* segments; // file segmentation
  unsigned size; // uncompressed size
  bool compressed;
  bool encrypted;

  struct aie_ArcAlloc* next;
} aie_ArcAlloc;

typedef struct aie_ArcFile
{ // Archive file
  FILE* file; // file descriptor
  char* name; // filename including path
  unsigned subtype; // file subtype
  struct aie_ArcFile* next;
} aie_ArcFile;

typedef struct aie_Archive
{ // Archive abstraction
  const struct aie_ArcFormat* fmt; // pointer to archive format info
  struct aie_ArcAlloc* table; // unified allocation table for archive
  struct aie_ArcFile* files; // files of archive
} aie_Archive;

// Functions:

extern const aie_ArcFormat* aie_arcfmt(aie_ArcFormatKind kind);
// get format for archives of kind

extern const char* aie_arcfmt_name(const aie_ArcFormat* format);
// get format name

extern const char* aie_arcfmt_extensions(const aie_ArcFormat* format);
// get string, listing acceptable fileextensions for format, space separated.
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
