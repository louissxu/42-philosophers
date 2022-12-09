#include "philosophers.h"

void	free_all(t_main_data *m)
{
	if (m->waiter.fork_in_use)
	{
		free(m->waiter.fork_in_use);
	}
	if (m->waiter.fork_locks)
	{
		free(m->waiter.fork_locks);
	}
	if (m->threads)
	{
		free(m->threads);
	}
	if (m->thread_data)
	{
		free(m->thread_data);
	}
	if (m->philosophers)
	{
		free(m->philosophers);
	}
}