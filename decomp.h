/* Crusty old DOS VGA dragon from 2001 by orthopteroid@gmail.com built with TC++. public domain license. */

#ifndef _decomp_h_
#define _decomp_h_

void d_55grid( int ov[], int mv, int* pnv, char iv[] );
void d_127( int ov[], int mv, int* pnv, char iv[] );

void d_rawEncode( int* v, int nv, int* a, int na, char* szBuffer );
void d_rawScan( int* pnv, int* pna, char* szBuffer );
void d_rawDecode( int* v, int* a, char* szBuffer );

#endif /* _decomp_h_ */
