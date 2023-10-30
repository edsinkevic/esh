//
// Created by edvin on 23.10.29.
//

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include "esh_kill.h"
#include "esh_array.h"

int esh_kill_handle(char **ts) {
    size_t n = esh_array_length(ts);
    int is_kill = n == 3 && strcmp(ts[0], "kill") == 0 && strcmp(ts[1], "stop") == 0;

    if (!is_kill) {
        return 0;
    }

    int pid = (int) strtol(ts[2], NULL, 10);

    if (kill(pid, SIGSTOP) == -1) {
        fprintf(stderr, "Could not stop process %d: %s\n", pid, strerror(errno));
    }

    return 1;
}