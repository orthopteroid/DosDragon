/* Crusty old DOS VGA dragon from 2001 by orthopteroid@gmail.com built with TC++. public domain license. */

#ifndef _trans_h_
#define _trans_h_

#include <math.h>

#define d2r(d)  (3.14159*(d)/180.0)
#define g2r(g)  (3.14159*(g)/50.0)

typedef float vector[3];
typedef float matrix[3][3];

void t_root();
void t_push();
void t_pop();

void t_translate( float x, float y );
void t_scale( float sx, float sy );
void t_rotate( float r );
void t_apply( int ov[], int iv[], int nv );

void t_copy( matrix* pMatrix );
void t_concat( matrix* pMatrix );

#endif /* _trans_h_ */
