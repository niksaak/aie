#pragma once

#include <aie_archive.h>

aie_Archive aie_arcopen(aie_ArcFile file, aie_ArcFormatKind kind,
                        const char* opt);
    // try to open archive assuming it is of kind or, if kind < 0,
    // deducing its kind first

/*
aie_Archive aie_arccreate(aie_ArcFile file, char** pathnames,
                          aie_ArcFormatKind kind, const char* opt);
    // create archive file from pathnames
*/

int aie_arcextract(const aie_Archive* archive, const char* target,
                   const char* opt);
    // extract archive to the target directory

int aie_arc_uextract(const aie_ArcUnit* unit, const aie_ArcFormat* fmt,
                     const char* target, const char* opt);
    // extract unit of format fmt to target.
    // Target can be filename, in which case unit will be extracted to file of
    // such name, directory, in which case it will be extracted to file in
    // that directory named unit->name, or NULL, in which case unit will be
    // extracted to stdout

size_t aie_arc_umextract(const aie_ArcUnit* unit, const aie_ArcFormat* fmt,
                         char* buf, size_t offset, size_t size,
                         const char* opt);
    // extract unit of format fmt to memory buffer buf starting at offset,
    // writing no more than size bytes.
    // Returns number of bytes written if end of file encountered,
    // otherwise returns offset + size + 1, i.e. index of next unwritten byte.

