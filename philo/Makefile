CC:=gcc
CFLAGS:= -Wall -Werror -Wextra -pthread -g3 -fsanitize=address
RM:=rm -rf

NAME:=philo

SRC_PATH:=./

SRC_FILES:= main.c  philo_init.c  philo_routine.c\
	philo_utils.c  utils.c  shared_resources.c \
	change_states.c  eat_sleep_think.c forks.c

SRC_FILES:= $(addprefix $(SRC_PATH)/,$(SRC_FILES))
SRC_OBJECTS:= $(patsubst %.c,%.o,$(SRC_FILES))

all: $(NAME)

$(NAME): $(SRC_OBJECTS)
	@echo Compiling main bin
	@$(CC) $^ $(CFLAGS) -o $@

clean:
	@$(RM) $(SRC_OBJECTS)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re philo