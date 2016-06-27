#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <debug.h>
#include "job.h"

extern list_t *jobs;

int set_job_status(int pid, int status)
{
	list_node_t *n;
	if (pid > 0)
	{
		for (n = jobs->first; n; n = n->next)
		{
			if (n->value->pid == pid)
			{
				if (WIFSTOPPED (status))
					n->value->status = 0; /* stopped */
				else
					n->value->status = 1; /* terminated */
				return 0; /* success*/
			}
		}
	}
	else
	{
		return -1; /* error */
	}
	return -1;
}

void update_jobs_status()
{
	int status, pid;
	list_node_t *n;
	for(;;)
	{
		/* check all background jobs status to delete from list */
		pid = waitpid(-1, &status, WUNTRACED|WNOHANG);
		/* update job status here */
		if( set_job_status(pid, status) < 0) {
			break; /* no background jobs */
		}
	}
	/* delete all jobs terminated */
	for (n = jobs->first; n; n = n->next)
	{
		if (n->value->status == 1)
		{
			printf("Process %d terminated\n", n->value->pid);
			del_node(jobs, n);
		}
	}
}

int run_bg(int pid, char *name)
{
	int i;
	list_node_t *job = append_node(jobs);
	job->value = malloc(sizeof(job_t));
	job->value->pid = pid;
	job->value->status = 0;
	strcpy(job->value->name, name);
	return 0;
}

int set_job_foreground(int pid)
{
	return 0;
}

int set_job_background(int pid)
{
	return 0;
}