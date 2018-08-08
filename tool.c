/* Crusty old DOS VGA dragon from 2001 by orthopteroid@gmail.com built with TC++. public domain license. */
/* This file implements the DOS mode keyboard/mouse geometry editing tool. */

#include "trans.h"
#include "render.h"
#include "platform.h"
#include "decomp.h"

#include "animate.h"
#include "text.h"

#include <stdio.h>

#define X(i) (i*2)
#define Y(i) (i*2+1)

/************************************/

timer g_aTimer = {0,0};

/**** polygon stuff */

#define MAXPOLY 50 /* 25 polgon points */

int g_sPolygon[ MAXPOLY ];
int g_iPolygon = 0;

/**** axis stuff  */

#define MAXAXIS 10

int g_sAxis[ MAXAXIS ];
int g_iAxis = 10;

/**** iobuffer stuff */

/* 2 lines of XXXX NNN NNN NNN ... + padding */
#define IOBUFSIZE 2 * (5 + 4 * MAXPOLY + 10)

char g_szIOBuffer[ IOBUFSIZE ];
char* g_szDataFile = 0;

/**** mouse input */

int g_xMouse = 0;
int g_yMouse = 0;
int g_bClick = 0;
int g_bDown = 0;

/**** text input/output */

char g_cCommand = ' ';
char* g_szText[80];

/**** pointer */

int g_sPointer[] = {3,3,-3,3,-3,-3,3,-3};
SAW_DECLARE(g_aPointerRotation);
BOUNCE_DECLARE(g_aRot);
BOUNCE_DECLARE(g_aWing);

/************************************/

void init()
{
  p_init();
  p_clear();
  r_init( p_clear, p_color, p_draw, p_move, p_pixel );

  SAW_INIT( g_aPointerRotation, 33, 0, 100 )
  BOUNCE_INIT( g_aRot, 3, 0, 10 )
  BOUNCE_INIT( g_aWing, 3, 0, 5 )
}

void tick_render()
{
  static int mData[ MAXPOLY ];

  r_root();
  r_setColor( BROWN );
  t_root();

  /* render world */

  t_push();
    t_translate( g_xMouse, g_yMouse );
    t_rotate( g_aPointerRotation.v );
    {
      int mPointer[ numof(g_sPointer) ];
      t_apply( mPointer, g_sPointer, numof(g_sPointer) );
      r_closedpoly( mPointer, numof(mPointer) );
    }
  t_pop();

  t_push();
    t_translate( 10, 470 );
    t_scale( 2, -3 ); /* text goes bottom-up */
    text_render();
  t_pop();

  if( g_cCommand != 'p' ) /* disable in prototype mode */
  {
    int iAxis = 0;
    t_push();
      t_scale( 1, -1 ); /* text goes bottom-up */
      do{
        int iSegs = 0;
        int sMarker[ MAXPOLY ];
        d_55grid( sMarker, MAXPOLY, &iSegs, t_szNumberData[ iAxis ] );

        t_push();
          t_translate(  g_sAxis[X(iAxis)], -g_sAxis[Y(iAxis)] ); /* offsets go top-down */
          t_apply( mData, sMarker, iSegs );
          r_openpoly( mData, iSegs );
        t_pop();
      }while( ++iAxis < MAXAXIS );
    t_pop();

    t_push();
      t_apply( mData, g_sPolygon, g_iPolygon );
      r_openpoly( mData, g_iPolygon );
    t_pop();
  } else if( stricmp( g_szDataFile, "spine" ) == 0 ) {
    static int iSegs = 5;
    static int ax[ MAXAXIS ];
    static int ax0[ MAXAXIS ];
    int i;
    t_push();
      memcpy( ax, g_sAxis, sizeof(ax) );
      for( i=0;i<iSegs;i++) {
        t_translate( ax[X(2)], ax[Y(2)] );        /* move to paste-point */
        t_translate( -ax[X(0)], -ax[Y(0)] );      /* move translation point to origin */
        t_translate( ax[X(1)], ax[Y(1)] );        /* undo rotation point move */
        t_rotate( 95 + g_aRot.v );                /* rotate about origin */
        t_scale( 0.8, 0.8 );                      /* scale segments down */
        t_translate( -ax[X(1)], -ax[Y(1)] );      /* move rotation point to origin */
        t_apply( mData, g_sPolygon, g_iPolygon ); /* transform polygon */
        r_openpoly( mData, g_iPolygon );          /* render */
        t_apply( ax0, ax, MAXAXIS );              /* transform axes */
        memcpy( ax, ax0, sizeof(ax) );
      }
    t_pop();
  } else if( stricmp( g_szDataFile, "wing" ) == 0 ) {
    static int iSegs = 4;
    static int ax[ MAXAXIS ];
    static int ax0[ MAXAXIS ];
    int i,j;
    for(j=0;j<2;j++) { /* 0 = left, 1 = right */
      int iPasteAxis = j?2:0;
      int iCopyAxis = j?0:2;
      float fAngle = j?98.0+g_aWing.v:2-g_aWing.v;
      t_push();
        memcpy( ax, g_sAxis, sizeof(ax) );
        for( i=0;i<iSegs;i++) {
          /* reuse the code below for the left and right wings */
          t_translate( ax[X(iPasteAxis)], ax[Y(iPasteAxis)] ); /* move to paste-point */
          t_translate( -ax[X(iCopyAxis)], -ax[Y(iCopyAxis)] ); /* move translation point to origin */
          t_translate( ax[X(1)], ax[Y(1)] );                   /* undo rotation point move */
          t_rotate( fAngle );                                  /* rotate about origin */
          t_scale( 0.75, ((i%2)?0.5:1.5) );                    /* scale segments down */
          t_translate( -ax[X(1)], -ax[Y(1)] );                 /* move rotation point to origin */
          t_apply( mData, g_sPolygon, g_iPolygon );            /* transform polygon */
          r_openpoly( mData, g_iPolygon );                     /* render */
          t_apply( ax0, ax, g_iAxis );                         /* transform axes */
          memcpy( ax, ax0, sizeof(ax) );
        }
      t_pop();
    }
  }
}

