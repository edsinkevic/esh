#include "esh_bg.h"
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

int esh_bg_handle(esh_tokens *ts) {
    pid_t childpid;
    if (ts->n > 1 && strcmp(ts->tokens[ts->n - 1], "&") == 0) {
        errno = 0;
        if ((childpid = fork()) == -1) {
            fprintf(stderr, "Could not fork: %s\n", strerror(errno));
            return 0;
        }

        if (childpid == 0) {
            ts->tokens[ts->n - 1] = NULL;
            execvp(ts->tokens[0], ts->tokens + 1);
            fprintf(stderr, "Could not execute: %s\n", strerror(errno));
            return 1;
        }

        if (childpid > 0) {
            printf("Started background process %d\n", childpid);
            return 1;
        }

        return 1;
    }

    return 0;
}
