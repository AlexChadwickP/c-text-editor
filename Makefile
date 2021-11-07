all: main.c
	$(CC) main.c -o app -Wall -Wextra -pedantic -std=c99
