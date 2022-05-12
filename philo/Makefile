# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: plouvel <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/05/06 16:28:09 by plouvel           #+#    #+#              #
#    Updated: 2022/05/12 19:02:15 by plouvel          ###   ########.fr        #
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

SRCS_FULL	=	$(addprefix $(SRCS_DIR)/, $(SRCS))

OBJS		=	$(addprefix $(OBJS_DIR)/, $(SRCS:.c=.o))

DEPS		=	$(addprefix $(OBJS_DIR)/, $(SRCS:.c=.d))

NAME		=	philo

RM			=	rm -rf

CC			=	cc

CFLAGS		=	-Wall -Werror -Wextra -g3 -fsanitize=thread -MD -I $(INC_DIR)

NORMINETTE	=	norminette

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

norm:
				@$(NORMINETTE) $(SRCS_FULL)
				@$(NORMINETTE) $(INC_DIR)

-include $(DEPS)

.PHONY:			all clean fclean re norm 
