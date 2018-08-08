/* Crusty old DOS VGA dragon from 2001 by orthopteroid@gmail.com built with TC++. public domain license. */

#ifndef _platform_h_
#define _platform_h_

#ifndef __COLORS
#define __COLORS

enum COLORS {
    BLACK,		    /* dark colors */
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    LIGHTGRAY,
    DARKGRAY,		    /* light colors */
    LIGHTBLUE,
    LIGHTGREEN,
    LIGHTCYAN,
    LIGHTRED,
    LIGHTMAGENTA,
    YELLOW,
    WHITE
};

#endif /* __COLORS */

typedef struct timer
{
  double c, l;
} timer;

void p_init();
void p_resolution(int* pX, int* pY);
void p_shutdown();

void p_clear();
void p_color(int c);
void p_draw(int x,int y);
void p_move(int x,int y);
void p_pixel(int x,int y);

int p_vblank();
void p_char(int* pChar);
void p_mouse(int* pX, int* pY, int* pB);
void p_timer(timer* pT);

#endif /* _platform_h_ */
