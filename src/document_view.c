// #include <pebble.h>
#include "document.h"
#include "document_view.h"

struct DocumentView {
  struct Document *document;
  GRect bounds;
  Layer *layer; // owned
  TextLayer *title_layer; // owned
};

struct DocumentView *document_view_create(struct Document *document) {
  struct DocumentView *self = malloc(sizeof(struct DocumentView));
  self->document = document;
  return self;
}

GRect document_view_get_bounds(struct DocumentView *self) {
  return self->bounds;
}

void document_view_set_bounds(struct DocumentView *self, GRect bounds) {
  self->bounds = bounds;
}

Layer *document_view_get_layer(struct DocumentView *self) {
  if (self->layer == NULL) {
    self->layer = layer_create(self->bounds);

    GRect title_layer_frame = self->bounds;;
    title_layer_frame.size.h = 50;
    self->title_layer = text_layer_create(title_layer_frame);
    text_layer_set_text(self->title_layer, document_get_title(self->document));
    layer_add_child(self->layer, text_layer_get_layer(self->title_layer));
  }

  return self->layer;
}

void document_view_free(struct DocumentView *self) {
  free(self->title_layer);
  free(self->layer);
  free(self);
}
