/* Crusty old DOS VGA dragon from 2001 by orthopteroid@gmail.com built with TC++. public domain license. */

#include <stdio.h>
#include <graphics.h>
#include <time.h>

#include "mouse.h"
#include "platform.h"

void p_init()
{
  int iDriver = VGA;
  int iMode = VGAHI;
  int iCode;

  initgraph( &iDriver, &iMode, "" );
  iCode = graphresult();
  if( iCode != grOk )
  {
    closegraph();
    printf(" Graphics System Error: %s\n", grapherrormsg( iCode ) );
    exit( 1 );
  }
  setviewport( 0, 0, getmaxx(), getmaxy(), 1 );
  setcolor( WHITE );

  {
    int m = 0;
    ms_reset( &m );
    ms_set_mouse_pos( 320, 240 );
  }
}

void p_shutdown()
{
  closegraph();
}

void p_resolution(int* pX, int* pY)
{
  *pX = getmaxx();
  *pY = getmaxy();
}

void p_clear()
{
  cleardevice();
}

void p_color(int c)
{
  setcolor( c );
}

void p_draw(int x,int y)
{
  lineto(x,y);
}

void p_move(int x,int y)
{
  moveto(x,y);
}

void p_pixel(int x,int y)
{
  int x0 = getx();
  int y0 = gety();
  putpixel( x, y, getcolor() );
  moveto( x0, y0 );
}

/* Thanks Allegro! */
int p_vblank()
{
  static int vbState = 0; /* bit8:bNowBlanking, bit7:bWasDrawing */

  /*
  #define VBLANKBIT    (inportb(0x3DA) & 8)
  #define VBLANKTEST1  (vbState=((!vbState & 0x80) >> 1))
  #define VBLANKTEST2  (vbState|=VBLANKBIT)
  #define DETECTVBLANK (VBLANKTEST1 ? VBLANKTEST2 : VBLANKTEST2)
  */

  vbState = ((!vbState & 0x80) >> 1) | (inportb(0x3DA) & 8);
  return vbState;
}

void p_char(int* pChar)
{
  *pChar = ( kbhit() ? getch() : 0 );
}

void p_mouse(int* pX, int* pY, int* pB)
{
  *pB = ms_get_mouse_pos( pX, pY );
}

void p_timer(timer* pT)
{
  pT->l = pT->c;
  pT->c = ((double)clock()) / CLK_TCK;
}
