/* Crusty old DOS VGA dragon from 2001 by orthopteroid@gmail.com built with TC++. public domain license. */

#include "trans.h"
#include "render.h"
#include "decomp.h"
#include "platform.h"

#include <string.h>

/************************************/

char* t_szLetterData[] =
{
  "akwokoe", "auxtnknjda", "txvpfbdj", "uadjtxu", "yuknkae", /* A B C D E */
  "yuknka", "yuaeom", "uakoye", "uywcae", "uywcbf",          /* F G H I J */
  "uakyke", "uae", "aumye", "auey", "uaeyu",                 /* K L M N O */
  "auyok", "ioyuacime", "auyoke", "yukoea", "uywc",          /* P Q R S T */
  "uaey", "ukcoy", "uamey", "uemay", "ukmcmoy", "uyae"       /* U V W X Y Z */
};

char* t_szNumberData[] =
{
  "qwcbd", "uyokae", "uyoloea", "okwc", "yukoea", /* 1 2 3 4 5 */
  "yuaeok", "uye", "oyuaeok", "eyuko", "yuaey",   /* 6 7 8 9 0 */
};

#define MAXCHARS 80
#define MAXSTROKES 10
#define MAXSEGMENTS (MAXCHARS * MAXSTROKES * 2)

int t_sSegment[ MAXSEGMENTS ];
int t_iCurSeg = 0;
int t_iLastSeg = 0;
int t_iSegRate = 1;
int t_iCharIndex[ MAXCHARS ];
int t_iChars;

void text_render()
{
  int mData[ MAXSEGMENTS ];
  int i;

  t_push();
    t_apply( mData, t_sSegment, t_iCurSeg );
    for( i = 0; i < t_iChars; i++ )
    {
      int* pStrokeStart = &mData[ t_iCharIndex[ i ] ];
      int iStrokeSize = t_iCharIndex[ i+1 ] - t_iCharIndex[ i ];
      r_openpoly( pStrokeStart, iStrokeSize );
    }
  t_pop();
}

void text_animate()
{
  if( t_iCurSeg < t_iLastSeg )
  {
    t_iCurSeg += t_iSegRate;
  }
}

int text_busy()
{
  return (t_iCurSeg != t_iLastSeg);
}

/************************************/

void text_setsegments( char* szText )
{
  int i;

  strupr( szText );
  t_iChars = strlen( szText );

  for( i = 0; i < t_iChars; i++ )
  {
    char* pData = 0;
    int iSegStart = t_iLastSeg;
    char c0 = szText[ i ];

    if( c0 >= 'A' && c0 <= 'Z' )
      pData = t_szLetterData[ c0 - 'A' ];
    else if( c0 >= '0' && c0 <= '9' )
      pData = t_szNumberData[ c0 - '1' ];
    else
      pData = "ln"; /* dash */

    t_iCharIndex[ i ] = t_iLastSeg;
    d_55grid( t_sSegment, MAXSEGMENTS, &t_iLastSeg, pData );

    while( iSegStart < t_iLastSeg )
    {
      t_sSegment[ iSegStart ] += i * 7;
      iSegStart += 2;
    }
  }

  t_iCharIndex[ t_iChars ] = t_iLastSeg;
}

/************************************/

void text_set( char* szText )
{
  t_iLastSeg = 0;
  text_setsegments( szText );
  t_iCurSeg = t_iLastSeg;
}

/************************************/

void text_setspeed( int isps )
{
  t_iCurSeg = 0;
  t_iSegRate = isps;
}

