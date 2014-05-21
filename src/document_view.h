#pragma once
#include <pebble.h>

struct DocumentView;
struct Document;

struct DocumentView *document_view_create(struct Document *document);
Layer *document_view_layer_for_bounds(struct DocumentView *view, GRect bounds);
void document_view_free(struct DocumentView *view);
