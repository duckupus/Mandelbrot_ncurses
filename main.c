#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ncurses.h>
#include <gmp.h>

unsigned long startx = 0; // amount of characters in x pos
unsigned long starty = 0; // amount of characters in y pos

mpf_t coord_x;
mpf_t coord_y;

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

mpf_t dx;
mpf_t dy;
mpf_t offset_mand;

void modify_arr(const mpf_t real, const mpf_t imag, char *buf) {
    //double bigimag = imag + offset_mand;
    mpf_t bigimag;
    mpf_init(bigimag);
    mpf_add(bigimag, imag, offset_mand);
    //double keepreal = real;
    mpf_t keepreal;
    mpf_init_set(keepreal, real);

    //dx = offset_mand/startx; // okay I'm dumb I think
    mpf_div_ui(dx, offset_mand, startx);
    //dy = offset_mand/starty; // okay I'm dumb I think
    mpf_div_ui(dy, offset_mand, starty);
    int k = 0;
    const int maxiter = 255;
    mpf_t tmpimag;
    mpf_init(tmpimag);
    mpf_t tmpreal;
    mpf_init(tmpreal);

    mpf_t u;
    mpf_init(u);
    mpf_t v;
    mpf_init(v);
    mpf_t u2;
    mpf_init(u2);
    mpf_t v2;
    mpf_init(v2);

    mpf_t cmp;
    mpf_init(cmp);
    mpf_t tmp;
    mpf_init(tmp);
    for(unsigned long int y = 0; y < starty; y++) {
        mpf_mul_ui(tmpimag, dy, y);
        mpf_sub(tmpimag, bigimag, tmpimag);
        //imag = bigimag - y*dy;
        for(unsigned long int x = 0; x < startx; x++) {
            //double u = 0.0;
            mpf_set_d(u, 0.0);
            //double v = 0.0;
            mpf_set_d(v, 0.0);
            //double u2 = u*u;
            //double v2 = v*v;
            mpf_mul(u2, u, u);

            mpf_mul(v2, v, v);
            //real = keepreal + x*dx;
            mpf_mul_ui(tmpreal, dx, x);
            mpf_add(tmpreal, keepreal, tmpreal);
            /*
            for(k = 0; k < maxiter && (u2 + v2 < 4.0); k++) {
                v = 2 * u * v + imag;
                u = u2 - v2 + real;
                u2 = u * u;
                v2 = v * v;
            }
            */
            mpf_add(cmp, u2, v2);
            for(k = 0; k < maxiter && (mpf_cmp_d(cmp, 4.0) < 0); k++) {
                //v = 2 * u * v + imag;
                mpf_mul_ui(tmp, u, 2);
                mpf_mul(tmp, tmp, v);
                mpf_add(v, tmp, tmpimag);
                //u = u2 - v2 + real;
                mpf_sub(tmp, u2, v2);
                mpf_add(u, tmp, tmpreal);
                //u2 = u * u;
                mpf_mul(u2, u, u);
                //v2 = v * v;
                mpf_mul(v2, v, v);
            }
            buf[x + y*startx] = k;
        }
    }
    mpf_clear(bigimag);
    mpf_clear(keepreal);
    mpf_clear(tmpimag);
    mpf_clear(tmpreal);
    mpf_clear(u);
    mpf_clear(v);
    mpf_clear(u2);
    mpf_clear(v2);
    mpf_clear(cmp);
    mpf_clear(tmp);
    return;
}

void logging(char *x) {
    return;
}

int main(int argc, char **argv) {
    initscr();
    clear();
    noecho();
    cbreak(); //no line buffering
              //WINDOW *my_win; my_win = newwin(LINES, COLS, 0, 0);
    char ch;
    char *buf = NULL;
    mpf_init_set_d(coord_x, -2.0);
    mpf_init_set_d(coord_y, -2.0);
    mpf_init_set_d(dx, 1.0);
    mpf_init_set_d(dy, 1.0);

    mpf_init_set_d(offset_mand, 2.2);

    buf = malloc(startx*starty*sizeof(char)); //gives an address at least
    if(buf == NULL) {
        perror("Malloc");
    }
    do {
        if(startx != COLS || starty != LINES) { //changed terminal screen size. maybe `realloc` next time?
            startx = COLS;
            starty = LINES;
            free(buf);
            buf = malloc(startx*starty*sizeof(char));
            if(buf == NULL) {
                perror("Malloc");
            }
        }
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
        mpf_t zoom_factor;
        mpf_init_set_d(zoom_factor, 0.05);
        mpf_t zoom_offset;
        mpf_init_set_d(zoom_offset, 0.1);
        switch(ch) {
            case 'w':
            case 'k':
                //coord_y += dy;
                mpf_add(coord_y, coord_y, dy);
                break;
            case 's':
            case 'j':
                //coord_y -= dy;
                mpf_sub(coord_y, coord_y, dy);
                break;
            case 'a':
            case 'h':
                //coord_x -= dx;
                mpf_sub(coord_x, coord_x, dx);
                break;
            case 'd':
            case 'l':
                //coord_x += dx;
                mpf_add(coord_x, coord_x, dx);
                break;
            case 'z':
                //offset_mand -= 0.1;
                mpf_sub(offset_mand, offset_mand, zoom_offset);
                //coord_x += 0.05;
                mpf_add(coord_x, coord_x, zoom_factor); 
                //coord_y += 0.05;
                mpf_add(coord_y, coord_y, zoom_factor); 
                break;
            case 'x':
                //offset_mand += 0.1;
                mpf_add(offset_mand, offset_mand, zoom_offset);
                //coord_x -= 0.05;
                mpf_sub(coord_x, coord_x, zoom_factor); 
                //coord_y -= 0.05;
                mpf_sub(coord_y, coord_y, zoom_factor); 
                break;
        }
    } while (ch != 'q');
    endwin();
    return 0;
}
