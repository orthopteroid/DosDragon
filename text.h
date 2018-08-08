/* Crusty old DOS VGA dragon from 2001 by orthopteroid@gmail.com built with TC++. public domain license. */

#ifndef _text_h_
#define _text_h_

void text_render();
void text_animate();

void text_set( char* szText );
void text_setspeed( int isps );

int text_busy();

extern char* t_szLetterData[];
extern char* t_szNumberData[];

#endif /* _text_h_ */
