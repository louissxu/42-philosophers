#include "philosophers_bonus.h"

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

int parse_input_args(int argc, char **argv, t_main_data *m)
{
	int ret;

    if (!(argc == 5 || argc == 6))
    {
        printf("Incorrect number of arguments. Needs 4 or 5 arguments.\n");
        printf("Usage: ./philo_bonus number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n");
		return (EXIT_FAILURE);
    }
	if (philo_simple_atoi(argv[2]) < 60 || philo_simple_atoi(argv[3]) < 60 || philo_simple_atoi(argv[4]) < 60)
	{
		printf("WARN: time_to_die, time_to_eat, and time_to_sleep are expected to be >= 60ms. Behaviour with values <60ms may be unreliable.\n");
	}

	// Input data setup
	m->input.number_of_philosophers = philo_simple_atoi(argv[1]);
	m->input.time_to_die = philo_time_create(0, philo_simple_atoi(argv[2]) * 1000);
	m->input.time_to_eat = philo_time_create(0, philo_simple_atoi(argv[3]) * 1000);
	m->input.time_to_sleep = philo_time_create(0, philo_simple_atoi(argv[4]) * 1000);
	m->input.infinite_simulation = TRUE;
	if (argc == 6)
	{
		m->input.infinite_simulation = FALSE;
		m->input.number_of_times_each_philosopher_must_eat = philo_simple_atoi(argv[5]);
	}
	ret = gettimeofday(&m->input.start_time, NULL);
	if (ret)
	{
		printf("ERROR: gettimeofday failed to get time of day, rc: %d\n", ret);
		return (EXIT_FAILURE);
	}
	return (0);
}

int	setup_main_data(t_main_data *m)
{
	m->philo_pid = malloc(sizeof (*m->philo_pid) * (m->input.number_of_philosophers + 1));
	if (!m->philo_pid)
	{
		printf("ERROR: Unable to malloc philo_pid.\n");
		return (EXIT_FAILURE);
	}
	return (0);
}