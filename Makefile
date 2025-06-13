CC = cc
SRC = $(wildcard src/*.c)
HR = $(wildcard include/*.h)
OBJ = $(patsubst src/%.c, build/%.o, $(SRC))
CFLAGS = -Iinclude -Werror -Wextra -Wall
NAME = mos6502

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -o $(NAME) $(OBJ) -lncurses

build/%.o: src/%.c $(HR)
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -rf build

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: clean
