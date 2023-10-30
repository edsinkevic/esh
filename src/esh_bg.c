//
// Created by edvin on 23.10.30.
//

#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include "esh_bg.h"
#include "esh_array.h"
#include "esh_proc.h"

int esh_bg_handle(char **ts) {
    size_t n = esh_array_length(ts);
    int is_bg = n == 2 && strcmp(ts[0], "bg") == 0;
    if (!is_bg) {
        return 0;
    }
    int id = (int) strtol(ts[1], NULL, 10);
    esh_proc *p = esh_proc_get(id);
    if (!p) {
        fprintf(stderr, "No process with id %d\n", id);
        return 1;
    }

    if (kill(p->pid, SIGCONT) == -1) {
        fprintf(stderr, "Could not send signal %d: %s\n", p->pid, strerror(errno));
        return 1;
    }

    p->status = esh_proc_status(p);
    return 1;
}
