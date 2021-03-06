/*
 * CPassiveVort.h
 *
 *  Created on: 07.03.2010
 *      Author: gerstrong
 */

#ifndef CPASSIVEVORT_H_
#define CPASSIVEVORT_H_

#include "engine/CPassive.h"
#include "common/CBehaviorEngine.h"

namespace vorticon
{

class CPassiveVort : public CPassive
{
public:
	CPassiveVort();

	bool init(char mode = INTRO);

	void process();

	void cleanup();

private:
	CIntro *mp_IntroScreen;
	CTitle *mp_TitleScreen;
	CTextBox *mp_PressAnyBox;
	CTilemap *mp_Tilemap;
	CMap *mp_Map;
	stOption *mp_Option;

	SDL_Surface *mp_Scrollsurface;

	std::vector<CObject*> m_object;

	int m_textsize;
	bool m_GoDemo;
	bool m_hideobjects;
	char *m_text;
	bool m_RestartVideo;
};

}

#endif /* CPASSIVEVORT_H_ */
