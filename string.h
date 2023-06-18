#ifndef STRING_H_SENTRY
#define STRING_H_SENTRY

enum SOURCE_TYPE {SOURCE_TYPE_STREAM, SOURCE_TYPE_BUFFER};
enum {STR_SEP_NONE = '\0'};
struct string_item;

struct string_list {
	struct string_item *first, *last;
};

void string_list_init(struct string_list *list);
void string_list_append(struct string_list *list, char *str);
void string_list_join(struct string_list list, char **dest, char *sep);
void string_list_free(struct string_list *list);

void string_to_upper(char **str);
void string_get_bare_file_name(char *name, char **res);
void string_concatenate(char **dest, ...);
void string_get_line(char **src, char **dest);

#endif
