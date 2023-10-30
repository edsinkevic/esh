#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "esh_array.h"
#include "esh_control.h"
#include "esh_cmd.h"
#include "esh_cmd.h"
#include "esh_kill.h"
#include "esh_fg.h"
#include "esh_proc.h"

#define IO_BUFFER_SIZE 256

char IO_BUFFER[IO_BUFFER_SIZE] = {0};

int esh_exit(char **ts) {
    return ts[0] != NULL && strcmp(ts[0], "exit") == 0;
}

int main() {
    printf("Entering esh shell...\n\n");
    char **tokens = NULL;

    while (1) {
        if (tokens) {
            esh_array_free(tokens);
            tokens = NULL;
        }

        printf("$/ ");


        errno = 0;
        if (!fgets(IO_BUFFER, IO_BUFFER_SIZE, stdin)) {
            if (errno) {
                fprintf(stderr, "Could not read input: %s\n", strerror(errno));
                exit(EXIT_FAILURE);
            }
            break;
        }

        tokens = esh_tokenize(IO_BUFFER);

        if (esh_exit(tokens)) {
            break;
        }

        if (esh_control_handle(tokens)) {
            continue;
        }

        if (esh_kill_handle(tokens)) {
            continue;
        }

        if (esh_fg_handle(tokens)) {
            continue;
        }

        if (esh_array_length(tokens) == 1 && strcmp(tokens[0], "jobs") == 0) {
            esh_proc_print();
            continue;
        }

        esh_cmd_handle(tokens);
    }

    printf("Exiting...\n");
    exit(EXIT_SUCCESS);
}
