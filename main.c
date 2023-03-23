#include <stdio.h>
#include <fcntl.h>
#include "header.h"


#ifdef DEBUG
int main(int argc, char **argv)
{
	struct header_list list;
	header_list_init(&list);
	if(argc < 2)
		return 1;
	int file = open(argv[1], O_RDONLY);
	if(file == -1)
		return 2;
	int state = header_read_all(file, &list);
	if(state != HEADER_READ_OK)
	{
		printf("HEADER READ ERR\n");
		return state;
	}
	struct header_item *current = list.first;
	for(; current; current = current->next)
	{
		printf("%s\n", current->str);
	}
	printf("_________________________\n");
	header_clear_locals(&list);
	/* struct header_item */ current = list.first;
	for(; current; current = current->next)
	{
		printf("%s\n", current->str);
	}
	return 0;
}
#else
int main(int argc, char **argv)
{
	/* Argument error handler */

	/* File opening + error handler; result in 'file' */

	/* Reading all unit's headers + error handling */
	struct fposition_type pos_err;
	int success = header_read_all(src_file, &headers);
	if(success != HEADER_READ_OK)
	{
		header_print_err_log(success, pos);
		close(src_file);
		exit(InfoReadingErrCode);
	}
	/* Getting all global ?(may other)? headers of units */
	success = header_clear_locals(&headers);

	/* Writing in file selected headers + error handling */
	success = file_write_headers(headers);
	if(success != FILE_WRITE_OK)
	{
		file_print_err_log(success);
		close(src_file);
		exit(FileWritingErrCode);
	}

	/* Closing files, printing? all special info */
	close(src_file);
	close(dest_file);

	return 0;
}
#endif
