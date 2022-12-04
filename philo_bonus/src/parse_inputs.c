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
	return ((int)(result * post_mult));
}

void	insert_values_into_input_struct(int argc, char **argv, t_input_data *in)
{
	in->number_of_philosophers = philo_simple_atoi(argv[1]);
	in->time_to_die = philo_time_create(0, philo_simple_atoi(argv[2]) * 1000);
	in->time_to_eat = philo_time_create(0, philo_simple_atoi(argv[3]) * 1000);
	in->time_to_sleep = philo_time_create(0, philo_simple_atoi(argv[4]) * 1000);
	in->infinite_simulation = TRUE;
	if (argc == 6)
	{
		in->infinite_simulation = FALSE;
		in->number_of_times_each_philosopher_must_eat = \
			philo_simple_atoi(argv[5]);
	}
}

int	parse_input_args(int argc, char **argv, t_main_data *m)
{
	int	ret;

	if (!(argc == 5 || argc == 6))
	{
		print_incorrect_arg_count_error_message();
		return (EXIT_FAILURE);
	}
	if (philo_simple_atoi(argv[2]) < 60 || \
		philo_simple_atoi(argv[3]) < 60 || \
		philo_simple_atoi(argv[4]) < 60)
	{
		print_small_time_values_warn_message();
	}
	insert_values_into_input_struct(argc, argv, &m->input);
	ret = gettimeofday(&m->input.start_time, NULL);
	if (ret)
	{
		printf("ERROR: gettimeofday failed to get time of day, rc: %d\n", ret);
		return (EXIT_FAILURE);
	}
	return (0);
}

int	malloc_main_data(t_main_data *m)
{
	size_t	num_bytes;

	num_bytes = sizeof (*m->philo_pid) * (m->input.number_of_philosophers + 1);
	m->philo_pid = malloc(num_bytes);
	if (!m->philo_pid)
	{
		printf("ERROR: Unable to malloc philo_pid.\n");
		return (EXIT_FAILURE);
	}
	return (0);
}
