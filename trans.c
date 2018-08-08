/* Crusty old DOS VGA dragon from 2001 by orthopteroid@gmail.com built with TC++. public domain license. */

#include <stdio.h>

#include "trans.h"

/****************************/

#define NODIAG

#define SWAP(a,b)   t_fTemp=a;a=b;b=t_fTemp;

#define VINIT(rv,a,b,c)  rv[0]=a;rv[1]=b;rv[2]=c;

#define VPHELPER        "%4.2f %4.2f %4.2f %s\n"

#ifdef NODIAG
  #define VPRINT(v,p)   ;
#else
  #define VPRINT(v,p)   printf( VPHELPER, v[0],v[1],v[2], (p?p:"") );
#endif

#define MPRINT(m,p)     VPRINT(m[0],p)\
                        VPRINT(m[1],0)\
                        VPRINT(m[2],0)
 
#define VDIAG(v,p)      VPRINT(v,p)
#define MDIAG(m,p)      MPRINT(m,p)

#define VSET(v,x,y,p)   VINIT(v,x,y,1)\
                        VDIAG(v,p)

#define VCOPY(v,a)      for(t_iTemp=0;t_iTemp<4;t_iTemp++)((float*)v)[t_iTemp]=((float*)a)[t_iTemp];
#define MCOPY(m,a)      for(t_iTemp=0;t_iTemp<10;t_iTemp++)((float*)m)[t_iTemp]=((float*)a)[t_iTemp];
#define MCLEAR(m)       for(t_iTemp=0;t_iTemp<10;t_iTemp++)((float*)m)[t_iTemp]=0;

#define MIDENTITY(m,p) \
  VINIT(m[0],1,0,0)\
  VINIT(m[1],0,1,0)\
  VINIT(m[2],0,0,1)\
  MDIAG(m,p)

#define MTRANSLATE(m,x,y,p) \
  VINIT(m[0],1,0,x)\
  VINIT(m[1],0,1,y)\
  VINIT(m[2],0,0,1)\
  MDIAG(m,p)

#define MSCALE(m,x,y,p) \
  VINIT(m[0],x,0,0)\
  VINIT(m[1],0,y,0)\
  VINIT(m[2],0,0,1)\
  MDIAG(m,p)

#define MROTATE(m,r,p) \
  VINIT(m[0],cos(r),-sin(r),0)\
  VINIT(m[1],sin(r),cos(r),0)\
  VINIT(m[2],0,0,1)\
  MDIAG(m,p)

#define MTHELPER(m,r,c) SWAP(m[r][c],m[c][r])
#define MTRANSPOSE(m,p) \
  MTHELPER(m,0,1)\
  MTHELPER(m,0,2)\
  MTHELPER(m,1,2)\
  MDIAG(m,p)

#define VVDOT(s,cv,rv) \
  (s)=(int)(cv[0]*rv[0]+cv[1]*rv[1]+cv[2]*rv[2]); /* <=== cast is optimization */

#define VMDOT(v,cv,m,p) \
  VVDOT(v[0],cv,m[0])\
  VVDOT(v[1],cv,m[1])\
  /*VVDOT(v[2],cv,m[2]) <=== removal is optimization */\
  VDIAG(v,p)

#define MMMHELPER(m,a,b,r,c) m[r][c]=a[r][0]*b[0][c]+a[r][1]*b[1][c]+a[r][2]*b[2][c];
#define MMMULTIPLY(m,a,b,p) \
  MMMHELPER(m,a,b,0,0)\
  MMMHELPER(m,a,b,0,1)\
  MMMHELPER(m,a,b,0,2)\
  MMMHELPER(m,a,b,1,0)\
  MMMHELPER(m,a,b,1,1)\
  MMMHELPER(m,a,b,1,2)\
  MMMHELPER(m,a,b,2,0)\
  MMMHELPER(m,a,b,2,1)\
  MMMHELPER(m,a,b,2,2)\
  MDIAG(m,p)

/*******************************************/

int t_bDiag = 0;
int t_iTemp;
float t_fTemp;
int t_iStack;
matrix t_stack[9];
matrix t_t;
matrix t_m;
vector t_i;

/* factored-out to reduce code-size */
void t_multiply()
{
  MMMULTIPLY( t_t, t_stack[ t_iStack ], t_m, "xM" );
  MCOPY( t_stack[ t_iStack ], t_t );
}

/****************************/

void t_root()
{
  t_iStack = 0;
  MIDENTITY( t_stack[ t_iStack ], "I" );
}

void t_push()
{
  t_iStack++;
  MCOPY( t_stack[ t_iStack ], t_stack[ t_iStack - 1 ] );
}

void t_pop()
{
  t_iStack--;
}

void t_translate( float x, float y )
{
  MTRANSLATE( t_m, x, y, "T" );
  t_multiply();
}

void t_scale( float sx, float sy )
{
  MSCALE( t_m, sx, sy, "S" );
  t_multiply();
}

void t_rotate( float r )
{
  MROTATE( t_m, g2r(r), "R" );
  t_multiply();
}

void t_apply( int ov[], int iv[], int nv )
{
  int p=0;
  for( p=0; p<nv; p+=2 )
  {
    VSET( t_i, iv[p], iv[p+1], "i" );
    VMDOT( ((ov+p)), t_i, t_stack[ t_iStack ], ".i" ); /* <=== & is optimization */
  }
}

void t_copy( matrix* pMatrix )
{
  MCOPY( *pMatrix, t_stack[ t_iStack ] );
}

void t_concat( matrix* pMatrix )
{
  MCOPY( t_m, *pMatrix );
  t_multiply();
}
