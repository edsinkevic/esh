//
// Created by edvin on 23.10.30.
//
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stddef.h>
#include <assert.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "esh_proc.h"

esh_proc *PROCESSES = NULL;
int CURRENT_INDEX = 1;
char *ESH_PROC_ERRORS[6] = {"UNEXPECTED", "EXITED", "STOPPED", "NOT_FOUND", "TERMINATED", "RUNNING"};
static esh_proc *ESH_PROC_FG = NULL;

int esh_proc_status_from_waitpid(pid_t result, int status) {
    if (result > 0) {
        if (WIFEXITED(status)) {
            return ESH_PROC_EXITED;
        } else if (WIFSTOPPED(status)) {
            return ESH_PROC_STOPPED;
        } else if (WIFSIGNALED(status)) {
            return ESH_PROC_TERMINATED;
        } else if (WIFCONTINUED(status)) {
            return ESH_PROC_RUNNING;
        }
    }

    return ESH_PROC_NOT_FOUND;
}

int esh_proc_status(esh_proc *p) {
    int status = 0;
    pid_t result = waitpid(p->pid, &status, WNOHANG | WUNTRACED | WCONTINUED);


    return esh_proc_status_from_waitpid(result, status);
}

esh_proc *esh_proc_get(int id) {
    for (esh_proc *p = PROCESSES; p != NULL; p = p->next) {
        if (p->id == id) {
            esh_proc_refresh_status(p);
            return p;
        }
    }

    return NULL;
}

esh_proc *esh_proc_add(pid_t pid, char *cmd) {
    esh_proc *p = malloc(sizeof(esh_proc));
    assert(p);
    p->id = CURRENT_INDEX++;
    p->pid = pid;
    p->cmd = cmd;
    p->next = PROCESSES;
    p->status = ESH_PROC_RUNNING;
    PROCESSES = p;
    return p;
}

void esh_proc_refresh_status(esh_proc *p) {
    int status = esh_proc_status(p);
    if (status != ESH_PROC_NOT_FOUND) {
        p->status = status;
    }
}

void esh_proc_refresh_statuses() {
    for (esh_proc *p = PROCESSES; p != NULL; p = p->next) {
        esh_proc_refresh_status(p);
    }
}

void esh_proc_print() {
    if (PROCESSES != NULL) {
        printf("Processes: \n");
    }

    esh_proc_refresh_statuses();
    for (esh_proc *p = PROCESSES; p != NULL; p = p->next) {
        printf("[%d] %d %s %s\n", p->id, p->pid, p->cmd, ESH_PROC_ERRORS[p->status]);
    }
}


void esh_proc_free_and_terminate(esh_proc *p) {
    if (p == NULL) {
        return;
    }

    esh_proc_free_and_terminate(p->next);
    p->next = NULL;
    kill(p->pid, SIGTERM);
    kill(p->pid, SIGKILL);
    free(p->cmd);
    p->cmd = NULL;
    free(p);
}

void esh_proc_free_and_terminate_all() {
    esh_proc_free_and_terminate(PROCESSES);
    PROCESSES = NULL;
}

void esh_proc_sig_handler(int sig) {
    if (ESH_PROC_FG == NULL) {
        return;
    }

    pid_t fg_pid = ESH_PROC_FG->pid;

    if (kill(fg_pid, sig) == -1) {
        fprintf(stderr, "Could not send signal %d: %s\n", fg_pid, strerror(errno));
    }
}

void esh_proc_fg_mode_enable(esh_proc *p) {
    ESH_PROC_FG = p;
    signal(SIGTSTP, esh_proc_sig_handler);
    signal(SIGINT, esh_proc_sig_handler);
}

void esh_proc_fg_mode_disable() {
    signal(SIGTSTP, SIG_DFL);
    signal(SIGINT, SIG_DFL);
    ESH_PROC_FG = NULL;
}
