# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aurban <aurban@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/08 01:00:11 by aurban            #+#    #+#              #
#    Updated: 2023/12/08 01:34:35 by aurban           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC:=gcc
CFLAGS:=-Wall -Wextra -fsanitize=address -pthread #-Werror
RM:=rm -rf

NAME:=philo
BNS_NAME:=philo_bonus

SRC_PATH:=./src/philo
BNS_PATH:=./src/philo_bonus

SRC_FILES:= main.c  philosopher.c  state_logs.c  utils.c  core_handler.c

SRC_FILES:= $(addprefix $(SRC_PATH)/,$(SRC_FILES))
SRC_OBJECTS:= $(patsubst %.c,%.o,$(SRC_FILES))

BNS_FILES:= main_bonus.c

BNS_FILES:= $(addprefix $(BNS_PATH)/,$(BNS_FILES))
BNS_OBJECTS:= $(patsubst %.c,%.o,$(BNS_FILES))


all: $(NAME) bonus

$(NAME): $(SRC_OBJECTS)
	$(CC) $^ $(CFLAGS) -o $@

bonus: $(BONUS_OBJ)
	@$(CC) $^ $(CFLAGS) -o $(BNS_NAME)

clean:
	@$(RM) $(SRC_OBJECTS) $(BONUS_OBJ)

fclean: clean
	@$(RM) $(NAME) $(BNS_NAME)

re: fclean all

.PHONY: all clean fclean re bonus philo