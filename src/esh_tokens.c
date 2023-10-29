#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include "esh_tokens.h"

char *strduplicate(char *org) {
    unsigned int org_size;
    static char *dup;
    char *dup_offset;

    /* Allocate memory for duplicate */
    org_size = strlen(org);
    dup = (char *) malloc(sizeof(char) * org_size + 1);
    if (dup == NULL)
        return ((char *) NULL);

    /* Copy string */
    dup_offset = dup;
    while (*org) {
        *dup_offset = *org;
        dup_offset++;
        org++;
    }
    *dup_offset = '\0';

    return (dup);
}

esh_tokens *esh_tokenize(char *input) {
    esh_tokens *tokens = malloc(sizeof(char *));
    tokens->n = 0;
    char delim = ' ';
    char *token = strtok(input, &delim);

    if (token) {
        tokens->tokens = malloc(sizeof(char *));
        tokens->tokens[0] = strduplicate(token);
        tokens->n++;
    }

    while ((token = strtok(NULL, &delim))) {
        char **allocated = realloc(tokens->tokens, tokens->n * sizeof(char *));
        assert(allocated);

        tokens->tokens = allocated;
        tokens->tokens[tokens->n] = strduplicate(token);
        tokens->n++;
    }

    return tokens;
}

void esh_print_tokens(esh_tokens *t) {
    printf("Printing tokens: \n\t");
    for (size_t i = 0; i < t->n; ++i) {
        printf("%ld: %s\n\t", i, t->tokens[i]);
    }

    printf("\n");
}


void esh_free_tokens(esh_tokens *t) {
    for (size_t i = 0; i < t->n; ++i) {
        free(t->tokens[i]);
    }
    free(t->tokens);
}
