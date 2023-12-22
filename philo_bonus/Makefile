# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aurban <aurban@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/08 01:00:11 by aurban            #+#    #+#              #
#    Updated: 2023/12/22 17:55:50 by aurban           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC:=gcc
CFLAGS:= -Wall -Werror -Wextra -pthread #-g3 -fsanitize=address
RM:=rm -rf

NAME:=philo_bonus

BNS_PATH:=./

BNS_FILES:= main_bonus.c change_states_bonus.c eat_sleep_bonus.c\
	philo_init_bonus.c philo_routine_bonus.c philo_utils_bonus.c\
	shared_resources_bonus.c utils_bonus.c

BNS_FILES:= $(addprefix $(BNS_PATH)/,$(BNS_FILES))
BNS_OBJECTS:= $(patsubst %.c,%.o,$(BNS_FILES))

all: $(NAME)

$(NAME): $(BNS_OBJECTS)
	@echo Compiling main bin
	@$(CC) $^ $(CFLAGS) -o $@

clean:
	@$(RM) $(BNS_OBJECTS)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus philo