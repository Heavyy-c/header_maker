#ifndef STRING_H_SENTRY
#define STRING_H_SENTRY

enum {	SPACE_CHAR=' ', TABUL_CHAR='\t', EOL_CHAR='\n',
	EOS_CHAR='\0', DOT_CHAR='.', SEMICOLON_CHAR
};

void string_get_name(char *src, char **dest);
void string_get_body(char *src, char **dest);
void string_to_upper(char **str);
void string_concatenate(char **dest, ...);
int string_get_word(const char *src, char **dest);
void string_clear_spaces(char **str);
void string_skip_word(char **str);
void string_remove_char(char **str, char c);

#endif
