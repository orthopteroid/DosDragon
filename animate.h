/* Crusty old DOS VGA dragon from 2001 by orthopteroid@gmail.com built with TC++. public domain license. */

#ifndef _animate_h_
#define _animate_h_

#include <math.h>

#include "platform.h"

/* in-range signaller */
#define RANGE_SIG0(l, v, u) ( (l < v && v < u) ? 1 : 0 )
#define RANGE_SIG(a, v, b) ( a < b ? RANGE_SIG0(a,v,b) : RANGE_SIG0(b,v,a) )

/* in-range clamper */
#define RANGE_CLAMP0(l, v, u) ( v > u ? u : ( v < l ? l : v ) )
#define RANGE_CLAMP(a, v, b) ( a < b ? RANGE_CLAMP0(a,v,b) : RANGE_CLAMP0(b,v,a) )

/*****************/

struct saw_state
{
  int psign;
  double vps,sv,ev;
  double v;
};

#define SAW_DECLARE(s) struct saw_state s
#define SAW_INIT(s,_vps,_sv,_ev) \
  { \
    s.psign = ( _sv < _ev ? 1 : 0 ); \
    s.vps = _vps; \
    s.sv = _sv; \
    s.ev = _ev; \
    s.v = 0; \
  }
#define SAW_TICK( t, s ) \
  { \
    if( t.c > t.l ) \
    { \
      s.v = ( RANGE_SIG( s.sv, s.v, s.ev ) ? s.v : s.sv ); \
      s.v += s.vps * ( s.psign ? 1 : -1 ) * fabs(t.c - t.l); \
      s.v = RANGE_CLAMP( s.sv, s.v, s.ev ); \
    } \
  }

/*****************/

struct bounce_state
{
  int psign;
  double vps,sv,ev;
  double v;
};

#define BOUNCE_DECLARE(s) struct bounce_state s
#define BOUNCE_INIT(s,_vps,_sv,_ev) \
  { \
    s.psign = ( _sv < _ev ? 1 : 0 ); \
    s.vps = _vps; \
    s.sv = _sv; \
    s.ev = _ev; \
    s.v = 0; \
  }
#define BOUNCE_TICK( t, s ) \
  { \
    if( t.c > t.l ) \
    { \
      s.psign = ( RANGE_SIG( s.sv, s.v, s.ev ) ? s.psign : !s.psign ); \
      s.v += s.vps * ( s.psign ? 1 : -1 ) * fabs(t.c - t.l); \
      s.v = RANGE_CLAMP( s.sv, s.v, s.ev ); \
    } \
  }

#endif /* _animate_h_ */
