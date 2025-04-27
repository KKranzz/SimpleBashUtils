.PHONY: all clean s21_grep
all: s21_grep
rebuild: clean all
clean:
	rm -rd s21_grep
s21_grep:
	gcc -std=c11 -Wall -Werror -Wextra -o ./s21_grep main_grep.c
