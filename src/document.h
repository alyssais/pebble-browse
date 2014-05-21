#pragma once
#include "document_view.h"
#include "substr.h"

struct Document;
struct DocumentView;

struct Document *document_create(char *content, char *url);
struct DocumentView *document_get_view(struct Document *document);
Substr document_get_tag_content_range(struct Document *document, char *tag);
char *document_get_title(struct Document *document);
char *document_copy_title(struct Document *document);
void document_free(struct Document *document);
