#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include "string.h"
#include "stdarg.h"

#define POSTFIX_STR ".h"
#define POSTFIX_LEN strlen(POSTFIX_STR)

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

static int is_space(char c)
{
	switch(c)
	{
		case SPACE_CHAR:
		case TABUL_CHAR:
		case EOL_CHAR:
			return 1;
		default:
			return 0;
	}
}

static int get_word_len(const char *source)
{
	int len;
	for(len = 0; source && *source; source++, len++)
		if(is_space(*source))
			break;
	return len;
}

int string_get_word(const char *src, char **dest)
{
	int len = get_word_len(src);
	*dest = malloc(sizeof(char) * (len+ 1));
	strncpy(*dest, src, len);
	(*dest)[len] = EOS_CHAR;
	return len;
}

void string_clear_spaces(char **str)
{
	int len;
	for(len = 0; str && *str && **str && is_space((*str)[len]); len++)
	{}
	*str = (*str)+len;
}

void string_skip_word(char **str)
{
	char *tmp = NULL;
	string_clear_spaces(str);
	*str += string_get_word(*str, &tmp);
	free(tmp);
}

void string_remove_char(char **str, char c)
{
	int i, k;
	if(!str)
		return;
	char *tmp = malloc(sizeof(char) * (strlen(*str)+1));
	for(i = 0, k = 0; *str && (*str)[k]; k++)
	{
		if((*str)[k] != c)
		{
			tmp[i] = (*str)[k];
			i++;
		}
	}
	tmp[i] = EOS_CHAR;
	free(*str);
	*str = tmp;
}

