/* Crusty old DOS VGA dragon from 2001 by orthopteroid@gmail.com built with TC++. public domain license. */

#include <ctype.h>
#include <assert.h>

/* gads - strtok isn't working right! */
char* mytok(char* a, char* b)
{
  static char* d = 0;
  char* c = a ? a : d + 1;
  if( c != 0 ) {
    while(*c != 0 && strchr(b, *c) != 0) c++; /* find start of token */
    if( *c == 0 ) c = 0; /* this call returns 0 */
    else {
      d = c;
      while(*d != 0 && strchr(b, *d) == 0) d++; /* find end of token */
      if( *d != 0 ) *d = 0; /* mark end of this token */
      else d--; /* next call will return 0 */
    }
  }
  return c;
}

/***********************************
 * 5x5 grid compression
        4
  4 uvwxy 4
    pqrst
    klmno
    fghij
  0 abcde
    0   4
 */

void d_55grid( int ov[], int mv, int* pnv, char iv[] )
{
  int j;
  int i = *pnv;

  j = 0;
  while( i < mv && iv[ j ] )
  {
    int iData = iv[ j++ ] - 'a';
    ov[ i++ ] = iData % 5;
    ov[ i++ ] = iData / 5;
  }

  *pnv = i;
}

/************************************/

void d_127( int ov[], int mv, int* pnv, char iv[] )
{
  int j;
  int i = *pnv;

  j = 0;
  while( i < mv && iv[ j ] )
  {
    ov[ i++ ] = iv[ j++ ];
  }

  *pnv = i;
}

/************/

void d_rawEncode( int* v, int nv, int* a, int na, char* szBuffer )
{
  static char szTemp[80];
  int i;
  *szBuffer = 0;
  strcat( szBuffer, "#undef AXIS\n#undef POLY\n" );
  strcat( szBuffer, "#define AXIS " );
  sprintf( szTemp, "%d", a[ 0 ] ); strcat( szBuffer, szTemp );
  for( i=1; i < na; i++ )
  {
    sprintf( szTemp, ",%d", a[ i ] ); strcat( szBuffer, szTemp );
  }
  strcat( szBuffer, "\n" );
  strcat( szBuffer, "#define POLY " );
  sprintf( szTemp, "%d", v[ 0 ] ); strcat( szBuffer, szTemp );
  for( i=1; i < nv; i++ )
  {
    sprintf( szTemp, ",%d", v[ i ] ); strcat( szBuffer, szTemp );
  }
  strcat( szBuffer, "\n" );
}

void d_rawScan( int* pnv, int* pna, char* szBuffer )
{
  char* sz;
  char m = 0;
  *pnv=0;
  *pna=0;

  sz = mytok( szBuffer, " " );
  while( sz )
  {
    if( *sz == '#' ) {
      if( *(sz+1) == 'u' ) sz = mytok( 0, " " ); /* skip ident */
      else if( *(sz+1) == 'd' ) m = 0; /* reset mode */
      else assert( 0 );
    } else if( *sz == 'P' ) m = 'P';
    else if( *sz == 'A' ) m = 'A';
    else if( m == 'P' ) (*pnv)++;
    else if( m == 'A' ) (*pna)++;
    else assert( 0 );
    sz = mytok( 0, ", \n" );
  }
}

void d_rawDecode( int* v, int* a, char* szBuffer )
{
  char* sz;
  char m = 0;
  int nv = 0;
  int na = 0;

  sz = mytok( szBuffer, " " );
  while( sz )
  {
    if( *sz == '#' ) {
      if( *(sz+1) == 'u' ) sz = mytok( 0, " " ); /* skip ident */
      else if( *(sz+1) == 'd' ) m = 0; /* reset mode */
      else assert( 0 );
    } else if( *sz == 'P' ) m = 'P';
    else if( *sz == 'A' ) m = 'A';
    else if( m == 'P' ) v[ nv++ ] = atoi( sz );
    else if( m == 'A' ) a[ na++ ] = atoi( sz );
    else assert( 0 );
    sz = mytok( 0, ", \n" );
  }
}

