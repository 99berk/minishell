NAME = minishell
NAME_B = minishell_bonus
CC = gcc
FLAGS =  -Wall -Wextra -Werror -g #-fsanitize=address -g

SRC =	src/main.c\
	src/free_n_exit.c\
	src/prompt.c\
	src/env.c\
	src/env_utils.c\
	src/utils.c\
	src/utils_2.c\
	src/quote.c\
	src/exec.c\
	src/pipe.c\
	src/pipe_utils.c\
	src/open_create.c\
	src/parse.c\
	src/dollar_parse.c\
	src/append.c\
	src/heredoc.c\
	src/input_output.c\
	src/parse_utils.c\
	builtins/cd.c\
	builtins/cmd_env.c\
	builtins/export_utils.c\
	builtins/export.c\
	builtins/unset.c\
	builtins/pwd.c\
	builtins/exit.c\
	builtins/echo.c\
	src/signals.c

SRC_B =	src/main.c\
	src/free_n_exit.c\
	src/env.c\
	src/env_utils.c\
	src/utils.c\
	src/utils_2.c\
	src/quote.c\
	src/exec.c\
	src/pipe.c\
	src/pipe_utils.c\
	src/open_create.c\
	src/parse.c\
	src/dollar_parse.c\
	src/append.c\
	src/heredoc.c\
	src/input_output.c\
	src/parse_utils.c\
	builtins/cd.c\
	builtins/cmd_env.c\
	builtins/export_utils.c\
	builtins/export.c\
	builtins/unset.c\
	builtins/pwd.c\
	builtins/exit.c\
	builtins/echo.c\
	bns/logic_op.c\
	bns/logic_utils.c\
	bns/prompt_bonus.c\
	bns/wildcard_utils.c\
	bns/wildcard.c\
	src/signals.c

LIBFT = libft/libft.a
GNL = gnl/gnl.a
OBJ = $(SRC:c=o)
OBJ_B = $(SRC_B:c=o)

GREEN := \033[0;32m
RED := \033[0;31m
RESET := \033[0m

BUILD_PRINT = @echo "$(GREEN)Building $@$(RESET)"
DELETE_PRINT = @echo "$(RED)Deleting ./minishell$(RESET)"
DELETE_OBJ = @echo "$(RED)Deleting Objects $(RESET)"
SUCCESS_MSG = @printf "$(GREEN)Compilation successful: $(NAME)$(RESET)\n"

all : $(NAME)

$(NAME) : $(OBJ)
	$(BUILD_PRINT)
	@make all -C libft
	@make all -C gnl
	@$(CC) $(FLAGS) $(OBJ) $(LIBFT) $(GNL) -lreadline -o $(NAME) 
	@printf "\033[K\r"
	$(SUCCESS_MSG)

%.o: %.c
	@printf "$(GREEN)Compiling $<$(RESET)\033[K\r"
	@$(CC) $(FLAGS) -c $< -o $@
	@printf "\033[K\r"

bonus : $(NAME_B)

$(NAME_B) : $(OBJ_B)
	$(BUILD_PRINT)
	@make all -C libft
	@make all -C gnl
	@$(CC) $(FLAGS) $(OBJ_B) $(LIBFT) $(GNL) -lreadline -o $(NAME)
	$(SUCCESS_MSG)

%.o: %.c
	@printf "$(GREEN)Compiling $<$(RESET)\033[K\r"
	@$(CC) $(FLAGS) -c $< -o $@
	@printf "\033[K\r"

clean :
	@make clean -C libft
	@make clean -C gnl
	$(DELETE_OBJ)
	@rm -rf $(OBJ) && rm -rf $(OBJ_B)
	@rm -rf builtins/*.o

fclean : clean
	@make fclean -C libft
	@make fclean -C gnl
	@rm -rf $(NAME)

re : fclean all

.PHONY: all bonus clean fclean re