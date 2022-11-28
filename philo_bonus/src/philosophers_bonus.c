# include "philosophers_bonus.h"

typedef struct s_input_data
{
	int		number_of_philosophers;
	int		time_to_die;
	int		time_to_eat;
	int 	time_to_sleep;
	BOOL	infinite_simulation;
	int		number_of_times_each_philosopher_must_eat;
	struct timeval	start_time;
}	t_input_data;

// Forks renamed to chopsticks to avoid confusion because this program is going to use fork to generate new processes
typedef struct s_common_data
{
	sem_t	*sem_mutex;
	sem_t	*sem_chopsticks;
	int		chopstic_count;
	int		*process_pid;
	int		number_of_full_philosophers;
}   t_common_data;

typedef struct s_philo_data
{
	int id;
	int times_eaten;
	struct timeval dies_at;
}	t_philo_data;

typedef struct s_process_data
{
	t_input_data	*input_data;
	t_common_data	*com;
	t_philo_data	*philo;
}	t_process_data;

typedef struct s_main_data
{
    t_input_data    input_data;
    t_common_data	com;
	t_philo_data	*philo_data;
	t_process_data	*process_data;
}   t_main_data;


int	philo_simple_atoi(const char *str)
{
	long int	result;
	int			post_mult;
	size_t		i;

	result = 0;
	post_mult = 1;
	i = 0;
	if (str[i] == '-')
	{
		post_mult = -1;
		i++;
	}
	while (str[i] >= '0' & str[i] <= '9')
	{
		result *= 10;
		result += str[i] - '0';
		i++;
	}
	return (int)(result * post_mult);
}

long int	time_delta(struct timeval initial, struct timeval final)
{
	long int	delta_ms;

	delta_ms = (final.tv_sec - initial.tv_sec) * 1000000 + (final.tv_usec - initial.tv_usec);
	return (delta_ms);
}

long int	time_since(struct timeval t)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return (time_delta(t, current_time));
}

struct timeval	time_plus_delta(struct timeval initial, long int sec, long int ms, long int us)
{
	initial.tv_usec += us;
	initial.tv_usec += (ms * 1000);
	initial.tv_sec += sec;
	initial.tv_sec += initial.tv_usec / 1000000;
	initial.tv_usec = initial.tv_usec % 1000000;
	return (initial);
}

struct timeval	current_time_plus_delta(long int usec)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	t = time_plus_delta(t, 0, 0, usec);
	return (t);
}

void	print_line(struct timeval start_time, int philo_name, char *str)
{
	printf("%5ld %i %s\n", time_since(start_time) / 1000, philo_name, str);
}


// void	semaphore_init(sem_t *sem, int value)
// {
// 	sem = sem_open("test", 0, value);
// }

//Ref: https://greenteapress.com/thinkos/html/thinkos013.html
void	semaphore_wait(sem_t *sem)
{
	int	ret;

	ret = sem_wait(sem);
	if (ret)
	{
		// Handle error?
		// perror_exit("sem_init failed");
	}
}

void	semaphore_signal(sem_t *sem)
{
	int ret;

	ret = sem_post(sem);
	if (ret)
	{
		// Handle error?
	}
}

int	philo(t_process_data *p)
{
	int	pid;
	pid = fork();
	if (pid)
	{
		return (pid);
	}
	else
	{
		while (p->philo->times_eaten < p->input_data->number_of_times_each_philosopher_must_eat || \
			p->input_data->infinite_simulation == TRUE)
		{
			printf("debug %i thread starting\n", p->philo->id);
			semaphore_wait(p->com->sem_mutex);
			semaphore_wait(p->com->sem_chopsticks);
			printf("thread continuing\n");
			print_line(p->input_data->start_time, p->philo->id, "took a fork");
			print_line(p->input_data->start_time, p->philo->id, "took a fork");
			print_line(p->input_data->start_time, p->philo->id, "is eating");
			p->philo->times_eaten++;
			p->philo->dies_at = current_time_plus_delta(p->input_data->time_to_die);
			semaphore_signal(p->com->sem_mutex);

			usleep(p->input_data->time_to_eat);
			semaphore_signal(p->com->sem_chopsticks);
			semaphore_wait(p->com->sem_mutex);
			if (p->philo->times_eaten >= p->input_data->number_of_times_each_philosopher_must_eat)
			{
				p->com->number_of_full_philosophers++;
				p->com->process_pid[p->philo->id] = 0;
				semaphore_signal(p->com->sem_mutex);
				printf("exiting thread!\n");
				exit(0);
			}
			print_line(p->input_data->start_time, p->philo->id, "is sleeping");
			semaphore_signal(p->com->sem_mutex);

			usleep(p->input_data->time_to_sleep);
			semaphore_wait(p->com->sem_mutex);
			print_line(p->input_data->start_time, p->philo->id, "is thinking");
			semaphore_signal(p->com->sem_mutex);
		}
		exit(0);
	}
}

