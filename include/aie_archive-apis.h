#pragma once

#include <aie_archive.h>

aie_Archive*
aie_arcopen(FILE* file, const char* name, aie_ArcFormatKind kind,
            const char* opt);
    // try to open archive assuming it is of kind or, if kind < 0, deducing
    // its kind with aie_arcdeduce()

aie_Archive*
aie_arcmake(const char* target, char** pathnames, aie_ArcFormatKind kind,
            const char* opt);
    // make archive at the target directory from pathnames

int aie_arcextract(const aie_Archive* archive, const char* target, const char* opt);
    // extract archive to the target directory

