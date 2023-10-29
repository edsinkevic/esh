#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "esh_tokens.h"
#include "esh_bg.h"

#define IO_BUFFER_SIZE 256

char IO_BUFFER[IO_BUFFER_SIZE] = {0};

int esh_exit(esh_tokens *ts) {
    return ts->n == 1 && strcmp(ts->tokens[0], "exit\n") == 0;
}

int main() {
    printf("Entering esh shell...\n\n");
    esh_tokens *tokens = NULL;

    while (1) {
        if (tokens) {
            esh_free_tokens(tokens);
            tokens = NULL;
        }

        printf("$/ ");


        errno = 0;
        if (!fgets(IO_BUFFER, IO_BUFFER_SIZE, stdin)) {
            if (errno) {
                fprintf(stderr, "Could not read input: %s\n", strerror(errno));
                exit(EXIT_FAILURE);
            }

            printf("Exiting...\n");
            exit(EXIT_SUCCESS);
        }

        tokens = esh_tokenize(IO_BUFFER);
        esh_print_tokens(tokens);

        if (esh_exit(tokens)) {
            break;
        }

        if (esh_bg_handle(tokens)) {
            continue;
        }
    }

    printf("Exiting...\n");
    exit(EXIT_SUCCESS);
}
