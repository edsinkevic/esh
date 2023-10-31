#include <errno.h>
#include "esh_cmd.h"
#include "esh_proc.h"
#include "esh_utils.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

int esh_cmd_handle(char **ts) {
    int childpid = fork();
    if (childpid == 0) {
        execvp(ts[0], ts);
        fprintf(stderr, "Could not execute: %s\n", strerror(errno));
        return 1;
    }

    int status;
    esh_proc *proc = esh_proc_add(childpid, esh_strdup(ts[0]));
    esh_proc_fg_mode_enable(proc);
    int result = waitpid(childpid, &status, WUNTRACED);

    esh_proc_fg_mode_disable();

    if (WIFSTOPPED(status)) {
        printf("[%d] suspended\n", childpid);
    }

    proc->status = esh_proc_status_from_waitpid(result, status);

    return 1;
}