int	main(int argc, char **argv)
{
	t_main_data		m;
	int				i;
	int				rc;
	int				pid;
	int				j;

	printf("starting\n");
	if (argc < 5 || argc > 6)
	{
		printf("Error: Number of args needs to be 4 or 5\n");
		return (EXIT_SUCCESS);
	}

	// Input data setup
	m.input_data.number_of_philosophers = philo_simple_atoi(argv[1]);
	m.input_data.time_to_die = philo_simple_atoi(argv[2]) * 1000;
	m.input_data.time_to_eat = philo_simple_atoi(argv[3]) * 1000;
	m.input_data.time_to_sleep = philo_simple_atoi(argv[4]) * 1000;
	m.input_data.infinite_simulation = TRUE;
	if (argc == 6)
	{
		m.input_data.infinite_simulation = FALSE;
		m.input_data.number_of_times_each_philosopher_must_eat = philo_simple_atoi(argv[5]);
	}
	if ((rc = gettimeofday(&m.input_data.start_time, NULL)))
	{
		printf("error: gettimeofday, rc: %d\n", rc);
		return (EXIT_FAILURE);
	}

	// Setup main semaphore
	// Ref: https://www.cs.auckland.ac.nz/references/unix/digital/APS33DTE/DOCU_010.HTM
	int oflag = O_CREAT;
	// oflag = O_EXCL;
	mode_t mode = 0644;

	printf("clearing old semaphores\n");
	m.com.sem_mutex = sem_open("a", oflag, mode, 1);
	m.com.sem_chopsticks = sem_open("b", oflag, mode, 1);
	printf("closing sem mutex %i\n", sem_close(m.com.sem_mutex));
	printf("closing sem chopstick %i\n", sem_close(m.com.sem_chopsticks));


	printf("creating mutexes\n");
	// m.com.sem_mutex = sem_open("/tmp/main-mutex", oflag, mode, 1);
	m.com.sem_mutex = sem_open("c", oflag, mode, 1);
	printf("starting2\n");


	semaphore_wait(m.com.sem_mutex);
	semaphore_signal(m.com.sem_mutex);
	printf("setting number of chopstick pairs to be %i\n", m.input_data.number_of_philosophers / 2);
	// m.com.sem_chopsticks = sem_open("/tmp/main-chopsticks", oflag, mode, m.input_data.number_of_philosophers / 2);
	m.com.sem_chopsticks = sem_open("d", oflag, mode, m.input_data.number_of_philosophers / 2);



	semaphore_wait(m.com.sem_chopsticks);
	semaphore_signal(m.com.sem_chopsticks);
	printf("semaphores created and have at least one open\n");
	m.com.number_of_full_philosophers = 0;

	m.philo_data = malloc(sizeof (*m.philo_data) * (m.input_data.number_of_philosophers + 1));
	i = 1;
	while (i <= m.input_data.number_of_philosophers)
	{
		m.philo_data[i].id = i;
		m.philo_data[i].times_eaten = 0;
		m.philo_data[i].dies_at = time_plus_delta(m.input_data.start_time, 0, 0, m.input_data.time_to_die);
		i++;
	}

	m.process_data = malloc(sizeof (*m.process_data) * (m.input_data.number_of_philosophers + 1));
	i = 1;
	while (i <= m.input_data.number_of_philosophers)
	{
		m.process_data[i].input_data = &m.input_data;
		m.process_data[i].com = &m.com;
		m.process_data[i].philo = &m.philo_data[i];
		i++;
	}

    // spawn workers
	m.com.process_pid = malloc(sizeof (*m.com.process_pid) * (m.input_data.number_of_philosophers + 1));
	printf("herbehersers\n");
	semaphore_wait(m.com.sem_mutex);
	i = 1;
	printf("blasefhasefjlsekfj\n");
	while (i <= m.input_data.number_of_philosophers)
	{
		pid = philo(&m.process_data[i]);
		printf("spawning process. pid is: %i\n", pid);
		m.com.process_pid[i] = pid;
		i++;
	}
	semaphore_signal(m.com.sem_mutex);

	printf("getting here\n");
	while (TRUE)
	{
		semaphore_wait(m.com.sem_mutex);
		rc = 0;
		i = 1;
		printf("stuff\n");
		while (i <= m.input_data.number_of_philosophers)
		{
			// printf("the time delta is %ld\n", time_since(m.philo_data[i].dies_at));
			if (time_since(m.philo_data[i].dies_at) > 0)
			{
				// semaphore_wait(m.com.sem_mutex);
				printf("doing the inside\n");
				print_line(m.input_data.start_time, m.philo_data[i].id, "died");
				j = 1;
				while (j <= m.input_data.number_of_philosophers)
				{
					if (m.com.process_pid[j] > 0)
					{
						kill(m.com.process_pid[j], SIGTERM);
						m.com.process_pid[j] = 0;
					}
					j++;
				}
				// semaphore_signal(m.com.sem_mutex);
				rc = 1;
				break ;
			}
		}
		printf("stuff2\n");
		if (rc)
		{
			break ;
		}
		if (m.com.number_of_full_philosophers >= m.input_data.number_of_philosophers)
		{
			// semaphore_wait(m.com.sem_mutex);
			j = 1;
			while (j <= m.input_data.number_of_philosophers)
			{
				if (m.com.process_pid[j] > 0)
				{
					kill(m.com.process_pid[j], SIGTERM);
					m.com.process_pid[j] = 0;
				}
				j++;
			}
			// semaphore_signal(m.com.sem_mutex);
			break ;
		}
		semaphore_signal(m.com.sem_mutex);
	}
	printf("exiting main\n");
    // spinning checker function
	return (EXIT_SUCCESS);

	// free stuff
}
