#ifndef FILE_H_SENTRY
#define FILE_H_SENTRY

#include "header.h"

void file_init(int file, char *name);
void file_append_headers(int file, struct header_list list);
void file_end(int file);

#endif
