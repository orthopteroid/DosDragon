/* Crusty old DOS VGA dragon from 2001 by orthopteroid@gmail.com built with TC++. public domain license. */
/* This file implements the animated dragon. Geometry is 'baked in' using #include directives. */

#include "trans.h"
#include "render.h"
#include "platform.h"
#include "decomp.h"

#include "animate.h"
#include "text.h"

#include <stdio.h>
#include <assert.h>

#define X(i) (2*i)
#define Y(i) (2*i+1)

/************************************/

timer g_aTimer = {0,0};

#define MAXAXIS 10
#define MAXPOLY 50

int g_iPolyBuf[ MAXPOLY ];
char g_cCommand = ' ';
int g_cCommandLatch = 0;
int x = 320, y = 150;
double s = 1.0;

#include "spine."
int g_sSpinePoly[] = {POLY};
int g_sSpineAxis[] = {AXIS};

#include "wing."
int g_sWingPoly[] = {POLY};
int g_sWingAxis[] = {AXIS};

#include "head."
int g_sHeadPoly[] = {POLY};
int g_sHeadAxis[] = {AXIS};

#include "jaw."
int g_sJawPoly[] = {POLY};
int g_sJawAxis[] = {AXIS};

/**** animation */

BOUNCE_DECLARE(g_aSpine);
BOUNCE_DECLARE(g_aWing);
BOUNCE_DECLARE(g_aHead);
BOUNCE_DECLARE(g_aJaw);

double g_aHeadAngle;

/************************************/

void init()
{
  p_init();
  p_clear();
  r_init( p_clear, p_color, p_draw, p_move, p_pixel );

  /* init animations */
  BOUNCE_INIT( g_aSpine, 3, 0, 6 )
  BOUNCE_INIT( g_aWing, 3, 0, 5 )
  BOUNCE_INIT( g_aHead, 3, 0, 40 )
  BOUNCE_INIT( g_aJaw, 7, 0, 10 )
}

