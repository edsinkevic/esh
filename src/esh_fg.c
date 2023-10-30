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


void esh_handler(int sig) {
    switch (sig) {
        case SIGSTOP:
            if (kill(ESH_PROC_FG->pid, SIGSTOP) == -1) {
                fprintf(stderr, "Could not send signal %d: %s\n", ESH_PROC_FG->pid, strerror(errno));
            }
            break;
    }
}

int esh_fg_handle(char **ts) {
    size_t n = esh_array_length(ts);
    int is_fg = n == 2 && strcmp(ts[0], "fg") == 0;

    if (!is_fg) {
        return 0;
    }

    int id = (int) strtol(ts[1], NULL, 10);

    esh_proc *proc = esh_proc_get(id);

    if (!proc) {
        fprintf(stderr, "No process with id %d\n", id);
        return 1;
    }

    int pid = proc->pid;

    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);

    signal(SIGSTOP, esh_handler);
    ESH_PROC_FG = proc;

    tcsetpgrp(STDIN_FILENO, pid);

    int status;
    int result = waitpid(pid, &status, WUNTRACED);

    tcsetpgrp(STDIN_FILENO, getpgrp());
    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);
    signal(SIGSTOP, SIG_DFL);

    if (WIFSTOPPED(status)) {
        printf("[%d] suspended\n", pid);
    }

    ESH_PROC_FG->status = esh_proc_status_from_waitpid(result, status);

    ESH_PROC_FG = NULL;

    return 1;
}
