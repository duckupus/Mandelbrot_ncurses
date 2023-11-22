CC := gcc
CFLAGS := -Wall -Wextra
SRC := main.c

LIB := ncurses
LIBS := $(LIB:%=-l%)

DEST := Mandelbrot

main:
	$(CC) $(CFLAGS) $(SRC) -o $(DEST) $(LIBS) 
