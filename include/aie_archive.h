#pragma once

#include <aie_types.h>

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

