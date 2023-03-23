#ifndef HEADER_H_SENTRY
#define HEADER_H_SENTRY

struct header_item {
	char *str;
	struct header_item *prev, *next;
};

struct header_list {
	struct header_item *first, *last;
};

enum {
	EOS_CHAR = '\0',
	HEADER_READ_OVERFLOW = -1,
	HEADER_READ_OK = 1,
	HEADER_BRACKET_SET_ERR = 2,
	HEADER_DATA_END = 3
};

void header_list_init(struct header_list *list);
int header_read_all(int file, struct header_list *list);
void header_clear_locals(struct header_list *list);

#endif
