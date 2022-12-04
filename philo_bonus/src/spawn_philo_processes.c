#include "philosophers_bonus.h"

void	*death_checker(void *arg)
{
	t_death_checker_thread_data	*d;

	d = (t_death_checker_thread_data *)arg;
	while (TRUE)
	{
		// sem_wait(d->arg->sem->mutex);
		if (philo_time_is_greater_than_zero(philo_time_since(d->dat->time_to_die)) == TRUE)
		{
			print_line(d->arg->input->start_time, d->arg->id, "died");
			sem_post(d->arg->sem->stop);
			// exit(0); //should be able to remove this when the parent process kills the pids
		}
		// sem_post(d->arg->sem->mutex);
		usleep(100); // CHECK if this is needed
		// usleep(1000000);
	}
}

void	run_philo(t_philosopher_arg_data arg)
{
	t_philosopher_own_data		dat;
	pthread_t					thread;
	t_death_checker_thread_data	thread_data;
	int							ret;

	dat.times_to_eat = 0;
	if (arg.input->infinite_simulation == FALSE)
	{
		dat.times_to_eat = arg.input->number_of_times_each_philosopher_must_eat;
	}
	dat.time_to_die = philo_time_add(arg.input->start_time, arg.input->time_to_die);
	thread_data.arg = &arg;
	thread_data.dat = &dat;
	ret = pthread_create(&thread, NULL, death_checker, &thread_data);
	if (ret)
	{
		printf("ERROR: Failed to create death checking thread\n");
		sem_post(arg.sem->stop);
	}
	if (dat.times_to_eat > 0 || arg.input->infinite_simulation == TRUE)
	{
		philo_eat(arg, &dat);
	}
	while (dat.times_to_eat > 0 || arg.input->infinite_simulation == TRUE)
	{
		philo_sleep(arg);
		philo_think(arg);
		philo_eat(arg, &dat);
	}
}

void	spawn_philo_processes(t_main_data *m)
{
	int	i;
	int	pid;
	t_philosopher_arg_data	arg;

	i = 1;
	sem_wait(m->sem.mutex);
	// usleep(10000);
	while (i <= m->input.number_of_philosophers)
	{
		pid = fork();
		if (pid)
		{
			m->philo_pid[i] = pid;
		}
		else
		{
			arg.id = i;
			arg.input = &m->input;
			arg.sem = &m->sem;
			run_philo(arg);
			exit(0);
		}
		i++;
		usleep(100);
	}
	sem_post(m->sem.mutex);
}
