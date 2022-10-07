# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gmillon <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/02/16 17:07:37 by atrilles          #+#    #+#              #
#    Updated: 2022/10/07 21:09:09 by gmillon          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

######## PROJECT ###############################################################

NAME = minishell

######## DIRECTORIES ###########################################################

SRC_DIR	= src/
LIBFTDIR = ./libftextended
LIBFTINCLUDES = $(LIBFTDIR)/headers

######## SOURCE FILES ##########################################################

SRC_FILES =		main		functions 	paths  export_unset  cmd atoi  var\
				init		case_parse	do_command	data_utils	double_chevron\
				input_traversal	case_parse_2 var_substitution cmd_echo\


SRC = $(addsuffix .c, $(SRC_FILES))
OBJ = $(addsuffix .o, $(SRC_FILES))

######## FLAGS #################################################################

CC = gcc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = 

######## RULES #################################################################

all: $(NAME)

$(NAME): $(OBJ) $(LIBFTDIR)/libft.a
	# $(CC) $(CFLAGS) $(OBJ) -o $(NAME) 
	$(CC) -ggdb3 $(OBJ) -o $(NAME) -lreadline -lncurses libreadline.a $(LIBFTDIR)/libft.a

asan: $(OBJ) $(LIBFTDIR)/libft.a
	# $(CC) $(CFLAGS) $(OBJ) -o $(NAME) 
	$(CC) -fsanitize=address $(OBJ) -o $(NAME) -lreadline -lncurses libreadline.a $(LIBFTDIR)/libft.a

%.o: %.c
	# $(CC) $(CFLAGS) -c $< -o $@
	$(CC) -ggdb3 -c -I$(LIBFTINCLUDES) $< -o $@ -lreadline -L .brew/opt/readline/lib -I .brew/opt/readline/include 

$(LIBFTDIR)/libft.a:
	$(MAKE) -C $(LIBFTDIR)
read: read_test.o
	gcc read_test.o -o read -lreadline -L .brew/opt/readline/lib -I .brew/opt/readline/include 
.PHONY: clean fclean re

clean: 
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)
	rm -f $(LIBFTDIR)/libft.a
re:	fclean all
