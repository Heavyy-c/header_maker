#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "output.h"
#include "string.h"
#include "consts.h"

void output_file_write(int file, const char *str)
{
	if(str)
		write(file, str, strlen(str));
}

void output_file_init(int file, char *name)
{
	char *bare_name, *res_str;
	lseek(file, 0, SEEK_SET);
	string_get_bare_file_name(name, &bare_name);
	string_to_upper(&bare_name);
	string_concatenate(&res_str, IFNDEF_DIRECTIVE, " ", bare_name, POSTFIX,
			"\n", DEFINE_DIRECTIVE, " ", bare_name, POSTFIX,
			"\n", NULL);
	output_file_write(file, res_str);
	free(res_str);
	free(bare_name);
}

void output_file_shutdown(int file)
{
	char *res_str;
	string_concatenate(&res_str, "\n", ENDIF_DIRECTIVE, "\n", NULL);
	output_file_write(file, res_str);
	int size = lseek(file, 0, SEEK_CUR);
	ftruncate(file, size);
	free(res_str);
}

