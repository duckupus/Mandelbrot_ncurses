C program to explore the [Mandelbrot set](https://en.wikipedia.org/wiki/Mandelbrot_set) interactively

Dependencies: `ncurses`

Usage: `wasd`, and `hjkl` are supported for navigation. \
`z` and `x` to zoom in and out accordingly. \
`q` to quit.

Compiling and running:
Running `make` should compile and output the binary into `Mandelbrot`.

NOTE: currently, the program uses `double` to do computations, and hence has a limit to how much it is able to zoom in
