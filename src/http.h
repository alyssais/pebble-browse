#pragma once
#include "document.h"

typedef void (DocumentHandler)(struct Document *);

void http_init();
void http_set_document_handler(DocumentHandler *handler);
void http_deinit();
