#pragma once

#include <aie_archive.h>

const aie_ArcFormat* aie_arcdeduce(FILE* file, const char* name);
    // get format for file

aie_Archive* aie_arcopen(FILE* file, const char* name, aie_ArcFormatKind kind);
    // try to open archive assuming it is of kind or, if kind < 0, deducing
    // its kind with aie_arcdeduce()

aie_Archive*
aie_arcmake(const char* target, const char** pathnames, const char* opt);
    // make archive at the target directory from pathnames

int aie_arcextract(char* target, aie_Archive* archive, const char* opt);
    // extract archive to the target directory

