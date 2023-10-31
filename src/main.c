#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "esh_array.h"
#include "esh_control.h"
#include "esh_cmd.h"
#include "esh_kill.h"
#include "esh_fg.h"
#include "esh_proc.h"
#include "esh_bg.h"
#include "esh_utils.h"

#define IO_BUFFER_SIZE 256

char IO_BUFFER[IO_BUFFER_SIZE] = {0};

int esh_exit_handle(char **ts) {
    return ts[0] != NULL && esh_streq(ts[0], "exit");
}

int esh_jobs_handle(char **ts) {
    if (ts[0] != NULL && esh_streq(ts[0], "jobs")) {
        esh_proc_print();
        return 1;
    }
    return 0;
}

int main() {
    printf("Entering esh shell...\n\n");
    char **tokens = NULL;

    while (1) {
        if (tokens) {
            esh_array_free(tokens);
            tokens = NULL;
        }

        printf("\n$/ ");

        errno = 0;
        if (!fgets(IO_BUFFER, IO_BUFFER_SIZE, stdin)) {
            if (errno) {
                fprintf(stderr, "Could not read input: %s\n", strerror(errno));
                exit(EXIT_FAILURE);
            }
            break;
        }

        tokens = esh_tokenize(IO_BUFFER);

        if (esh_exit_handle(tokens)) {
            break;
        }

        esh_control_handle(tokens)
        || esh_kill_handle(tokens)
        || esh_fg_handle(tokens)
        || esh_bg_handle(tokens)
        || esh_jobs_handle(tokens)
        || esh_cmd_handle(tokens);
    }


    if (tokens) {
        esh_array_free(tokens);
        tokens = NULL;
    }

    esh_proc_free_and_terminate_all();
    esh_proc_print();
    printf("Exiting...\n");
    exit(EXIT_SUCCESS);
}
