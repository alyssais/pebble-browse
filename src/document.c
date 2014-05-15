#include <pebble.h>
#include "document.h"
#include "substr.h"

Substr document_get_tag_content_range(char *doc, char *tag) {
  Substr range = { .string = doc };

  char tag_start[strlen(tag) + 2];
  strcpy(tag_start, "<");

  strcat(tag_start, tag);
  char *tag_start_position = strstr(doc, tag_start);
  char *tag_end_position = strstr(tag_start_position, ">");
  range.start = tag_end_position + 1;

  // currently just finds content until start of next tag
  char *tag_close_position = strstr(tag_end_position, "<");
  range.end = tag_close_position - 1;

  return range;
}
