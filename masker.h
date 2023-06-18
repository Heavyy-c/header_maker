#ifndef MASKER_H_SENTRY
#define MASKER_H_SENTRY

int mask_copy_zone(char *src);
int read_copy_zone(char *src, char **dest);
void mask_func_bodies(char *src);
void read_func_headers(char *src, char **dest);

#endif
