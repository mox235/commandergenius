/*
 * CBullets.h
 *
 *  Created on: 01.12.2010
 *      Author: gerstrong
 *
 *  This handles the bullets that go over the place, whenever keen shoots one.
 */

#ifndef CBULLETS_H_
#define CBULLETS_H_

#include "common/CObject.h"

namespace galaxy
{

#define A_KEENSHOT_MOVING	0
#define A_KEENSHOT_IMPACT	4

/*ACTION keenshot_actions[] = {
   //Shot moving routine
	// AZ.1738
	{	174,	174,	1,	0,
		0,	6,	64,	64,
		I:0D6A,		NULL,		I:0E32,
		&keenshot_actions[A_KEENSHOT_MOVING+1]
	},
	// AZ.1756
	{	175,	175,	1,	0,
		0,	6,	64,	64,
		I:0D6A,		NULL,		I:0E32,
		&keenshot_actions[A_KEENSHOT_MOVING+2]
	},
	// AZ.1774
	{	176,	176,	1,	0,
		0,	6,	64,	64,
		I:0D6A,		NULL,		I:0E32,
		&keenshot_actions[A_KEENSHOT_MOVING+3]
	},
	// AZ.1792
	{	177,	177,	1,	0,
		0,	6,	64,	64,
		I:0D6A, 	NULL,		I:0E32,
		&keenshot_actions[A_KEENSHOT_MOVING]
	},

  // Shot impact routine
	// AZ.17B0
	{	178,	178,	0,	0,
		0,	12,	0,	0,
		NULL,		NULL,		G:176E,
		&keenshot_actions[A_KEENSHOT_IMPACT+1]
	},
	// AZ.17CE
	{	179,	179,	0,	0,
		0,	12,	0,	0,
		NULL,		NULL,		G:176E,
		NULL
	}
};
*/

class CBullets : public CObject {
public:
	CBullets(CMap *pmap, Uint32 x, Uint32 y, direction_t dir);
	void process();

private:
	Uint32 m_timer;
};

}

#endif /* CBULLETS_H_ */
