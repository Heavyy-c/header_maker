#include <stdlib.h>
#include "type_searcher.h"

#define TYPE_ENUM_WORD "enum"
#define TYPE_STRUCT_WORD "struct"
#define TYPE_UNION_WORD "union"

typedef struct type_item{
	char *name;
	struct type_item *next;
} type_item;

typedef struct {
	type_item *first, *last;
} type_list;

static append_type_list(type_list *list, const char *type_name)
{
}

static int find_typedef_word(char **src)
{
	char *word = NULL;
	int word_found = -1;
	for(;src && *src && **src;)
	{
		string_clear_spaces(src);
		string_get_word(*src, &word);
		if(strcmp(word, TYPE_TYPEDEF_WORD) == 0)
		{
			*src -= strlen(word);
			word_found = 1;
			break;
		}
	}
	return word_found;
}

static int composite_type(const char *word)
{
	return	(strcmp(word, TYPE_STRUCT_WORD) == 0) ||
		(strcmp(word, TYPE_UNION_WORD) == 0) ||
		(strcmp(word, TYPE_ENUM_WORD) == 0);
}

static void skip_body_defenition(char **src)
{
	for(;src && *src; (*src)++)
	{
		if(*src == FIGURE_BRACKET_CLOSE)
			break;
	}
}

static int find_type(char **src, char **dest)
{
	*dest = NULL;
	int distance = find_typedef_word(src);
	if(distance == -1)
		return 0;
	*src += distance;
	string_skip_word(*src); /* skip typedef word */
	string_read_word(*src, *dest); /* simple or composite type? */
	if(composite_type(*dest))
	{
		free(*dest);
		skip_body_definition(*src);
		string_read_word(*src, *dest);
	}
	else
	{
		string_skip_word(*src);
		string_read_word(*src, *dest);
	}
	string_remove_char(*dest, SEMICOLON_CHAR);
}

int type_find_list(const char *src, type_list *list)
{
	char *dest;
	do
	{
		find_type(&src, &dest);
		if(dest)
		{
			append_type_list(list, dest);
			free(dest);
		}
	} while(dest);
}
