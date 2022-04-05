# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/02/13 15:36:33 by sakllam           #+#    #+#              #
#    Updated: 2022/02/13 15:40:37 by sakllam          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo_bonus
INCLUDE = Philosophers.h
SRC =	Philo_utils_3.c\
		Philosophers.c\
		atoi.c\
		parsing.c\
		Philo_utils_1.c\
		Philo_utils_2.c\
		Philo_utils_4.c\
		ft_printf.c
CFLAGS = -Wall -Werror -Wextra -lpthread
all: $(NAME)
$(NAME): $(INCLUDE) $(SRC:.c=.o)
	cc $(CFLAGS) $(SRC) -L $(INCLUDE) -o $(NAME)
clean:
	rm -f $(SRC:.c=.o)
fclean: clean
	rm -f $(NAME)
re: fclean all
.PHONY: all clean fclean re