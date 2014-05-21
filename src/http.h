#pragma once
#include "document.h"

typedef void (HttpInitCallback)(void);
typedef void (DocumentHandler)(struct Document *);

void http_init(void);
void http_get(char *url);
void http_set_document_handler(DocumentHandler *handler);
void http_deinit(void);
