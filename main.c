#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include "masker.h"
#include "argument.h"
#include "output.h"

#define ARG_ERR_MSG "No arguments detected; please specify source" \
			" and destination file (the last with -o)"
#define OUTPUT_FILE_SPEC_ERR_MSG "No output file specified;" \
					"set it by -o param"
#define SOURCE_FILE_SPEC_ERR_MSG "No source file specified"

enum {
	FILE_SPEC_ERR = 1,
	FILE_OPEN_ERR, FSTAT_ERR, MMAP_ERR
};

int main(int argc, char **argv)
{
	struct stat output_file_stat, source_file_stat;
	int output_file, source_file, state, size;
	char *output_file_name, *output_file_mmap;
	char *source_file_name, *source_file_mmap;
	char *headers, *copy_zone_saved, *copy_zone;
	if(argc == 1)
	{
		printf("%s\n", ARG_ERR_MSG);
		return FILE_SPEC_ERR;
	}
	arg_pop_param(argv, &output_file_name, ARG_OUTPUT_SPEC);
	if(!output_file_name)
	{
		printf("%s\n", OUTPUT_FILE_SPEC_ERR_MSG);
		return FILE_SPEC_ERR;
	}
	arg_pop_param(argv, &source_file_name, ARG_FREE_SPEC);
	if(!source_file_name)
	{
		printf("%s\n", SOURCE_FILE_SPEC_ERR_MSG);
		return FILE_SPEC_ERR;
	}
	source_file = open(source_file_name, O_RDONLY);
	if(source_file == -1)
	{
		perror(source_file_name);
		return FILE_OPEN_ERR;
	}
	output_file = open(output_file_name, O_RDWR | O_CREAT, 0666);
	if(output_file == -1)
	{
		perror(output_file_name);
		return FILE_OPEN_ERR;
	}
	state = fstat(output_file, &output_file_stat);
	if(state != 0)
	{
		perror(output_file_name);
		return FSTAT_ERR;
	}
	state = fstat(source_file, &source_file_stat);
	if(state != 0)
	{
		perror(source_file_name);
		return FSTAT_ERR;
	}
	size = ((output_file_stat.st_size - 1) / getpagesize() + 1)
						* getpagesize();
	output_file_mmap = mmap(NULL, size, PROT_READ,
					MAP_PRIVATE, output_file, 0);
	if(output_file_mmap == (void *)(-1))
	{
		perror(output_file_name);
		return MMAP_ERR;
	}
	if(output_file_stat.st_size)
		read_copy_zone(output_file_mmap, &copy_zone_saved);
	munmap(output_file_mmap, size);

	size = ((source_file_stat.st_size - 1) / getpagesize() + 1)
						* getpagesize();
	source_file_mmap = mmap(NULL, size, PROT_READ | PROT_WRITE,
					MAP_PRIVATE, source_file, 0);
	if(source_file_mmap == (void *)(-1))
	{
		perror(source_file_name);
		return MMAP_ERR;
	}
	read_copy_zone(source_file_mmap, &copy_zone);
	mask_copy_zone(source_file_mmap);
	mask_func_bodies(source_file_mmap);
	read_func_headers(source_file_mmap, &headers);
	munmap(source_file_mmap, size);

	output_file_init(output_file, output_file_name);
	output_file_write(output_file, copy_zone_saved);
	output_file_write(output_file, copy_zone);
	output_file_write(output_file, headers);
	output_file_shutdown(output_file);
	close(output_file);
	close(source_file);
	return 0;
}
