#include "esh_control.h"
#include "esh_proc.h"
#include "esh_utils.h"
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>

int esh_control_handle(char **ts) {
    pid_t childpid;
    size_t n = esh_array_length(ts);
    int should_handle = n > 1 && esh_streq(ts[n - 1], "&");

    if (!should_handle) {
        return 0;
    }

    errno = 0;
    if ((childpid = fork()) == -1) {
        fprintf(stderr, "Could not fork: %s\n", strerror(errno));
        return 0;
    }

    if (childpid == 0) {
        esh_array_remove_last(ts);
        errno = 0;
        execvp(ts[0], ts);
        fprintf(stderr, "Could not execute: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (childpid > 0) {
        esh_proc *p = esh_proc_add(childpid, esh_strdup(ts[0]));
        assert(p);
        printf("Started background process %d with id %d\n", childpid, p->id);
        return 1;
    }

    return 1;
}
