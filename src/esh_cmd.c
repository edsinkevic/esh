#include <errno.h>
#include "esh_cmd.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

int esh_cmd_handle(char **ts) {
    int childpid = fork();
    if (childpid == 0) {
        execvp(ts[0], ts);
        fprintf(stderr, "Could not execute: %s\n", strerror(errno));
    } else {
        int status;
        waitpid(childpid, &status, WUNTRACED);
    }

    return 1;
}