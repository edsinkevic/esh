//
// Created by edvin on 23.10.30.
//
#include <stddef.h>
#include <malloc.h>
#include <assert.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "esh_proc.h"

esh_proc *processes = NULL;
int current_index = 1;

char *ESH_PROC_ERRORS[6] = {"UNEXPECTED", "EXITED", "STOPPED", "NOT_FOUND", "TERMINATED", "RUNNING"};

int esh_proc_status_from_waitpid(pid_t result, int status) {
    if (result > 0) {
        if (WIFEXITED(status)) {
            return ESH_PROC_EXITED;
        } else if (WIFSTOPPED(status)) {
            return ESH_PROC_STOPPED;
        } else if (WIFSIGNALED(status)) {
            return ESH_PROC_TERMINATED;
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
    for (esh_proc *p = processes; p != NULL; p = p->next) {
        if (p->id == id) {
            return p;
        }
    }

    return NULL;
}

esh_proc *esh_proc_add(pid_t pid, char *cmd) {
    esh_proc *p = malloc(sizeof(esh_proc));
    assert(p);
    p->id = current_index++;
    p->pid = pid;
    p->cmd = cmd;
    p->next = processes;
    p->status = ESH_PROC_RUNNING;
    processes = p;
    return p;
}


void proc_refresh_statuses() {
    for (esh_proc *p = processes; p != NULL; p = p->next) {
        int status = esh_proc_status(p);
        if (status != ESH_PROC_NOT_FOUND) {
            p->status = status;
        }
    }
}

void esh_proc_print() {
    printf("Processes: \n");
    proc_refresh_statuses();
    for (esh_proc *p = processes; p != NULL; p = p->next) {
        printf("[%d] %d %s %s\n", p->id, p->pid, p->cmd, ESH_PROC_ERRORS[p->status]);
    }
}
