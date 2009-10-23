/*
 * spritedefines.h
 *
 *  Created on: 22.10.2009
 *      Author: gerstrong
 */

#ifndef SPRITEDEFINES_H_
#define SPRITEDEFINES_H_

// special sprites (they either weren't in the game originally,
// or are used for internal engine stuff).
#define BLANKSPRITE			(MAX_SPRITES-1)
#define DOOR_YELLOW_SPRITE	(MAX_SPRITES-2)			// opening door (yellow)
#define DOOR_BLUE_SPRITE	(MAX_SPRITES-3)
#define DOOR_GREEN_SPRITE	(MAX_SPRITES-4)
#define DOOR_RED_SPRITE		(MAX_SPRITES-5)
#define PT5000_SPRITE		(MAX_SPRITES-6)			// rising bonus pts (+5000)
#define PT1000_SPRITE		(MAX_SPRITES-7)
#define PT500_SPRITE		(MAX_SPRITES-8)
#define PT200_SPRITE		(MAX_SPRITES-9)
#define PT100_SPRITE		(MAX_SPRITES-10)
#define PT1UP_SPRITE		(MAX_SPRITES-11)		// rising 1up bonus
#define PTCARDY_SPRITE		(MAX_SPRITES-12)		// rising access card
#define PTCARDR_SPRITE		(MAX_SPRITES-13)
#define PTCARDG_SPRITE		(MAX_SPRITES-14)
#define PTCARDB_SPRITE		(MAX_SPRITES-15)
#define SHOTUP_SPRITE		(MAX_SPRITES-16)		// rising single shot (ep3)
#define ANKHUP_SPRITE		(MAX_SPRITES-17)		// rising single shot (ep3)
#define GUNUP_SPRITE		(MAX_SPRITES-18)		// rising ray gun
#define YORPSHIELD_SPRITE	(MAX_SPRITES-19)		// invincibility force field, used in editor and in "yorps have forcefields" mode
#define VERSION_SPRITE		(MAX_SPRITES-20)		// version text shown in lower-right corner at startup
#define DEMOBOX_SPRITE		(MAX_SPRITES-21)		// says "Demo"
#define OSD_LIVES_SPRITE	(MAX_SPRITES-22)
#define OSD_AMMO_SPRITE		(MAX_SPRITES-23)
#define OSD_YORPS_SPRITE	(MAX_SPRITES-24)
#define TITLE_LOGO1_SPRITE	(MAX_SPRITES-25)		// left half of the logo
#define TITLE_LOGO2_SPRITE	(MAX_SPRITES-26)		// right half of the logo
#define TITLE_FLOOR_SPRITE	(MAX_SPRITES-27)		// more stuff for the logo
#define TITLE_VORT_SPRITE   (MAX_SPRITES-28)
#define ARROWLR_SPRITE		(MAX_SPRITES-29)		// these are directional arrows used in the editor
#define ARROWUD_SPRITE		(MAX_SPRITES-30)
#define ARROWUR_SPRITE		(MAX_SPRITES-31)
#define ARROWUL_SPRITE		(MAX_SPRITES-32)
#define ARROWU_SPRITE		(MAX_SPRITES-33)
#define ARROWD_SPRITE		(MAX_SPRITES-34)
#define LAST_SPECIAL_SPRITE (MAX_SPRITES-35)


#endif /* DEFINES_H_ */