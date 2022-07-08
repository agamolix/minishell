# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gmillon <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/02/16 17:07:37 by atrilles          #+#    #+#              #
#    Updated: 2022/07/08 21:48:24 by gmillon          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

######## PROJECT ###############################################################

NAME = minishell

######## DIRECTORIES ###########################################################

SRC_DIR	= src/

######## SOURCE FILES ##########################################################

SRC_FILES =		main		functions 	paths  export_unset  cmd atoi  var\
				init		case_parse	do_command

SRC = $(addsuffix .c, $(SRC_FILES))
OBJ = $(addsuffix .o, $(SRC_FILES))

######## FLAGS #################################################################

CC = gcc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = 

######## RULES #################################################################

all: $(NAME)

$(NAME): $(OBJ)
	# $(CC) $(CFLAGS) $(OBJ) -o $(NAME) 
	$(CC) -ggdb3 $(OBJ) -o $(NAME) -lreadline

%.o: %.c
	# $(CC) $(CFLAGS) -c $< -o $@
	$(CC) -ggdb3 -lreadline -c $< -o $@

.PHONY: clean fclean re

clean: 
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re:	fclean all
