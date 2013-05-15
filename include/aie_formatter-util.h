#pragma once

#include <stdlib.h>

size_t aie_xor_array(const char* array, const char* xor, char* buf,
                     size_t array_size, size_t xor_size, size_t buf_size);
    // write result of (array ^ xor) to buf, no more than buf_size;
    // if xor_size < array_size, xor will wrap around;
    // return amount of bytes written to buf

size_t aie_xor1_array(const char* array, char xor, char* buf,
                      size_t array_size, size_t buf_size);
    // same as aie_xor_array(), but xor with one char

