#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "string.h"
#include "stdarg.h"

#define POSTFIX_STR ".h"
#define POSTFIX_LEN strlen(POSTFIX_STR)
#define EOS_CHAR '\0'
#define DOT_CHAR '.'

static int get_body_name_len(char *src)
{
	int len;
	for(len = 0; src && *src; src++, len++)
		if(*src == DOT_CHAR)
			break;
	return len;
}

void string_get_name(char *src, char **dest)
{
	int len = get_body_name_len(src) + POSTFIX_LEN;
	*dest = malloc(sizeof(char) * (len+1));
	strncpy(*dest, src, len-POSTFIX_LEN);
	strcpy(((*dest)+len-POSTFIX_LEN), POSTFIX_STR);
	(*dest)[len] = EOS_CHAR;
}

void string_get_body(char *src, char **dest)
{
	int len = get_body_name_len(src);
	*dest = malloc(sizeof(char) * len+1);
	strncpy(*dest, src, len);
	(*dest)[len] = EOS_CHAR;
}

void string_to_upper(char **str)
{
	char *temp = *str;
	for(; temp && *temp; temp++)
		*temp = toupper(*temp);
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

