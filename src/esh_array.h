#ifndef ESH_TOKENS
#define ESH_TOKENS

#include <stdlib.h>

char **esh_tokenize(char *in);

void esh_array_print(char **ts);

void esh_array_free(char **ts);

void esh_array_remove_last(char **ts);

size_t esh_array_length(char **ts);

#endif
