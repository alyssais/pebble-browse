#include <pebble.h>
#include "document_view.h"
#include "document.h"
#include "substr.h"

struct Document {
  char *content; // owned
  char *url; // owned
  char *title; // owned
  struct DocumentView *view; // owned
};


struct Document *document_create(char *content, char *url) {
  struct Document *self = malloc(sizeof(struct Document));
  self->content = content;
  self->url = url;
  self->view = document_view_create(self);
  self->title = document_copy_title(self);
  return self;
}

struct DocumentView *document_get_view(struct Document *self) {
  return self->view;
}

Substr document_get_tag_content_range(struct Document *self, char *tag) {
  Substr range = { .string = self->content };

  char tag_start[strlen(tag) + 2];
  strcpy(tag_start, "<");

  strcat(tag_start, tag);
  char *tag_start_position = strstr(self->content, tag_start);
  char *tag_end_position = strstr(tag_start_position, ">");
  range.start = tag_end_position + 1;

  // currently just finds content until start of next tag
  char *tag_close_position = strstr(tag_end_position, "<");
  range.end = tag_close_position - 1;

  return range;
}

char *document_get_title(struct Document *self) {
  return self->title;
}

char *document_copy_title(struct Document *self) {
  Substr title_range = document_get_tag_content_range(self, "title");
  char *title = malloc(substrlen(title_range) + 1);
  substrcpy(title, title_range);
  return title;
}

void document_free(struct Document *self) {
  free(self->view);
  free(self->content);
  free(self->url);
  free(self->title);
  free(self);
}
