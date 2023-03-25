#include <stdio.h>
#include <fcntl.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "header.h"
#include "string.h"
#include "file.h"

#define ARG_COUNT_ERR_MSG "Invalid argument count; --help for more info"

enum {
	MIN_ARG_COUNT = 2,
	ARG_COUNT_ERR_CODE = 1,
	FILE_OPEN_ERR_CODE = 2,
	FILE_DATA_ERR_CODE = 3
};

int main(int argc, char **argv)
{
	struct header_list list;
	char *name;
	header_list_init(&list);
	if(argc < MIN_ARG_COUNT)
	{
		fprintf(stderr, "%s\n", ARG_COUNT_ERR_MSG);
		return ARG_COUNT_ERR_CODE;
	}
	int file = open(argv[1], O_RDONLY);
	if(file == -1)
	{
		perror(argv[1]);
		return FILE_OPEN_ERR_CODE;
	}
	int state = header_read_global(file, &list);
	if(state != HEADER_READ_OK)
	{
		printf("HEADER READ ERR\n");
		return FILE_DATA_ERR_CODE;
	}
	string_get_name(argv[1], &name);

	int dest_file = open(name, O_WRONLY|O_CREAT|O_EXCL, 0666);
	if(dest_file == -1)
	{
		perror(argv[1]);
		return FILE_OPEN_ERR_CODE;
	}
	file_init(dest_file, argv[1]);
	file_append_headers(dest_file, list);
	file_end(dest_file);
	close(dest_file);
	return 0;
}
