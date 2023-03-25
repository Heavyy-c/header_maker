#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#include "string.h"
#include "file.h"

#define LOCKER_PREFIX "_H_SENTRY"
#define IFNDEF_WORD_WITH_SPACE "#ifndef "
#define LF_STR "\n"
#define DEFINE_WORD_WITH_SPACE "#define "
#define ENDIF_WORD "#endif"
#define SEMICOLON_STR ";"

void file_init(int file, char *name)
{
	char *lock_def, *tmp, *res;
	string_get_body(name, &tmp);
	string_concatenate(&lock_def, tmp, LOCKER_PREFIX, NULL);
	string_to_upper(&lock_def);
	string_concatenate(&res, IFNDEF_WORD_WITH_SPACE, 
			lock_def, LF_STR,
			DEFINE_WORD_WITH_SPACE, lock_def, 
			LF_STR, LF_STR, NULL);
	write(file, res, strlen(res));
	free(res);
	free(tmp);
}

void file_append_headers(int file, struct header_list list)
{
	struct header_item *current = list.first;
	for(; current; current = current->next)
	{
		write(file, current->str, strlen(current->str));
		write(file, SEMICOLON_STR, 1);
		write(file, LF_STR, 1);
	}
}

void file_end(int file)
{
	char *res;
	string_concatenate(&res, LF_STR, LF_STR, ENDIF_WORD, LF_STR, NULL);
	write(file, res, strlen(res));
	free(res);
}
