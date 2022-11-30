#ifndef PHILOSOPHERS_BONUS_H

# define BOOL int
# define TRUE 1
# define FALSE 0

# include <stdio.h>
# include <sys/time.h>
# include <semaphore.h>

# include <stdlib.h>
# include <unistd.h>

typedef struct s_input_data
{
    int		number_of_philosophers;
	int		time_to_die;
	int		time_to_eat;
	int 	time_to_sleep;
	BOOL	infinite_simulation;
	int		number_of_times_each_philosopher_must_eat;
	struct timeval	start_time;
}   t_input_data;

typedef struct s_philosopher_data
{
    int id;
    int times_to_eat;
    struct timeval  time_to_die;
}   t_philosopher_data;

typedef struct s_semaphores
{
    sem_t   *mutex;
    sem_t   *chopsticks;
    sem_t   *full;
    sem_t   *died;
}   t_semaphores;

typedef struct s_main_data
{
    t_input_data    input;
    t_semaphores    sem;
    int             *philo_pid;
    
}   t_main_data;

#endif