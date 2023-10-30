#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "esh_array.h"

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

size_t esh_array_length(char **ts) {
    size_t n = 0;
    while (ts[n]) {
        n++;
    }
    return n;
}

char **esh_tokenize(char *input) {
    char **ts;
    char *delimiters = " \n";
    char *token = strtok(input, delimiters);

    if (token) {
        ts = malloc(sizeof(char *));
        ts[0] = strduplicate(token);
    }

    size_t n = 1;

    while ((token = strtok(NULL, delimiters))) {
        char **allocated = realloc(ts, (n + 1) * sizeof(char *));
        assert(allocated);

        ts = allocated;
        ts[n] = strduplicate(token);
        n++;
    }

    char **allocated = realloc(ts, (n + 1) * sizeof(char *));
    assert(allocated);
    ts = allocated;
    ts[n] = NULL;

    return ts;
}

void esh_array_print(char **ts) {
    printf("Printing tokens: \n\t");

    size_t i = 0;
    for (char *current = ts[i]; current; current = ts[++i]) {
        printf("%ld: [%s]\n\t", i, ts[i]);
    }

    printf("\n");
}

void esh_array_remove_last(char **ts) {
    size_t n = esh_array_length(ts);
    free(ts[n - 1]);
    ts[n - 1] = NULL;
}

void esh_array_free(char **t) {
    size_t i = 0;
    for (char *current = t[i]; current; current = t[++i]) {
        free(t[i]);
    }
    free(t);
}
