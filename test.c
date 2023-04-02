#include <stdio.h>
#include <stdlib.h>
#include "string.h"

#define TEST_STR "LOLABOAAAAB sadad"

int main(int argc, char **argv)
{
	char *result;
	char test_str[] = "[LOLABOAAAAB sadad dasdasd    		\n new_word  heh]";
	int len = string_get_word(test_str, &result);
	printf("%s\n", result);
	string_remove_char(&result, 'A');
	printf("%s\n", result);
	free(result);
	result = test_str+len;
	string_clear_spaces(&result);
	printf("%s\n", result);
	string_skip_word(&result);
	string_skip_word(&result);
	string_clear_spaces(&result);
	printf("%s\n", result);
	return 0;
}
