#include <pebble.h>
#include "window.h"
#include "http.h"

static void request_page(void) {
	http_get("http://alyssa.is");
}

static void init(void) {
	http_init(request_page);
	http_set_document_handler(window_display_document);
	window_init(void);
}

static void deinit(void) {
	window_deinit(void);
	http_deinit(void);
}

int main(void) {
	init(void);
	app_event_loop(void);
	deinit(void);
}
