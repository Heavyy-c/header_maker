#ifndef STRING_H_SENTRY
#define STRING_H_SENTRY

void string_get_name(char *src, char **dest);
void string_get_body(char *src, char **dest);
void string_to_upper(char **str);
void string_concatenate(char **dest, ...);

#endif
