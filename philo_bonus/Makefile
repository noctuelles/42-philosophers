# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: plouvel <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/05/06 16:28:09 by plouvel           #+#    #+#              #
#    Updated: 2022/05/12 16:09:47 by plouvel          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS_DIR	=	srcs

INC_DIR		=	includes

OBJS_DIR	=	objs

SRCS		=	main.c							\
				parsing.c						\
				philosopher.c					\
				philosopher_routine.c			\
				philosopher_routine_utils.c		\
				supervisor.c					\
				supervisor_utils.c				\
				time_utils.c					\
				string.c						\
				utils.c

OBJS		=	$(addprefix $(OBJS_DIR)/, $(SRCS:.c=.o))

DEPS		=	$(addprefix $(OBJS_DIR)/, $(SRCS:.c=.d))

NAME		=	philo_bonus

RM			=	rm -rf

CC			=	cc

NORMINETTE	=	norminette

CFLAGS		=	-MD -g3 -fsanitize=thread -I  $(INC_DIR)

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

-include $(DEPS)

.PHONY:			all clean fclean re
