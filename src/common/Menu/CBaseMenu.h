/*
 * CBaseMenu.h
 *
 *  Created on: 28.11.2009
 *      Author: gerstrong
 */

#ifndef CBASEMENU_H_
#define CBASEMENU_H_

const int NO_SELECTION = -1;

#include "dialog/CDialog.h"

// Which Menu has to be shown?

enum menutypes{
	MAIN, NEW, OVERWRITE,
	CONTROLPLAYERS, STORY,
	HIGHSCORES, ABOUTCG,
	ABOUTID, ORDERING,
	LOAD, SAVE, START,
	DIFFICULTY, CONFIGURE,
	GRAPHICS, CONTROLS,
	AUDIO, OPTIONS,
	F1, HELP, ENDGAME, QUIT,
	BOUNDS, VOLUME, MENU_DEBUG,
	MODCONF
};

const unsigned int NUM_MENUS = MODCONF+1;

// Active means, when the player is playing, PASSIVE when the Player is not playing
enum menumodes{
	ACTIVE, PASSIVE
};

class CBaseMenu {
public:
	CBaseMenu(Uint8 dlg_theme);

	/*
	 *  \brief
	 * This function returns a string of character why will show whether the switch is on or off
	 * We use a function for that because sometimes when using special fontmaps it is not so clear
	 * how the string is formed
	 */
	std::string getSwitchString(const bool value);

	// Processes the stuff that the menus have in common
	virtual void processCommon();
	// Processes the stuff that are specific for the derived menu
	virtual void processSpecific() {}
	// Process more common stuff, like drawing or reading input of mp_Dialog
	virtual void postProcess();

	bool mustClose();

	virtual ~CBaseMenu();

protected:
	bool m_mustclose;
	int m_selection;
	CDialog *mp_Dialog;
	Uint8 m_dlg_theme;
	bool m_suspended;
	bool m_noenter;
};

#endif /* CBASEMENU_H_ */
