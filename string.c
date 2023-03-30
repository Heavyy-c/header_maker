#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include "string.h"
#include "stdarg.h"

#define POSTFIX_STR ".h"
#define POSTFIX_LEN strlen(POSTFIX_STR)
#define EOS_CHAR '\0'
#define DOT_CHAR '.'

enum {SPACE_CHAR=' ', TABUL_CHAR='\t'};

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
			return 1;
		default:
			return 0;
	}
}

static int get_stream_word_len(int stream)
{
	int len, count;
	char c;
	for(len = 0; (count = read(stream, &c, 1)); len++)
		if(is_space(c))
			break;
	return len;
}

static void read_stream_word(int stream, char **dest)
{
	int pos = lseek(stream, SEEK_CUR, 0), count = 1;
	int len = get_stream_word_len(stream);
	lseek(stream, SEEK_SET, pos);
	*dest = malloc(sizeof(char) * (len+1));
	for(int i = 0;(i < len) && count; i++)
	{
		count = read(stream, ((*dest)+i), 1);
	}
	(*dest)[len] = EOS_CHAR;
}

static int get_buffer_word_len(char *source)
{
	int len;
	for(len = 0; source && *source; source++, len++)
		if(is_space(*source))
			break;
	return len;
}

static void read_buffer_word(char *source, char **dest)
{
	int len = get_buffer_word_len(source);
	*dest = malloc(sizeof(char) * (len+ 1));
	strncpy(*dest, source, len);
	(*dest)[len] = EOS_CHAR;
}

void string_get_word(char **dest, enum SOURCE_TYPE type, ...)
{
	va_list args;
	char *buffer;
	int stream, save_file_pos;
	va_start(args, type);
	switch(type)
	{
		case SOURCE_TYPE_STREAM:
			stream = va_arg(args, int);
			save_file_pos = va_arg(args, int);
			int pos = lseek(stream, SEEK_CUR, 0);
			read_stream_word(stream, dest);
			if(save_file_pos)
				lseek(stream, SEEK_SET, pos);
			break;
		case SOURCE_TYPE_BUFFER:
			buffer = va_arg(args, char*);
			read_buffer_word(buffer, dest);
			break;
	}
	va_end(args);
}

