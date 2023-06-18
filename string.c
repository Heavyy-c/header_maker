#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <stdarg.h>
#include "string.h"
#include "consts.h"

struct string_item {
	char *str;
	struct string_item *next;
};

void string_list_init(struct string_list *list)
{
	list->first = NULL;
	list->last = NULL;
}

void string_list_append(struct string_list *list, char *str)
{
	struct string_item *tmp = malloc(sizeof(struct string_item));
	tmp->next = NULL;
	tmp->str = strdup(str);
	if(!(list->first))
		list->first = tmp;
	else
		list->last->next = tmp;
	list->last = tmp;
}

static void set_sep(char *c, char *sep)
{
	switch(*sep)
	{
		case STR_SEP_NONE:
			return;
		default:
			strcpy(c, sep);
	}
}

void string_list_join(struct string_list list, char **dest, char *sep)
{
	*dest = NULL;
	int pos, len = 0, count;
	struct string_list cpy = list;
	for(count = 0; list.first; list.first = list.first->next, count++)
		len += strlen(list.first->str);
	switch(*sep)
	{
		case STR_SEP_NONE:
			*dest = malloc(len + 1);
			break;
		default:
			*dest = malloc(len + strlen(sep) * count + 1);
			break;
	}
	list = cpy;
	for(pos = 0; list.first; list.first = list.first->next)
	{
		strcpy(((*dest)+pos), list.first->str);
		set_sep(((*dest)+pos+strlen(list.first->str)), sep);
		pos += sep == STR_SEP_NONE ?
			strlen(list.first->str) :
				strlen(list.first->str) + strlen(sep);
	}
	len = sep == STR_SEP_NONE ? len : len + strlen(sep) * count;
	(*dest)[len] = EOS_CHAR;
}

void string_list_free(struct string_list *list)
{
	struct string_item *tmp;
	for(; list->first; )
	{
		tmp = list->first;
		list->first = list->first->next;
		free(tmp->str);
		free(tmp);
	}
	string_list_init(list);
}

void string_to_upper(char **str)
{
	char *temp = *str;
	for(; temp && *temp; temp++)
		*temp = toupper(*temp);
}

void string_get_bare_file_name(char *name, char **res)
{
	*res = strdup(name);
	char *pos = strchr(*res, DOT_CHAR);
	if(pos)
		*pos = EOS_CHAR;
}

void string_concatenate(char **dest, ...)
{
	int len = 0, len_used = 0;
	char *tmp;
	va_list args;
	va_start(args, dest);
	while((tmp = va_arg(args, char*)) != NULL)
		len += strlen(tmp);
	va_end(args);
	
	*dest = malloc(sizeof(char) * (len+1));

	va_start(args, dest);
	while((tmp = va_arg(args, char*)) != NULL)
	{
		strncpy((*dest)+len_used, tmp, strlen(tmp));
		len_used += strlen(tmp);
	}
	va_end(args);
	(*dest)[len] = EOS_CHAR;
}

void string_get_line(char **src, char **dest)
{
	*dest = NULL;
	char *eol = strchr(*src, EOL_CHAR);
	if(!eol)
		return;
	int len = eol - *src;
	*dest = malloc(len + 1);
	strncpy(*dest, *src, len);
	(*dest)[len] = EOS_CHAR;
	if(eol == *src)
		len += 1;
	*src = *src + len;
}

