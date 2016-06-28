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
	list_node_t *n, *p;
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
		if (n && n->value) {
			if(n->value->status == 1) {
				printf("Process %d terminated\n", n->value->pid);
				p = n->previous;
				del_node(jobs, n);
				n = p;
				if(!n) {
					break;
				}
			}
		}
	}
}

int run_bg(int pid, int pgid, char *name)
{
	int i;
	list_node_t *job = append_node(jobs);
	job->value = malloc(sizeof(job_t));
	job->value->pid = pid;
	job->value->pgid = pgid;
	job->value->status = 0;
	strcpy(job->value->name, name);
	return set_job_background(job);
}

/*int set_job_foreground(list_node_t *n)
{
	int aux, status;
	aux = tcsetpgrp(0, n->value->pgid);
	sysfail (aux < 0, -1);
	aux = kill(-n->value->pgid, SIGCONT);
	sysfail (aux < 0, -1);
	while(1)
	{
		aux = waitpid(-1, &status, WUNTRACED);
		if(set_job_status(aux, status) < 0)
			break;
	}
	sysfail (aux < 0, -1);
	aux = tcsetpgrp(0, getpgrp());
	sysfail (aux < 0, -1);
	return 0;
}*/
int set_job_foreground(list_node_t *n)
{
	int aux = kill(-n->value->pid, SIGCONT);
	sysfail(aux < 0, -1);
	tcsetpgrp(STDIN_FILENO, n->value->pgid);
	aux = waitpid(n->value->pid, NULL, 0);
	sysfail(aux < 0, -1);
	tcsetpgrp(STDIN_FILENO, getpgid(0));
	return aux;
}

int set_job_background(list_node_t *n)
{
	return kill(-n->value->pgid, SIGCONT);
}