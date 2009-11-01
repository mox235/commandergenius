/*
 * CEGABitmap.cpp
 *
 *  Created on: 23.10.2009
 *      Author: gerstrong
 */

#include "CEGABitmap.h"

///
// Initialization Routine
///
CEGABitmap::CEGABitmap(SDL_Surface *pSurface, CBitmap *pBitmap) {
	mp_Surface = pSurface;
	mp_Bitmap = pBitmap;
}

///
// Process Routine
///
void CEGABitmap::process()
{
	mp_Bitmap->draw( mp_Surface, scrx, scry);
}

CEGABitmap::~CEGABitmap() {
	// TODO Auto-generated destructor stub
}