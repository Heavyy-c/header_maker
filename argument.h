#ifndef ARGUMENT_H_SENTRY
#define ARGUMENT_H_SENTRY

#define ARG_OUTPUT_SPEC	"-o"
#define ARG_FREE_SPEC	"\0"

void arg_pop_param(char **argv, char **res, char *param);

#endif
