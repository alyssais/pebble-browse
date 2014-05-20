#pragma once
#include <pebble.h>

struct DocumentView;
struct Document;

struct DocumentView *document_view_create(struct Document *document);
GRect document_view_get_bounds(struct DocumentView *view);
void document_view_set_bounds(struct DocumentView *view, GRect bounds);
Layer *document_view_get_layer(struct DocumentView *view);
