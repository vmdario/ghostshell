
#ifndef JOB_H
#define JOB_H
#include "list.h"

#define RCMD_MAXARGS 1024

struct _job_t
{
    int pid;
    int status; /* 0 - Stopped, 1 - Terminated */
    char name[RCMD_MAXARGS];
};

void update_jobs_status();

int run_bg(int pid, char *name);

int set_job_foreground(int pid);

int set_job_background(int pid);

#endif