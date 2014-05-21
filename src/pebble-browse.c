#include <pebble.h>
#include "substr.h"
#include "document.h"
#include "http.h"

// Old habits die hard.
#define printf(...) APP_LOG(APP_LOG_LEVEL_DEBUG, __VA_ARGS__)

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

static void init() {
	http_init();
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
