#include <aie_formatter-util.h>

#include <string.h>

#include <aie_error.h>
#include <aie_util.h>

size_t aie_xor_array(const char* array, const char* xor, char* buf,
    size_t array_size, size_t xor_size, size_t buf_size)
{ // TODO: counters need better names
  size_t i = 0; // array counter
  size_t j = 0; // xor counter

  if(array_size == 0 || buf_size == 0) { // corner case
    return 0;
  }
  if(array == NULL || buf == NULL) { // nurupo~
    AIE_ERROR(aie_ENURUPO, "array or buf");
    return 0;
  }
  if(xor_size == 0) { // if there is no xor, just do memcpy()
    size_t size = MIN(array_size, buf_size);
    memcpy(buf, array, size);
    return size;
  }
  if(xor == NULL) { // nurupo~
    AIE_ERROR(aie_ENURUPO, "xor");
    return 0;
  }
  if(xor_size == 1) {
    return aie_xor1_array(array, *xor, buf, array_size, buf_size);
  }

  do {
    if(j == xor_size) { // go to the store and sell some more
      j = 0; // no more bottles of beer on the wall
    }
    buf[i] = array[i] ^ xor[j];
    i = i + 1;
    j = j + 1;
  } while(i < array_size && i < buf_size);

  return i;
}

size_t aie_xor1_array(const char* array, char xor, char* buf,
    size_t array_size, size_t buf_size)
{
  size_t i = 0;

  if(array_size == 0 || buf_size == 0) {
    return 0;
  }
  if(array == NULL || buf == NULL) {
    AIE_ERROR(aie_ENURUPO, "array or buf");
    return 0;
  }
  if(xor == 0) { // if xor is zero, just do memcpy()
    size_t size = MIN(array_size, buf_size);
    memcpy(buf, array, size);
    return size;
  }

  for(; i < array_size && i < buf_size; i++) {
    buf[i] = array[i] ^ xor;
  }

  return i;
}

