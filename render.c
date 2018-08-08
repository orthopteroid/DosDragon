/* Crusty old DOS VGA dragon from 2001 by orthopteroid@gmail.com built with TC++. public domain license. */

#include <stdio.h>

#include "render.h"

#define INIT (r_clearFunc)
                                                         
/****************************/

int r_bStartEdge;
VFunc r_clearFunc = 0;
IFunc  r_colorFunc = 0;
IIFunc r_drawFunc = 0;
IIFunc r_moveFunc = 0;
IIFunc r_pixelFunc = 0;

int r_iStack;
int r_stack[9];

/****************************/

void r_init( VFunc clearFunc, 
             IFunc colorFunc, IIFunc drawFunc,
             IIFunc moveFunc, IIFunc pixelFunc )
{
  r_clearFunc = clearFunc;
  r_colorFunc = colorFunc;
  r_drawFunc = drawFunc;
  r_moveFunc = moveFunc;
  r_pixelFunc = pixelFunc;
}

void r_root()
{
  r_iStack = 0;

  r_clearFunc();
}

void r_push()
{
  r_iStack++;
}

void r_pop()
{
  r_iStack--;
}

void r_setColor( int iColor )
{
  r_stack[ r_iStack ] = iColor;
}

void r_startEdge()
{
  r_bStartEdge = 1;
}

void r_polypixel( int v[], int nv )
{
  int i=0;

  if( !INIT ) return;

  r_colorFunc( r_stack[ r_iStack ] );

  for( i=0; i<nv; i+=2 )
  {
    r_pixelFunc( v[i], v[i+1] );
  }
}

void r_openpoly( int v[], int nv )
{
  int i=0;

  if( !INIT ) return;

  r_colorFunc( r_stack[ r_iStack ] );

  r_moveFunc( v[0], v[1] );
  for( i=0; i<nv; i+=2 )
  {
    r_drawFunc( v[i], v[i+1] );
  }
}

void r_closedpoly( int v[], int nv )
{
  int i=0;

  if( !INIT ) return;

  r_colorFunc( r_stack[ r_iStack ] );

  r_moveFunc( v[0], v[1] );
  for( i=0; i<nv; i+=2 )
  {
    r_drawFunc( v[i], v[i+1] );
  }
  if( nv>5 )
  {
    r_drawFunc( v[0], v[1] );
  }
}

void r_polyline( int v[], int nv )
{
  int i=0;

  if( !INIT ) return;

  r_colorFunc( r_stack[ r_iStack ] );

  for( i=0; i<nv; i+=4 )
  {
    r_moveFunc( v[i], v[i+1] );
    r_drawFunc( v[i+2], v[i+3] );
  }
}

void r_edge( int v[], int nv )
{
  int i=0;

  if( !INIT ) return;

  r_colorFunc( r_stack[ r_iStack ] );

  for( i=0; i<nv; i+=2 )
  {
    if( r_bStartEdge && i<3 )
    {
      r_bStartEdge = 0;
      r_moveFunc( v[i], v[i+1] );
    }
    else
      r_drawFunc( v[i], v[i+1] );
  }
}

