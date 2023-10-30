//
// Created by edvin on 23.10.29.
//

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include "esh_kill.h"
#include "esh_array.h"
#include "esh_proc.h"

int esh_kill_handle(char **ts) {
    size_t n = esh_array_length(ts);
    int is_kill = n == 3 && strcmp(ts[0], "kill") == 0 && strcmp(ts[1], "stop") == 0;

    if (!is_kill) {
        return 0;
    }

    int id = (int) strtol(ts[2], NULL, 10);

    esh_proc *p = esh_proc_get(id);

    if (!p) {
        fprintf(stderr, "No process with id %d\n", id);
        return 1;
    }

    if (kill(p->pid, SIGSTOP) == -1) {
        fprintf(stderr, "Could not send signal %d: %s\n", p->pid, strerror(errno));
    }

    return 1;
}