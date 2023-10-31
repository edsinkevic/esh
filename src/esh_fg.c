//
// Created by edvin on 23.10.29.
//

#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdio.h>
#include <errno.h>
#include "esh_fg.h"
#include "esh_array.h"
#include "esh_proc.h"
#include "esh_utils.h"


int esh_fg_handle(char **ts) {
    size_t n = esh_array_length(ts);
    int is_fg = n == 2 && esh_streq(ts[0], "fg");

    if (!is_fg) {
        return 0;
    }

    int id = (int) strtol(ts[1], NULL, 10);

    esh_proc *proc = esh_proc_get(id);

    if (!proc) {
        fprintf(stderr, "No process with id %d\n", id);
        return 1;
    }

    if (proc->status != ESH_PROC_RUNNING) {
        fprintf(stderr, "%d process is not running\n", id);
        return 1;
    }

    int pid = proc->pid;

    esh_proc_fg_mode_enable(proc);

    int status;
    int result = waitpid(pid, &status, WUNTRACED);

    if (WIFSTOPPED(status)) {
        printf("[%d] suspended\n", pid);
    }

    proc->status = esh_proc_status_from_waitpid(result, status);

    esh_proc_fg_mode_disable();

    return 1;
}
