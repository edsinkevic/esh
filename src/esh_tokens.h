#ifndef ESH_TOKENS
#define ESH_TOKENS

#include <stdlib.h>
typedef struct {
    size_t n;
    char **tokens;
} esh_tokens;

esh_tokens *esh_tokenize(char *input);

void esh_print_tokens(esh_tokens *t);

void esh_free_tokens(esh_tokens *t);

#endif
