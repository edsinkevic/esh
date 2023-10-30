//
// Created by edvin on 23.10.29.
//

#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "esh_fg.h"
#include "esh_array.h"

int esh_fg_handle(char **ts) {
    size_t n = esh_array_length(ts);
    int is_fg = n == 2 && strcmp(ts[0], "fg") == 0;

    if (!is_fg) {
        return 0;
    }

    int pid = (int) strtol(ts[1], NULL, 10);

    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);

    tcsetpgrp(STDIN_FILENO, pid);

    kill(pid, SIGCONT);

    int status;
    waitpid(pid, &status, WUNTRACED);

    tcsetpgrp(STDIN_FILENO, getpgrp());
    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);

    return 1;
}
