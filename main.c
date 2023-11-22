#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ncurses.h>

int startx = 0; // amount of characters in x pos
int starty = 0; // amount of characters in y pos

double coord_x = 0;
double coord_y = 0;

char px_arr[] = ".,-~:;=!*#$@"; //12 in total (highest index == 11)
void print_arr(char *buf) {
    for(int y = 0; y < starty; y++)
        for(int x = 0; x < startx; x++)
        {
            char ch = buf[x + y*startx]/20;
            if(ch > 11) {
                mvaddch(y, x, '@');
                continue;
            } if(ch < 0) ch = 0;
            mvaddch(y, x, px_arr[ch]);
        }
    return;
}

double dx = 1.0;
double dy = 1.0;
double offset_mand = 2.2;

void modify_arr(double real, double imag, char *buf) {
    /*
    double real = -2.0;
    double imag = -2.0;
    */
    double bigimag = imag + offset_mand;
    double keepreal = real;
    dx = ((real + offset_mand) - real)/startx;
    dy = (bigimag - imag)/starty;
    int k = 0;
    const int maxiter = 255;
    for(int y = 0; y < starty; y++) {
        imag = bigimag - y*dy;
        for(int x = 0; x < startx; x++) {
            double u = 0.0;
            double v = 0.0;
            double u2 = u*u;
            double v2 = v*v;
            real = keepreal + x*dx;
            for(k = 0; k < maxiter && (u2 + v2 < 4.0); k++) {
                v = 2 * u * v + imag;
                u = u2 - v2 + real;
                u2 = u * u;
                v2 = v * v;
            }
            buf[x + y*startx] = k;
        }
    }
    return;
}

void logging(char *x) {
    return;
}

int main() {
    initscr();
    clear();
    noecho();
    cbreak(); //no line buffering
              //WINDOW *my_win; my_win = newwin(LINES, COLS, 0, 0);
    char ch;
    char *buf = NULL;
    coord_x = -2.0;
    coord_y = -2.0;
    do {
        startx = COLS;
        starty = LINES;
        buf = malloc(startx*starty*sizeof(char));
        for(unsigned long i = 0; i < startx*starty*sizeof(char); i++) buf[i] = ' ';
        /*
           for(int i = 0; i < startx; i++) {
           buf[i] = '+';
           buf[i + (startx * (starty-1))] = '+';
           }
           */
        modify_arr(coord_x, coord_y, buf);
        print_arr(buf);
        //mvprintw(1, 1, "startx: %d\n starty: %d\n", startx, starty);
        refresh();
        //ch = wgetch(my_win);
        ch = getch();
        switch(ch) {
            case 'w':
            case 'k':
                coord_y += dy;
                break;
            case 's':
            case 'j':
                coord_y -= dy;
                break;
            case 'a':
            case 'h':
                coord_x -= dx;
                break;
            case 'd':
            case 'l':
                coord_x += dx;
                break;
            case 'z':
                offset_mand -= 0.1;
                break;
            case 'x':
                offset_mand += 0.1;
                break;
        }
        free(buf);
    } while (ch != 'q');
    endwin();
    return 0;
}
