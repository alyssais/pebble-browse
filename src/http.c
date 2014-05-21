#include "http.h"

enum {
	INBOX_SIZE_KEY = 0,
	RESPONSE_LENGTH_KEY = 1,
	CHUNK_KEY = 2
};

static uint32_t response_length;
static char *response;
static DocumentHandler *document_handler;

void set_response_length(DictionaryIterator *dict) {
	Tuple *tuple = dict_find(dict, RESPONSE_LENGTH_KEY);
	if (!tuple) return;
	response_length = tuple->value->int32;
	if (response) free(response);
	response = malloc(response_length + 1);
}

void read_chunk(DictionaryIterator *dict) {
	Tuple *tuple = dict_find(dict, CHUNK_KEY);
	if (!tuple) return;
	char *chunk = tuple->value->cstring;
	strncat(response, chunk, response_length - strlen(response));
}

void on_message(DictionaryIterator *dict, void *context) {
	set_response_length(dict);
	read_chunk(dict);
	if (strlen(response) == response_length) {
		if (document_handler) {
			document_handler(document_create(response, NULL));
		}
	}
}

/**
 * Sends inbox size to phone as a
 * little-endian byte array of length 4.
 */
void broadcast_inbox_size(uint32_t size) {	
	DictionaryIterator *dict;
	app_message_outbox_begin(&dict);
	dict_write_data(dict, INBOX_SIZE_KEY, (uint8_t *)&size, 4);
	dict_write_end(dict);
	app_message_outbox_send();
}

void http_init() {
	uint32_t inbox_size = app_message_inbox_size_maximum();
	uint32_t outbox_size = app_message_outbox_size_maximum();

	app_message_register_inbox_received(on_message);
	app_message_open(inbox_size, outbox_size);

	broadcast_inbox_size(inbox_size);
}

void http_set_document_handler(DocumentHandler *handler) {
	document_handler = handler;
}

void http_deinit() {
	if (response) free(response);
}
