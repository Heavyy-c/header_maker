#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "argument.h"

enum {PARAM_START = '-', PARAM_SPEC_LEN = 2};

static int is_spec_param(char *str)
{
	return str && (*str == PARAM_START) && (strlen(str) == PARAM_SPEC_LEN);
}

static int is_free_param(char *str)
{
	return str && strlen(str) == 0;
}

static void search_free_param(char **argv, char **res)
{
	int skip;
	for(skip = 0; *argv; argv++)
	{
		if(skip)
		{
			skip = 0;
			continue;
		}
		if(is_spec_param(*argv))
			skip = 1;
		else
			break;
	}
	if(*argv)
		*res = strdup(*argv);
	else
		*res = NULL;
}

static void search_spec_param(char **argv, char **res, char *param)
{
	for(; *argv && strcmp(*argv, param) != 0; argv++)
	{}
	if((*argv) && (*(argv+1)))
		*res = strdup(*(argv+1));
	else
		*res = NULL;
}

void arg_pop_param(char **argv, char **res, char *param)
{
	argv += 1;	/* to erase program name */
	if(is_free_param(param))
		search_free_param(argv, res);
	else
		search_spec_param(argv, res, param);
}

