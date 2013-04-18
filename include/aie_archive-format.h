#pragma once

#include <stdlib.h>
#include <stdio.h>
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

typedef struct aie_Archive*
(*aie_ArcOpenF)(FILE* file, const char* name, const char* opt);
    // pointer to function that opens archive

typedef struct aie_Archive* 
(*aie_ArcCreateF)(char** files, const char* target, const char* opt);
    // pointer to function that creates archive

typedef int
(*aie_ArcExtractF)(const struct aie_Archive* archive, const char* target,
                   const char* opt);
    // pointer to function that extracts archive

typedef int
(*aie_ArcUnitExtractF)(const aie_ArcUnit* unit, const char* target,
                       const char* opt);
    // pointer to function that extracts unit.
    // target can be filename, in which case unit must be extracted to
    // file with that name, or directory, in which case unit must be extracted
    // to file in that directory named unit->name

typedef int
(*aie_ArcUnitMemExtractF)(const aie_ArcUnit* unit,
                          char* buf, size_t offset, size_t size,
                          const char* opt);
    // pointer to function that extracts unit to buffer in memory.
    // offset is an offset from start of unit and size is maximum of bytes
    // which can be written to buf

typedef struct aie_ArcFormat
{ // Archive format description
  aie_ArcFormatKind id;         // identifier.
  const char* name;             // formatter name
  enum aie_ArcFormatFeatures features;

  unsigned subformat_num;       // number of subformats
  const char* subformat_names;  // subformat names, colon separated
  const char* ext;              // file extensions for archive, space separated
  size_t filename_len;          // max filename len
  uint32_t drv_version;         // version in format 0xYYYYmmdd

  aie_ArcOpenF open;
  aie_ArcCreateF create;
  aie_ArcExtractF extract;
  aie_ArcUnitExtractF uextract;
  aie_ArcUnitMemExtractF umextract;

} aie_ArcFormat;

extern const aie_ArcFormat* const aie_arcformats[];
    // array of pointers to formatter descriptions

aie_ArcFormat aie_arcfmt(aie_ArcFormatKind kind);
    // get format for archives of kind

const char* aie_arcfmt_name(const aie_ArcFormat* format);
    // get format name

const char* aie_arcfmt_subformats(const aie_ArcFormat* format);
    // get string listing subformats of format, colon separated, or NULL
    // if there is no subformats.

const char* aie_arcfmt_extensions(const aie_ArcFormat* format);
    // get string listing acceptable fileextensions for 'format'
    // space separated, or NULL if archives of this format
    // can not be recognized by fileextension.

aie_ArcFormatFeatures aie_arcfmt_features(const aie_ArcFormat* format);
    // get formatter features

const char* aie_arcfmt_featurestr(const aie_ArcFormat* format);
    // get string representation for status of a formatter
    // WARNING: returns static string which is modified on each call

size_t aie_arcfmt_namelen(const aie_ArcFormat* format);
    // get maximum filename length in bytes for format

uint32_t aie_arcfmt_ver(const aie_ArcFormat* format);
    // get formatter version