void tick_render()
{
  r_root();
  r_setColor( BROWN );
  t_root();

  /* render world */

  t_translate( x, y );
  t_scale( s, s );
  t_push();
  {
    int iSegs = 5;
    static int ax[ numof(g_sSpineAxis) ];
    static int ax0[ numof(g_sSpineAxis) ];
    int i;
    memcpy( ax, g_sSpineAxis, sizeof(ax) );
    t_translate( -10,70 );
    t_push();
      for( i=0;i<iSegs;i++) {
        t_translate( ax[X(2)], ax[Y(2)] );        /* move to paste-point */
        t_translate( -ax[X(0)], -ax[Y(0)] );      /* move translation point to origin */
        t_rotate( 97 + g_aSpine.v );              /* rotate about origin */
        t_scale( 0.8, 0.8 );                      /* scale segments down */
        t_push();
          t_translate( -g_sSpineAxis[X(1)], -g_sSpineAxis[Y(1)] );  /* normalize */
          t_apply( g_iPolyBuf, g_sSpinePoly, numof(g_sSpinePoly) ); /* transform polygon */
        t_pop();
        r_openpoly( g_iPolyBuf, numof(g_sSpinePoly) );   /* render */
        t_push();
          t_scale( 1.5, 1.5 );                      /* scale axis not so much */
          t_translate( -g_sSpineAxis[X(1)], -g_sSpineAxis[Y(1)] ); /* normalize */
          t_apply( ax0, ax, numof(g_sSpineAxis) );  /* transform axes */
          memcpy( ax, ax0, sizeof(ax) );
        t_pop();
      }
    t_pop();
  }
  t_pop();

  t_push();
  {
    int iSegs = 4;
    static int ax[ numof(g_sWingAxis) ];
    static int ax0[ numof(g_sWingAxis) ];
    int i,j;
    for(j=0;j<2;j++) { /* 0 = left, 1 = right */
      int iPasteAxis = j?2:0;
      int iCopyAxis = j?0:2;
      float fAngle = j?98.0+g_aWing.v:2-g_aWing.v;
      memcpy( ax, g_sWingAxis, sizeof(ax) );
      t_translate( j?+20:-20 , 0 );
      t_push();
        /* reuse the code below for the left and right wings */
        for( i=0;i<iSegs;i++) {
          t_translate( ax[X(iPasteAxis)], ax[Y(iPasteAxis)] ); /* move to paste-point */
          t_translate( -ax[X(iCopyAxis)], -ax[Y(iCopyAxis)] ); /* move translation point to origin */
          t_rotate( fAngle );                                  /* rotate about origin */
          t_scale( 0.75, ((i%2)?0.35:1.35) );                  /* scale segments down */
          t_push();
            t_translate( -g_sWingAxis[X(1)], -g_sWingAxis[Y(1)] ); /* normalize */
            t_apply( g_iPolyBuf, g_sWingPoly, numof(g_sWingPoly) );/* transform polygon */
          t_pop();
          r_openpoly( g_iPolyBuf, numof(g_sWingPoly) );        /* render */
          t_push();
            t_scale( 1.5,1.5 );                                /* scale axis not so much */
            t_translate( -g_sWingAxis[X(1)], -g_sWingAxis[Y(1)] ); /* normalize */
            t_apply( ax0, ax, numof(g_sWingAxis) );            /* transform axes */
            memcpy( ax, ax0, sizeof(ax) );
          t_pop();
        }
      t_pop();
    }
  }
  t_pop();

  t_push();
  {
    t_translate( 0, -30 );
    t_rotate( g_aHeadAngle );            /* rotate about origin */
    if( g_aHeadAngle < 60 )
    {
      t_scale( -1, 1 );                 /* flip for other side */
    }
    t_push();
      t_translate( -g_sHeadAxis[X(1)], -g_sHeadAxis[Y(1)] );   /* normalize */
      t_apply( g_iPolyBuf, g_sHeadPoly, numof(g_sHeadPoly) );  /* transform polygon */
      r_openpoly( g_iPolyBuf, numof(g_sHeadPoly) );            /* render */
    t_pop();
    t_push();
      t_translate( -g_sJawAxis[X(1)], -g_sJawAxis[Y(1)] );     /* normalize */
      t_translate( g_sJawAxis[X(0)], g_sJawAxis[Y(0)] );     /* unnormalize */
      t_rotate( g_aJaw.v );            /* rotate about origin */
      t_translate( -g_sJawAxis[X(0)], -g_sJawAxis[Y(0)] );     /* normalize */
      t_apply( g_iPolyBuf, g_sJawPoly, numof(g_sJawPoly) );    /* transform polygon */
      r_openpoly( g_iPolyBuf, numof(g_sJawPoly) );             /* render */
    t_pop();
  }
  t_pop();
}

void tick_input()
{
   p_timer( &g_aTimer );

  {
    static int c0=0;
    c0 = g_cCommandLatch;
    p_char( &g_cCommandLatch );
    if( c0==0 && g_cCommandLatch!=0 )
    {
      g_cCommand = g_cCommandLatch;
    }
  }
}

void tick_logic()
{
  switch( g_cCommandLatch ) {
  case 'w': y-=5; break;
  case 's': y+=5; break;
  case 'a': x-=5; break;
  case 'd': x+=5; break;
  case '+': s+=0.1; break;
  case '-': s-=0.1; break;
  default: ;
  }
  if( g_cCommand == 'r' ) {
    x = 320; y = 150; s = 1.0;
  }
}

void tick_animate()
{
  t_root();

  /* tick the animations */
  BOUNCE_TICK( g_aTimer, g_aSpine );
  BOUNCE_TICK( g_aTimer, g_aWing );
  BOUNCE_TICK( g_aTimer, g_aHead );
  BOUNCE_TICK( g_aTimer, g_aJaw );

  /* actual head angle */
  g_aHeadAngle = ( g_aHead.v > 20 ? 60 + g_aHead.v : g_aHead.v );
}

/**************************************/

int main( int argc, char* argv[] )
{
  int f=1;
  init();
  while( g_cCommand!='q' )
  {
    tick_input();
    tick_logic();
    tick_animate();
    if( p_vblank() )
    {
      tick_render();
    }
  }
  p_shutdown();
  return(0);
}
