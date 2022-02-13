# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/02/13 15:41:49 by sakllam           #+#    #+#              #
#    Updated: 2022/02/13 15:44:33 by sakllam          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo
INCLUDE = Philosophers.h
SRC =	Philosophers_utils_1.c\
		Philosophers.c\
		atoi.c\
		parsing.c\
		Philosophers_utils_3.c\
		Philosophers_utils_2.c\
		ft_printf.c
CFLAGS = -Wall -Werror -Wextra
all: $(NAME)
$(NAME): $(INCLUDE) $(SRC:.c=.o)
	cc $(CFLAGS) $(SRC) -I $(INCLUDE) -o $(NAME)
clean:
	rm -f $(SRC:.c=.o)
fclean: clean
	rm -f $(NAME)
re: fclean all
.PHONY: all clean fclean re