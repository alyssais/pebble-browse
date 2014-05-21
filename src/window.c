#include <pebble.h>
#include "document.h"
#include "window.h"

static Window *window;

void window_display_document(struct Document *document) {
	Layer *window_layer = window_get_root_layer(window);
	struct DocumentView *view = document_get_view(document);
	Layer *layer = document_view_layer_for_bounds(view, layer_get_bounds(window_layer));
	layer_add_child(window_layer, layer);
}

void window_init() {
	window = window_create();
	window_stack_push(window, true);
}

void window_deinit() {
	window_destroy(window);
}
