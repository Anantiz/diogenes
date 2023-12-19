# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aurban <aurban@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/08 01:00:11 by aurban            #+#    #+#              #
#    Updated: 2023/12/19 19:07:52 by aurban           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC:=gcc
CFLAGS:= -Wall -Werror -Wextra -pthread #-g3 -fsanitize=address
RM:=rm -rf

NAME:=philo
BNS_NAME:=philo_bonus

SRC_PATH:=./src/philo
BNS_PATH:=./src/philo_bonus

SRC_FILES:= main.c  philo_init.c  philo_routine.c\
	philo_utils.c  utils.c  shared_resources.c \
	change_states.c  eat_sleep_think.c forks.c

SRC_FILES:= $(addprefix $(SRC_PATH)/,$(SRC_FILES))
SRC_OBJECTS:= $(patsubst %.c,%.o,$(SRC_FILES))

BNS_FILES:= main_bonus.c change_states_bonus.c eat_sleep_bonus.c\
	philo_init_bonus.c philo_routine_bonus.c philo_utils_bonus.c\
	shared_resources_bonus.c utils_bonus.c

BNS_FILES:= $(addprefix $(BNS_PATH)/,$(BNS_FILES))
BNS_OBJECTS:= $(patsubst %.c,%.o,$(BNS_FILES))


all: $(NAME) bonus

$(NAME): $(SRC_OBJECTS)
	@echo Compiling main bin
	@$(CC) $^ $(CFLAGS) -o $@

bonus: $(BNS_OBJECTS)
	@echo Compiling bonus bin
	@$(CC) $^ $(CFLAGS) -o $(BNS_NAME)

clean:
	@$(RM) $(SRC_OBJECTS) $(BONUS_OBJ)

fclean: clean
	@$(RM) $(NAME) $(BNS_NAME)

re: fclean all

.PHONY: all clean fclean re bonus philo