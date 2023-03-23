#include "header.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define FUNCTION_LOCAL_LABEL_STR "static"
#define ARG_START_STR "("
#define ARG_END_STR ")"
#define BODY_START_STR "{"
#define BODY_END_STR "}"

#define ROUND_OPEN_BRACKET_STR "("

#define EOL(C) ((C) == '\n')
#define EOS(C) ((C) == '\0')


#define BODY_START(STR) (strstr((STR), BODY_START_STR))
#define BODY_END(STR) (strstr((STR), BODY_END_STR))

enum {READ_STRING_SIZE = 83};
enum {SPACE_CHAR = ' ', TABUL_CHAR = '\t'};
enum {
	ROUND_CLOSE_BRACKET_CHAR = ')',
	FIGURE_OPEN_BRACKET_CHAR = '{',
	FIGURE_CLOSE_BRACKET_CHAR = '}'
};

void header_list_init(struct header_list *list)
{
	list->first = NULL;
	list->last = NULL;
}

static void list_append(struct header_list *list, const char *str)
{
	struct header_item *tmp = malloc(sizeof(struct header_item));
	tmp->str = malloc(strlen(str)+1);
	strcpy(tmp->str, str);
	tmp->str[strlen(str)] = '\0';
	tmp->next = NULL;
	tmp->prev = list->last;
	if(!list->first)
		list->first = tmp;
	else
		list->last->next = tmp;
	list->last = tmp;
}

static void list_free_item(struct header_list *list, struct header_item *item)
{
	if(!item->prev)
		list->first = item->next;
	else
		item->prev->next = item->next;
	if(!item->next)
		list->last = item->prev;
	else
		item->next->prev = item->prev;
	free(item->str);
	free(item);
}

static int get_line(int file, char *buf, int size)
{
	int i, count;
	char c;
	for(i = 0; (count = read(file, &c, 1)) && (i < size-1); i++)
	{
		if(EOL(c) || !count)
		{
			buf[i] = EOS_CHAR;
			break;
		}
		buf[i] = c;
	}
	if(!i && !count)
		return HEADER_DATA_END;
	if(!EOS(buf[i]))
		return HEADER_READ_OVERFLOW;
	return HEADER_READ_OK;
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

static void get_first_word(char *source, char **dest)
{
	int count;
	char *tmp = source;
	for(count = 0; tmp && *tmp; tmp++, count++)
	{
		if(is_space(*tmp))
			break;
	}
	*dest = malloc(sizeof(char) * (count+1));
	strncpy(*dest, source, count);
	(*dest)[count] = EOS_CHAR;
}

static int is_type(char *str)
{
	char *types[] = {"void", "char", "struct", "int", "float",
			"double", "long", "enum", "static"};
	int res, i;
	for(res = 0, i = 0; !res && str && *str &&
			(i < sizeof(types) / sizeof(char*)); i++)
	{
		res = res || !strcmp(str, types[i]);
	}
	return res && strlen(str);
}

static int is_function(char *str)
{
	char *word;
	get_first_word(str, &word);
	int res = is_type(word) && strstr(str, ROUND_OPEN_BRACKET_STR);
	free(word);
	return res;
}

static void skip_func_body(int file)
{
	char c;
	int inner, count;
	for(inner = 1; inner && (count = read(file, &c, 1)); )
	{
		switch(c) {
			case FIGURE_OPEN_BRACKET_CHAR:
				inner++;
				break;
			case FIGURE_CLOSE_BRACKET_CHAR:
				inner--;
				break;
		}
	}
}

static int get_func_length(int file)
{
	char c;
	int count, len, reading = 1;
	int saved_pos = lseek(file, 0, SEEK_CUR);
	for(len = 0; (count = read(file, &c, 1)) && reading; len++)
	{
		if(c == ROUND_CLOSE_BRACKET_CHAR)
			reading = 0;
	}
	lseek(file, saved_pos, SEEK_SET);
	return len;
}

static void read_function_head(int file, char **dest)
{
	char c;
	int count, i, length = get_func_length(file) + 1;
	*dest = malloc(sizeof(char) * length);
	for(i = 0; (i < length - 1) && (count = read(file, &c, 1)); i++)
	{
		(*dest)[i] = c;
	}
	(*dest)[length] = EOS_CHAR;
}

int header_read_all(int file, struct header_list *list)
{
	char buf[READ_STRING_SIZE], *tmp_str;
	int saved_pos = lseek(file, 0, SEEK_CUR), state;
	lseek(file, 0, SEEK_SET);
	for(;;)
	{
		state = get_line(file, buf, READ_STRING_SIZE);
		if(state == HEADER_DATA_END)
			break;
		if(state == HEADER_READ_OVERFLOW)
			return HEADER_READ_OVERFLOW;
		if(is_function(buf))
		{
			lseek(file, -strlen(buf)-1, SEEK_CUR);
			read_function_head(file, &tmp_str);
			list_append(list, tmp_str);
		}
		if(BODY_START(buf) && !BODY_END(buf))
			skip_func_body(file);
	}
	lseek(file, saved_pos, SEEK_SET);
	return HEADER_READ_OK;
}

static int local_func(char *str)
{
	return (strstr(str, FUNCTION_LOCAL_LABEL_STR) != NULL);
}

/* func line end retransmit (new line) - should be realised */
void header_clear_locals(struct header_list *list)
{
	struct header_item *current = list->first;
	for(; current; current = current->next)
		if(local_func(current->str))
			list_free_item(list, current);
}

