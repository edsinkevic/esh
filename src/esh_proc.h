//
// Created by edvin on 23.10.29.
//

#ifndef ESH_ESH_PROC_H
#define ESH_ESH_PROC_H

#include <sys/types.h>
#include <stddef.h>

#define ESH_PROC_EXITED 1
#define ESH_PROC_STOPPED 2
#define ESH_PROC_NOT_FOUND 3
#define ESH_PROC_TERMINATED 4
#define ESH_PROC_RUNNING 5

typedef struct esh_proc {
    int id;
    pid_t pid;
    char *cmd;
    int status;
    struct esh_proc *next;
} esh_proc;

esh_proc *esh_proc_get(int id);

esh_proc *esh_proc_add(pid_t pid, char *cmd);

void esh_proc_print();

int esh_proc_status(esh_proc *p);

int esh_proc_status_from_waitpid(pid_t result, int status);

void esh_proc_refresh_statuses();

void esh_proc_refresh_status(esh_proc *p);

void esh_proc_free_and_terminate_all();

void esh_proc_sig_handler(int sig);

void esh_proc_fg_mode_enable(esh_proc *p);

void esh_proc_fg_mode_disable();

#endif //ESH_ESH_PROC_H
