#include <pebble.h>
#include "substr.h"

uint32_t substrlen(Substr range) {
  return range.end - range.start + 1;
}

char *substrcpy(char *destination, Substr range) {
  uint32_t length = substrlen(range);
  destination[length] = 0;
  return strncpy(destination, range.start, length);
}
