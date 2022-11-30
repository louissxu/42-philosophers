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

int parse_input_args(int argc, char *argv, t_main_data *m)
{
	int ret;

    if (!(argc == 5 || argc == 6))
    {
        printf("Incorrect number of arguments. Needs 4 or 5 arguments.\n");
        printf("Usage: ./philo_bonus number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n");
    }

	// Input data setup
	m->input.number_of_philosophers = philo_simple_atoi(argv[1]);
	m->input.time_to_die = philo_simple_atoi(argv[2]) * 1000;
	m->input.time_to_eat = philo_simple_atoi(argv[3]) * 1000;
	m->input.time_to_sleep = philo_simple_atoi(argv[4]) * 1000;
	m->input.infinite_simulation = TRUE;
	if (argc == 6)
	{
		m->input.infinite_simulation = FALSE;
		m->input.number_of_times_each_philosopher_must_eat = philo_simple_atoi(argv[5]);
	}
	ret = gettimeofday(&m->input.start_time, NULL);
	if (ret)
	{
		printf("error: gettimeofday, rc: %d\n", rc);
		return (EXIT_FAILURE);
	}
	return (0);
}