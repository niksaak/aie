#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

#include <aie_archive-kinds.h>

typedef enum aie_ArcFormatFeatures
{ // Formatter features
  aie_FMTOpen = 0x0001,
  aie_FMTExtract = 0x0002,
  aie_FMTCreate = 0x0004,
  aie_FMTOK = aie_FMTOpen | aie_FMTExtract | aie_FMTCreate,
  aie_FMTUnsafe = 0x0008,
  aie_FMTHack = 0x0010,
  aie_FMTPlaceholder = 0x0020
} aie_ArcFormatFeatures;

typedef struct aie_Archive* (*aie_ArcOpenFun)(const char* name);
    // pointer to function that opens archive

typedef struct aie_Archive* (*aie_ArcCreateFun)(const char* name);
    // pointer to function that creates archive

typedef bool (*aie_ArcExtractFun)(struct aie_Archive* archive);
    // pointer to function that extracts archive

typedef struct aie_ArcFormat
{ // Archive format description
  aie_ArcFormatKind id;         // identifier.
  const char* name;             // formatter name
  unsigned subformat_num;       // number of subformats
  const char* subformat_names;  // subformat names, colon separated
  const char* arc_ext;          // file extensions for archive, space separated
  const char* meta_ext;         // file extensions for metadata
  enum aie_ArcFormatFeatures features;
  size_t filename_len;          // max filename len
  uint32_t drv_version;         // version in format 0xYYYYmmdd

  aie_ArcOpenFun open;
  aie_ArcCreateFun create;
  aie_ArcExtractFun extract;

} aie_ArcFormat;

const aie_ArcFormat* aie_arcfmt(aie_ArcFormatKind kind);
    // get format for archives of kind

const char* aie_arcfmt_name(const aie_ArcFormat* format);
    // get format name

const char* aie_arcfmt_subformats(const aie_ArcFormat* format);
    // get string listing subformats of format, or NULL
    // if there is no subformats.

const char* aie_arcfmt_extensions(const aie_ArcFormat* format);
    // get string listing acceptable fileextensions for 'format'
    // space separated, or NULL if archives of this format
    // can not be recognized by fileextension.
    // WARNING: returns static string which is modified on each call

aie_ArcFormatFeatures aie_arcfmt_features(const aie_ArcFormat* format);
    // get formatter features

const char* aie_arcfmt_featurestr(const aie_ArcFormat* format);
    // get string representation for status of a formatter
    // WARNING: returns static string which is modified on each call

size_t aie_arcfmt_namelen(const aie_ArcFormat* format);
    // get maximum filename length in bytes for format

uint32_t aie_arcfmt_ver(const aie_ArcFormat* format);
    // get formatter version

