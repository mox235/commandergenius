/*
 * CMessageBoxVort.cpp
 *
 *  Created on: 30.03.2011
 *      Author: gerstrong
 */

#include "CMessageBoxVort.h"
#include "sdl/CVideoDriver.h"
#include "sdl/CInput.h"
#include "graphics/CGfxEngine.h"

CMessageBoxVort::CMessageBoxVort(const std::string& Text, bool lower, bool keymsg, bool leftbound) :
CMessageBox(Text, lower, keymsg, leftbound),
m_twirltimer(0),
m_twirlframe(0)
{
	std::string buf;
	if(m_keymsg)
	{
		m_Lines.push_back("");
		buf = "Press any KEY/BUTTON  ";
		if( m_text_width<buf.size() )
			m_text_width=buf.size();
		else
		{
			size_t trail = m_text_width-buf.size();
			buf.insert((size_t)0,trail,' ');
		}
		m_Lines.push_back(buf);
	}

	// try to center that dialog box
	m_gamerect = g_pVideoDriver->getGameResolution();

	int h = m_gamerect.h;
	m_gamerect.x = m_gamerect.w/2;
	m_gamerect.y = m_gamerect.h/2;

	m_gamerect.h = (m_Lines.size()+2)*8;
	m_gamerect.w = (m_text_width+2)*8;

	m_gamerect.x -= m_gamerect.w/2;

	if(!lower)
		m_gamerect.y -= m_gamerect.h/2;
	else
		m_gamerect.y = h-m_gamerect.h;

	if(leftbound) m_gamerect.x = 0;


	m_gamerect.h /= 8;
	m_gamerect.w /= 8;

	mp_DlgFrame = new CDlgFrame(m_gamerect.x, m_gamerect.y,
								m_gamerect.w, m_gamerect.h, DLG_THEME_VORTICON);
}

// This function is used in your Ship need those parts.
// This special Messagebox can hold up to 4 images
void CMessageBoxVort::addTileAt(Uint16 tile, Uint16 x, Uint16 y)
{
	TileHolder tileholder;

	tileholder.tile = tile;
	tileholder.x = x;
	tileholder.y = y;

	m_Tiles.push_back(tileholder);
}

#define TWIRL_TIME	5
void CMessageBoxVort::process()
{
	SDL_Surface *sfc = g_pVideoDriver->mp_VideoEngine->getFGLayerSurface();

	// Look, if somebody pressed a button, and close this dialog!
	if(g_pInput->getPressedAnyCommand()) {
		m_mustclose = true;
		return;
	}

	// Draw the empty Dialog Box
	mp_DlgFrame->draw(sfc);

	// Draw the Text on our surface
	for( size_t i=0 ; i<m_Lines.size() ; i++)
		g_pGfxEngine->getFont(0).drawFont(sfc, m_Lines[i], m_gamerect.x+8, m_gamerect.y+((i+1)*8) );

	// Draw additional tiles on the surface if any where defined
	for(size_t i=0 ; i<m_Tiles.size() ; i++) {
		g_pGfxEngine->getTileMap(1).drawTile(sfc, m_gamerect.x+m_Tiles[i].x,
								m_gamerect.y+m_Tiles[i].y, m_Tiles[i].tile);
	}

	if(m_keymsg)
	{
		// Draw the twirl
		if( m_twirltimer >= TWIRL_TIME )
		{
			m_twirltimer = 0;
			if(m_twirlframe >= 7)
				m_twirlframe=0;
			else
				m_twirlframe++;
		}
		else m_twirltimer++;
		g_pGfxEngine->getCursor()->draw( sfc, m_twirlframe,
				mp_DlgFrame->m_x+(mp_DlgFrame->m_w-16),
				mp_DlgFrame->m_y+(mp_DlgFrame->m_h-16));
	}
}
