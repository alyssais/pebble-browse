#include <pebble.h>
#include "document.h"
#include "window.h"

static Window *window;
static struct Document *current_document;

void window_display_document(struct Document *document) {
	current_document = document;
	Layer *window_layer = window_get_root_layer(window);
	struct DocumentView *view = document_get_view(document);
	Layer *layer = document_view_layer_for_bounds(view, layer_get_bounds(window_layer));
	layer_add_child(window_layer, layer);
}

void window_init(void) {
	window = window_create(void);
	window_stack_push(window, true);
}

void window_deinit(void) {
	if (current_document) document_free(current_document);
	window_destroy(window);
}
