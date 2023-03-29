#ifndef STRING_H_SENTRY
#define STRING_H_SENTRY

enum SOURCE_TYPE {SOURCE_TYPE_STREAM, SOURCE_TYPE_BUFFER};

void string_get_name(char *src, char **dest);
void string_get_body(char *src, char **dest);
void string_to_upper(char **str);
void string_concatenate(char **dest, ...);
void string_get_word(char **dest, enum SOURCE_TYPE type, ...);

#endif
