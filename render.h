/* Crusty old DOS VGA dragon from 2001 by orthopteroid@gmail.com built with TC++. public domain license. */

#ifndef _render_h_
#define _render_h_

#define numof( vect ) (sizeof(vect)/sizeof(vect[0]))

typedef void (*IIFunc)(int,int);
typedef void (*IFunc)(int);
typedef void (*VFunc)(void);

void r_init( VFunc clearFunc, 
             IFunc colorFunc, IIFunc drawFunc,
             IIFunc moveFunc, IIFunc pixelFunc );

void r_root();
void r_push();
void r_pop();
void r_setColor( int iColor );
void r_startEdge();

void r_polypixel( int v[], int nv );
void r_polyline( int v[], int nv );
void r_edge( int v[], int nv );

void r_openpoly( int v[], int nv );
void r_closedpoly( int v[], int nv );

#endif /* _render_h_ */
