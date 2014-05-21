#pragma once
#include "document.h"

typedef void (HttpInitCallback)();
typedef void (DocumentHandler)(struct Document *);

void http_init();
void http_get(char *url);
void http_set_document_handler(DocumentHandler *handler);
void http_deinit();
