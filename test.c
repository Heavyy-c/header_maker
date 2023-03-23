#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

#define FUNCTION_LOCAL_LABEL_STR "static"
#define BODY_START_CHAR "{"
#define BODY_END_CHAR "}"

#define BUFFER_SIZE 81

#define EOL_MACRO(C) ((C) == '\n')
#define EOF_MACRO(C) ((C) == '\0')
#define EOS_MACRO(C) ((C) == '\0')
#define BODY_START_MACRO(STR) (strstr((STR), BODY_START_CHAR))
#define BODY_END_MACRO(STR) (strstr((STR), BODY_END_CHAR))

enum {
	EOS_CHAR = '\0',
	HEADER_READ_OVERFLOW = -1,
	HEADER_READ_OK = 1,
	HEADER_BRACKET_SET_ERR = 2,
	HEADER_DATA_END = 3
};

static int get_line(int file, char *buf, int size)
{
	int i, count;
	char c;
	for(i = 0; (count = read(file, &c, 1)) && (i < size-1); i++)
	{
		if(EOL_MACRO(c) || EOF_MACRO(c) || !count)
		{
			buf[i] = EOS_CHAR;
			break;
		}
		buf[i] = c;
	}
	if(!i && !count)
		return HEADER_DATA_END;
	if(!EOS_MACRO(buf[i]))
		return HEADER_READ_OVERFLOW;
	return HEADER_READ_OK;
}

int main()
{
	char buf[BUFFER_SIZE];
	int file = open("header.c", O_RDONLY), count;
	for(count = 0; ; count++)
	{
		int state = get_line(file, buf, BUFFER_SIZE);
		if(state == HEADER_READ_OK)
		{
			printf("OK: %i\n", count);
		}
		if(state == HEADER_READ_OVERFLOW)
		{
			printf("OVERFLOW: %i\n", count);
		}
		if(state == HEADER_DATA_END)
		{
			printf("END\n");
			return 0;
		}
	}
}
