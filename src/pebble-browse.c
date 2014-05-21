#include <pebble.h>
#include "substr.h"
#include "document.h"
#include "http.h"

static Window *window;

static void create_window() {
	window = window_create();
	window_stack_push(window, true);
}

static void destroy_window() {
	window_destroy(window);
}

static void display_document(struct Document *document) {
	Layer *window_layer = window_get_root_layer(window);
	struct DocumentView *view = document_get_view(document);
	Layer *layer = document_view_layer_for_bounds(view, layer_get_bounds(window_layer));
	layer_add_child(window_layer, layer);
}

static void request_page() {
	http_get("http://alyssa.is");
}

static void init() {
	http_init(request_page);
	http_set_document_handler(display_document);
	create_window();
}

static void deinit() {
	destroy_window();
	http_deinit();
}

int main() {
  init();
  app_event_loop();
  deinit();
}
