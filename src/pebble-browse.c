#include <pebble.h>
#include "substr.h"
#include "document.h"

// Old habits die hard.
#define PEN_LOG(...) APP_LOG(APP_LOG_LEVEL_DEBUG, __VA_ARGS__)

enum {
	INBOX_SIZE_KEY = 0,
	RESPONSE_LENGTH_KEY = 1,
	CHUNK_KEY = 2
};

static Window *window;
static uint32_t response_length;
static char *response;
static struct Document *current_document;

static void create_window() {
	window = window_create();
	window_stack_push(window, true);
}

static void destroy_window() {
	window_destroy(window);
}

static void set_response_length(DictionaryIterator *dict) {
	Tuple *tuple = dict_find(dict, RESPONSE_LENGTH_KEY);
	if (!tuple) return;
	response_length = tuple->value->int32;
	if (response) free(response);
	response = malloc(response_length + 1);
	PEN_LOG("response length: %lu", response_length);
}

static void read_chunk(DictionaryIterator *dict) {
	Tuple *tuple = dict_find(dict, CHUNK_KEY);
	if (!tuple) return;
	char *chunk = tuple->value->cstring;
	strncat(response, chunk, response_length - strlen(response));
}


static void display_document(struct Document *document) {
	// locate the start index of title.
	// allow for any attributes
	PEN_LOG("displaying document %s", document_get_title(document));
	Layer *document_layer = document_view_get_layer(document_get_view(document));
	Layer *window_layer = window_get_root_layer(window);
	layer_add_child(window_layer, document_layer);
}

static void on_message(DictionaryIterator *dict, void *context) {
	set_response_length(dict);
	read_chunk(dict);
	if (strlen(response) == response_length) {
		current_document = document_create(response, NULL);
		display_document(current_document);
	}
}

/**
 * Sends inbox size to phone as a
 * little-endian byte array of length 4.
 */
static void broadcast_inbox_size(uint32_t size) {
	DictionaryIterator *dict;
	app_message_outbox_begin(&dict);
	dict_write_data(dict, INBOX_SIZE_KEY, (uint8_t *)&size, 4);
	dict_write_end(dict);
	app_message_outbox_send();
}

static void init_app_message() {
	uint32_t inbox_size = app_message_inbox_size_maximum();
	uint32_t outbox_size = app_message_outbox_size_maximum();

	app_message_register_inbox_received(on_message);
	app_message_open(inbox_size, outbox_size);

	broadcast_inbox_size(inbox_size);
}

static void init() {
	init_app_message();
	create_window();
}

static void deinit() {
	destroy_window();
	if (response) free(response);
}

int main() {
  init();
  app_event_loop();
  deinit();
}
