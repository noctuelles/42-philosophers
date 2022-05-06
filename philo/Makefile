# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: plouvel <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/05/06 16:28:09 by plouvel           #+#    #+#              #
#    Updated: 2022/05/06 16:34:17 by plouvel          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS_DIR	=	srcs

INC_DIR		=	includes

OBJS_DIR	=	objs

SRCS		=	forks.c               \
				main.c                \
				parsing.c             \
				philosopher.c         \
				philosopher_routine.c \
				supervisor.c          \
				time_utils.c          \
				utils.c

OBJS		=	$(addprefix $(OBJS_DIR)/, $(SRCS:.c=.o))

NAME		=	philo

RM			=	rm -rf

CC			=	cc

CFLAGS		=	-Wall -Werror -Wextra -I $(INC_DIR)

CLIBS		=	-lpthread

$(NAME):		$(OBJS)
				$(CC) $(OBJS) $(CFLAGS) $(CLIBS) -o $(NAME)

$(OBJS_DIR)/%.o:$(SRCS_DIR)/%.c
				@mkdir -p $(dir $@)
				$(CC) $(CFLAGS) -c $< -o $@

all:			$(NAME)

clean:
				$(RM) $(OBJS_DIR)

fclean:			clean
				$(RM) $(NAME)

re:				fclean all

.PHONY:			all clean fclean re bonus
