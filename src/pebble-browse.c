#include <pebble.h>
#include "substr.h"
#include "document.h"
#define printf(...) APP_LOG(APP_LOG_LEVEL_DEBUG, __VA_ARGS__)

enum {
	INBOX_SIZE_KEY = 0,
	RESPONSE_LENGTH_KEY = 1,
	CHUNK_KEY = 2
};

static Window *window;
static uint32_t response_length;
static char *response;

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
}

static void read_chunk(DictionaryIterator *dict) {
	Tuple *tuple = dict_find(dict, CHUNK_KEY);
	if (!tuple) return;
	char *chunk = tuple->value->cstring;
	strncat(response, chunk, response_length - strlen(response));
}

static void display_document(char *document) {
	// locate the start index of title.
	// allow for any attributes
	Substr title_range = document_get_tag_content_range(document, "title");
	char title[substrlen(title_range) + 1];
	substrcpy(title, title_range);
	printf("Extracted title: '%s'", title);
}

static void on_message(DictionaryIterator *dict, void *context) {
	set_response_length(dict);
	read_chunk(dict);
	if (strlen(response) == response_length) {
		display_document(response);
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
}

int main() {
  init();
  app_event_loop();
  deinit();
}
