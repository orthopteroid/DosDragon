/* Crusty old DOS VGA dragon from 2001 by orthopteroid@gmail.com built with TC++. public domain license. */

#include "trans.h"
#include "render.h"
#include "animate.h"
#include "mesh.h"
#include "platform.h"

/************************************/

int sPointer[] = {10,10,-10,10,-10,-10,10,-10};
int mPointer[ numof(sPointer) ];

int pointer_x = 0;
int pointer_y = 0;
int pointer_b = 0;

void pointer_init()
{
}

void pointer_render()
{
  t_push();
    r_setColor( BROWN );
    t_translate( pointer_x, pointer_y );
    if( pointer_b ) t_rotate( 45 );
    t_apply( mPointer, sPointer, numof(sPointer) );
    r_closedpoly( mPointer, numof(mPointer) );
  t_pop();
}
