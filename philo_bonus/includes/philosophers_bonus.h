#ifndef PHILOSOPHERS_BONUS_H

# define BOOL int
# define TRUE 1
# define FALSE 0

# define SEM_MUTEX "sem_mutex"
# define SEM_CHOPSTICKS "sem_chopsticks"
# define SEM_FULL "sem_full"
# define SEM_STOP "sem_stop"
# define SEM_FIRST_TO_DIE "sem_first_to_die"

# include <stdio.h>
# include <sys/time.h>
# include <semaphore.h>
# include <pthread.h>
# include <signal.h>

# include <stdlib.h>
# include <unistd.h>

typedef struct s_input_data
{
	int				number_of_philosophers;
	struct timeval	time_to_die;
	struct timeval	time_to_eat;
	struct timeval	time_to_sleep;
	BOOL			infinite_simulation;
	int				number_of_times_each_philosopher_must_eat;
	struct timeval	start_time;
}	t_input_data;

typedef struct s_semaphores
{
	sem_t	*mutex;
	sem_t	*chopsticks;
	sem_t	*full;
	sem_t	*stop;
	sem_t	*first_to_die;
}	t_semaphores;

typedef struct s_philosopher_arg_data
{
	int				id;
	t_input_data	*input;
	t_semaphores	*sem;
}	t_philosopher_arg_data;

typedef struct s_philosopher_own_data
{
	int				times_to_eat;
	struct timeval	time_to_die;
}	t_philosopher_own_data;

typedef struct s_death_checker_thread_data
{
	t_philosopher_arg_data	*arg;
	t_philosopher_own_data	*dat;
}	t_death_checker_thread_data;

typedef struct s_main_data
{
	t_input_data	input;
	t_semaphores	sem;
	int				*philo_pid;
}	t_main_data;

void			philo_eat(t_philosopher_arg_data arg, \
	t_philosopher_own_data *dat);
void			philo_sleep(t_philosopher_arg_data arg);
void			philo_think(t_philosopher_arg_data arg);

int				philo_simple_atoi(const char *str);
int				parse_input_args(int argc, char **argv, t_main_data *m);
int				malloc_main_data(t_main_data *m);

void			setup_semaphores(t_main_data *m);
void			clean_up_semaphores(void);

int				setup_state_checks(t_main_data *m);

void			spawn_philo_processes(t_main_data *m);

struct timeval	philo_time_create(time_t sec, long int usec);
struct timeval	philo_time_now_safe(int *err);
struct timeval	philo_time_now(void);
struct timeval	philo_time_normalize(struct timeval t);
struct timeval	philo_time_add(struct timeval t0, struct timeval t1);
struct timeval	philo_time_diff(struct timeval t0, struct timeval t1);
struct timeval	philo_time_since(struct timeval t);
long int		philo_time_to_ms(struct timeval t);
long int		philo_ms_since(struct timeval t);
BOOL			philo_time_is_greater_than_zero(struct timeval t);



void	ft_timeval_sleep(struct timeval t);

void	print_line(struct timeval start_time, int philo_name, char *str);
void	print_incorrect_arg_count_error_message(void);
void	print_small_time_values_warn_message(void);


void	free_mallocs(t_main_data *m);

#endif