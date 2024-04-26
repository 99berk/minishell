NAME = gnl.a
FLAG = -Wall -Wextra -Werror
SRC = get_next_line.c\
	get_next_line_utils.c\

all:
	@gcc $(FLAG) -c $(SRC)
	@ar rc $(NAME) *.o
clean:
	@/bin/rm -f  *.o
fclean: clean
	@/bin/rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re