void tick_input()
{
   p_timer( &g_aTimer );

  {
    static int b0=0;
    static int b1=0;
    b0 = b1;
    p_mouse( &g_xMouse, &g_yMouse, &b1 );
    g_bClick = ( b0==0 && b1!=0 ? 1 : 0 );
    g_bDown = b1;
  }

  {
    static int c0=0;
    static int c1=0;
    c0 = c1;
    p_char( &c1 );
    if( c0==0 && c1!=0 )
    {
      char* szText = "";
      g_cCommand = c1;
      switch( g_cCommand )
      {
      case 'a': szText="ADD"; break;
      case 'd': szText="DELETE"; break;
      case 'm': szText="MOVE"; break;
      case 'l': szText=( g_szDataFile ? "" : "NOFILE" ); break;
      case 's': szText=( g_szDataFile ? "" : "NOFILE" ); break;
			default: ;
      }
      g_szText[0] = 0;
      sprintf( g_szText, "%s", szText );
      text_set( g_szText );
    }
  }
}

void tick_logic()
{
  int p = -1;
  if( g_bDown )
  {
    double d = sqrt(640.0*640.0+480.0*480.0)+1;
    int i = 0;
    for( i=0; i<g_iPolygon; i+=2 )
    {
      double dx = abs(g_xMouse - g_sPolygon[i]);
      double dy = abs(g_yMouse - g_sPolygon[i+1]);
      double nd = sqrt( dx * dx + dy * dy );
      if( nd < d ) { p = i; d = nd; }
    }
  }

  if( g_cCommand=='a' && g_bClick && g_iPolygon < MAXPOLY )
  {
    g_sPolygon[ g_iPolygon++ ] = g_xMouse; g_sPolygon[ g_iPolygon++ ] = g_yMouse;
  }
  else if( g_cCommand=='d' && g_bClick && p > -1 )
  {
    memcpy( &g_sPolygon[ p ], &g_sPolygon[ p+2 ], g_iPolygon-2 - p );
    g_iPolygon -= 2;
  }
  else if( g_cCommand=='m' && g_bDown && p > -1 )
  {
    g_sPolygon[ p+0 ] = g_xMouse; g_sPolygon[ p+1 ] = g_yMouse;
  }
  else if( g_cCommand >= '1' && g_cCommand <= '5' )
  {
    int iAxis = g_cCommand - '1';
    g_sAxis[X(iAxis)] = g_sAxis[X(iAxis)] ? 0 : g_xMouse;
    g_sAxis[Y(iAxis)] = g_sAxis[Y(iAxis)] ? 0 : g_yMouse;
    g_cCommand = ' ';
  }
  else if( g_cCommand=='s' && g_szDataFile )
  {
    int rc = unlink( g_szDataFile );
    FILE* pFile = fopen( g_szDataFile, "w" );
    d_rawEncode( g_sPolygon, g_iPolygon, g_sAxis, g_iAxis, g_szIOBuffer );
    fprintf( pFile, g_szIOBuffer );
    fclose( pFile );
    g_cCommand = ' ';
  }
  else if( g_cCommand=='l' && g_szDataFile )
  {
    FILE* pFile = fopen( g_szDataFile, "r" );
    fread( g_szIOBuffer, 1, IOBUFSIZE, pFile );
    d_rawScan( &g_iPolygon, &g_iAxis, g_szIOBuffer );
    fseek( pFile, 0, SEEK_SET );
    fread( g_szIOBuffer, 1, IOBUFSIZE, pFile );
    d_rawDecode( &g_sPolygon, &g_sAxis, g_szIOBuffer );
    fclose( pFile );
    g_cCommand = ' ';
  }
}

void tick_animate()
{
  t_root();

  SAW_TICK( g_aTimer, g_aPointerRotation );
  BOUNCE_TICK( g_aTimer, g_aRot );
  BOUNCE_TICK( g_aTimer, g_aWing );
}

/**************************************/

int main( int argc, char* argv[] )
{
  if( argc == 2 ) g_szDataFile = argv[1];

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
