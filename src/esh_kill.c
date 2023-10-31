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
#include "esh_utils.h"

int determine_sig(char *sig);

int esh_kill_handle(char **ts) {
    size_t n = esh_array_length(ts);
    int is_kill = n == 3 && esh_streq(ts[0], "kill");

    if (!is_kill) {
        return 0;
    }

    int id = (int) strtol(ts[2], NULL, 10);

    esh_proc *p = esh_proc_get(id);

    if (!p) {
        fprintf(stderr, "No process with id %d\n", id);
        return 1;
    }

    int sig = determine_sig(ts[1]);

    if (sig == -1) {
        fprintf(stderr, "Invalid signal %s\n", ts[1]);
        return 1;
    }

    if (kill(p->pid, sig) == -1) {
        fprintf(stderr, "Could not send signal %d: %s\n", p->pid, strerror(errno));
    }

    return 1;
}

int determine_sig(char *sig) {
    if (esh_streq(sig, "stop")) {
        return SIGSTOP;
    }
    if (esh_streq(sig, "cont")) {
        return SIGCONT;
    }
    if (esh_streq(sig, "kill")) {
        return SIGKILL;
    }
    if (esh_streq(sig, "term")) {
        return SIGTERM;
    }

    return -1;
}
