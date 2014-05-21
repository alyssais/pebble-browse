#include "document.h"
#include "document_view.h"

struct DocumentView {
  struct Document *document;
};

struct DocumentView *document_view_create(struct Document *document) {
  struct DocumentView *self = malloc(sizeof(struct DocumentView));
  self->document = document;
  return self;
}

Layer *document_view_layer_for_bounds(struct DocumentView *self, GRect bounds) {
  Layer *layer = layer_create(bounds);

  GRect title_layer_frame = bounds;
  title_layer_frame.size.h = 16;
  TextLayer *title_layer = text_layer_create(title_layer_frame);
  text_layer_set_font(title_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(title_layer, GTextAlignmentCenter);
  text_layer_set_text(title_layer, document_get_title(self->document));
  layer_add_child(layer, text_layer_get_layer(title_layer));

  return layer;
}

void document_view_free(struct DocumentView *self) {
  free(self);
}
