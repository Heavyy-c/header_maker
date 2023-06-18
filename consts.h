#ifndef CONSTS_H_SENTRY
#define CONSTS_H_SENTRY

#define COPY_ZONE_START "/*COPYS*/"
#define COPY_ZONE_END "/*COPYE*/"
#define COPY_ZONE_DEFINITION_ERR 4
#define FUNC_BODY_START "{"
#define FUNC_BODY_END "}"

#define IFNDEF_DIRECTIVE	"#ifndef"
#define DEFINE_DIRECTIVE	"#define"
#define ENDIF_DIRECTIVE		"#endif"
#define POSTFIX			"_H_SENTRY"

enum {
	SPACE_CHAR = ' ', EOL_CHAR = '\n',
	TAB_CHAR = '\t', MASK_SYMBOL = '#',
	OPEN_ROUND_BRACKET = '(', COPY_OK = 0,
	EOS_CHAR='\0', DOT_CHAR='.'
};

#endif
