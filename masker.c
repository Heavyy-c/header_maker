#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "string.h"
#include "consts.h"

int mask_copy_zone(char *src)
{
	int len;
	char *start, *end;
	for(;;)
	{
		start = strstr(src, COPY_ZONE_START);
		end = strstr(src, COPY_ZONE_END);
		if(!start ^ !end)
			return COPY_ZONE_DEFINITION_ERR;
		else if(!start && !end)
			break;
		len = end - start + strlen(COPY_ZONE_END);
		memset(start, MASK_SYMBOL, len);
		src = start + len;
	}
	return COPY_OK;
}

int read_copy_zone(char *src, char **dest)
{
	struct string_list str_list;
	string_list_init(&str_list);
	int len;
	char *start, *end, *tmp;
	for(;;)
	{
		start = strstr(src, COPY_ZONE_START);
		end = strstr(src, COPY_ZONE_END);
		if(!start ^ !end)
			return COPY_ZONE_DEFINITION_ERR;
		else if(!start && !end)
			break;
		len = end - start + strlen(COPY_ZONE_END);
		tmp = malloc(len + 1);
		memcpy(tmp, start, len);
		src = start + len;
		string_list_append(&str_list, tmp);
		free(tmp);
	}
	string_list_join(str_list, dest, "\0");
	string_list_free(&str_list);
	return COPY_OK;
}

void mask_func_bodies(char *src)
{
	int reading;
	char *cur, *line;
	for(reading = 0, cur = src; ;)
	{
		string_get_line(&cur, &line);
		if(!line)
			break;
		if(reading && (strcmp(line, FUNC_BODY_END) == 0))
			reading = 0;
		else if(strcmp(line, FUNC_BODY_START) == 0)
			reading = 1;
		if(reading || (strcmp(line, FUNC_BODY_END) == 0))
			memset(src, MASK_SYMBOL, strlen(line));
		src = cur;
	}
}

static int masked(char *line)
{
	return (strchr(line, MASK_SYMBOL) != NULL);
}

void read_func_headers(char *src, char **dest)
{
	*dest = NULL;
	char *line;
	struct string_list list;
	string_list_init(&list);
	for(;;)
	{
		string_get_line(&src, &line);
		if(!line)
			break;
		if(!masked(line) && strlen(line))
			string_list_append(&list, line);
	}
	string_list_join(list, dest, ";\n");
	string_list_free(&list);
}

