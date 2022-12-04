NAME = philo_bonus

CC = gcc
CFLAGS = -Wall -Wextra -Werror

INCLUDES = INCLUDES

SRCS_RAW = \
	parse_input_args.c \
	setup.c \
	spawn_philo_processes.c \
	philo_time.c \
	print.c \
	actions.c \
	sleep.c \
	clean_up.c \
	philosophers_bonus.c

SRC_DIR = src
SRCS = $(addprefix $(SRC_DIR)/,$(SRCS_RAW))

OBJS_RAW = $(SRCS_RAW:.c=.o)
OBJ_DIR = obj
OBJS = $(addprefix $(OBJ_DIR)/,$(OBJS_RAW))

all: $(NAME)
	@echo $(SRCS)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -I $(INCLUDES) $(OBJS) -o $@

$(OBJ_DIR)/%.o:$(SRC_DIR)/%.c
	@mkdir -p $(@D)
	@echo "Making $<"
	$(CC) $(CFLAGS) -I $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)
	rm -df obj

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all, clean, fclean, re