CC := gcc
CFLAGS := -Wall -Wextra -O3
SRC := main.c

LIB := ncurses gmp
LIBS := $(LIB:%=-l%)

DEST := Mandelbrot

main:
	$(CC) $(CFLAGS) $(SRC) -o $(DEST) $(LIBS) 
