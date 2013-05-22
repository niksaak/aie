/* aie_formatter-util.h - general internal utilities for formatters */

#pragma once

#include <aie_archive.h>

int aie_xor_extract(const aie_Archive* archive, const char* target,
                    const char* xor);
    // function for extracting non-compressed, possibly xored archives;
    // if xor == NULL, extracts data unchanged

int aie_xor_uextract(aie_ArcUnit unit, const char* target, const char* xor);
    // same as above, for ArcUnits

size_t aie_xor_umextract(aie_ArcUnit unit, char* buf,
                      size_t offset, size_t size, const char* xor);
    // same as above, streaming

size_t aie_xor_array(const char* array, const char* xor, char* buf,
                     size_t array_size, size_t xor_size, size_t buf_size);
    // write result of (array ^ xor) to buf, no more than buf_size;
    // if xor_size < array_size, xor will wrap around;
    // return amount of bytes written to buf

size_t aie_xor1_array(const char* array, char xor, char* buf,
                      size_t array_size, size_t buf_size);
    // same as aie_xor_array(), but xor with one char

