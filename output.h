#ifndef OUTPUT_H_SENTRY
#define OUTPUT_H_SENTRY

void output_file_write(int file, const char *str);
void output_file_init(int file, char *name);
void output_file_shutdown(int file);

#endif

