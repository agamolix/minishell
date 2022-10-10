# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gmillon <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/02/16 17:07:37 by atrilles          #+#    #+#              #
#    Updated: 2022/10/11 00:25:01 by gmillon          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

######## PROJECT ###############################################################

NAME = minishell

######## DIRECTORIES ###########################################################

SRC_DIR	= src/
LIBFTDIR = ./libftextended
LIBFTINCLUDES = $(LIBFTDIR)/headers

######## SOURCE FILES ##########################################################

SRC_FILES =		main		functions 	paths  export	unset  cmd atoi  var\
				init		case_parse	do_command	data_utils	double_chevron\
				input_traversal	case_parse_2 var_substitution cmd_echo utils\
				utils2 utils3 relative_paths signals wildcard_bonus wildcard_bonus2\
				wildcard_bonus3 case_parse3



SRC = $(addsuffix .c, $(SRC_FILES))
OBJ = $(addsuffix .o, $(SRC_FILES))

######## FLAGS #################################################################

CC = gcc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = 

######## RULES #################################################################

all: $(NAME)

$(NAME): $(LIBFTDIR)/libft.a $(OBJ)
	$(CC) $(CFLAGS) -ggdb3 $(OBJ) -o $(NAME) -lreadline -lncurses libreadline.a $(LIBFTDIR)/libft.a

asan: $(OBJ) $(LIBFTDIR)/libft.a
	# $(CC) $(CFLAGS) $(OBJ) -o $(NAME) 
	$(CC) -fsanitize=address $(OBJ) -o $(NAME) -lreadline -lncurses libreadline.a $(LIBFTDIR)/libft.a
test:
	gcc -ggdb3 test.c $(LIBFTDIR)/libft.a -I$(LIBFTINCLUDES) 

%.o: %.c
	$(CC) $(CFLAGS) -ggdb3 -c -I$(LIBFTINCLUDES) $< -o $@ 

$(LIBFTDIR)/libft.a:
	git submodule update --remote
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
