/*
 * CPlayerWM.cpp
 *
 *  Created on: 07.10.2009
 *      Author: gerstrong
 */

#include "CPlayer.h"
#include "../graphics/CGfxEngine.h"
#include "../sdl/CInput.h"
#include "../keen.h"

///
// Process Part
///

////
// Process the stuff of the player when playing on the world map
void CPlayer::processWorldMap()
{
    inhibitwalking = false;
    inhibitfall = false;

    StatusBox();

    ProcessInput();
    setWorldMapdir();

    setWMblockedlrud();
    //gamepdo_wm_AllowEnterLevel(cp, pCKP);

    Walking();
    WalkingAnimation();

    InertiaAndFriction_X();
    InertiaAndFriction_Y();

    if (m_episode==3)
    {
      AllowMountUnmountNessie();
    }

    selectFrameOnWorldMap();
}

// select proper player direction
void CPlayer::setWorldMapdir()
{
bool exception;
   dpadcount = 0;
   if (playcontrol[PA_X] < 0) { pdir = LEFT; dpadcount++; }
   if (playcontrol[PA_X] > 0) { pdir = RIGHT; dpadcount++; }
   if (playcontrol[PA_Y] < 0) { pdir = UP; dpadcount++; }
   if (playcontrol[PA_Y] > 0) { pdir = DOWN; dpadcount++; }

   exception = false;
   if ( pshowdir==UP && !playcontrol[PA_Y] < 0) exception = true;
   if ( pshowdir==DOWN && !playcontrol[PA_Y] > 0) exception = true;
   if ( pshowdir==LEFT && !playcontrol[PA_X] < 0) exception = true;
   if ( pshowdir==RIGHT && !playcontrol[PA_X] > 0) exception = true;
   if ((playcontrol[PA_Y] < 0) && (playcontrol[PA_Y] > 0)) pshowdir = DOWN;
   if ((playcontrol[PA_X] < 0) && (playcontrol[PA_X] > 0)) pshowdir = RIGHT;

   if (dpadcount==1 || dpadlastcount==0 || exception)
   {
     pshowdir = pdir;
   }
   dpadlastcount = dpadcount;
}

// set blockedl and blockedr...is Keen up against a solid object?
void CPlayer::setWMblockedlrud()
{
   blockedl = blockedr = false;
   blockedu = blockedd = false;

   // cheat: holding down TAB will turn off clipping. or if you are in godmode
   //if ((pCKP->Option[OPT_CHEATS].value && g_pInput->getHoldedKey(KTAB)) || player[cp].godmode) return;

   // R
   if (isWMSolid((x>>CSF)+8, (y>>CSF)+1, mp_levels_completed))
      { blockedr = true; }
   else if (isWMSolid((x>>CSF)+8, (y>>CSF)+8, mp_levels_completed))
      { blockedr = true; }
   else if (isWMSolid((x>>CSF)+8, (y>>CSF)+13, mp_levels_completed))
      { blockedr = true; }

   // L
   if (isWMSolid((x>>CSF)+0, (y>>CSF)+1, mp_levels_completed))
      { blockedl = true; }
   else if (isWMSolid((x>>CSF)+0, (y>>CSF)+8, mp_levels_completed))
      { blockedl = true; }
   else if (isWMSolid((x>>CSF)+0, (y>>CSF)+13, mp_levels_completed))
      { blockedl = true; }

   // U
   if (isWMSolid((x>>CSF)+1, (y>>CSF)-1, mp_levels_completed))
      { blockedu = true; }
   else if (isWMSolid((x>>CSF)+4, (y>>CSF)-1, mp_levels_completed))
      { blockedu = true; }
   else if (isWMSolid((x>>CSF)+7, (y>>CSF)-1, mp_levels_completed))
      { blockedu = true; }

   // D
   if (isWMSolid((x>>CSF)+1, (y>>CSF)+14, mp_levels_completed))
      { blockedd = true; }
   else if (isWMSolid((x>>CSF)+4, (y>>CSF)+14, mp_levels_completed))
      { blockedd = true; }
   else if (isWMSolid((x>>CSF)+7, (y>>CSF)+14, mp_levels_completed))
      { blockedd = true; }

}

void CPlayer::AllowEnterLevelonWM()
{
/*int lvl;
int x,y,i,o;
int destx, desty;
int telfrom, teldest, telsnap;

stLevelControl *p_levelcontrol;

// 3859 <-> 3889
// 3911 -> 3955
// 3877 <-> 3922

// 4032 -> 3842
// 3842 -> 3877
// 4063 -> 4094
// 4094 -> 4072
// 4072 -> 3980
// 3980 -> 4032 nosnap

// table of teleporters (mostly for ep3). player will teleport from
// a teleporter in a teleport_from entry, to the teleporter
// in the matching teleport_dest entry. Thus 3859 leads to 3889, etc.
// if the snap entry is 1 the screen will snap to center on the new
// position, otherwise it will smoothly scroll over to it
#define NUM_TPORTS    20
int teleport_from[NUM_TPORTS+1] = {4014,4032,3842,4063,4094,4072,3980,3859,3889,3911,3877,3922,3947,4025,3988,3955,41,38,0};
int teleport_dest[NUM_TPORTS+1] = {4072,3842,3877,4094,4072,3980,4032,3889,3859,3955,3922,3877,4025,3988,3911,3889,38,41,0};
int teleport_snap[NUM_TPORTS+1] = {1   ,1   ,1   ,1   ,1   ,1   ,0   ,0   ,0   ,1   ,1   ,1   ,1   ,0   ,1   ,1   ,0 ,0 ,0};

p_levelcontrol = &(pCKP->Control.levelcontrol);

    if (player[cp].hideplayer) return;  // don't execute function while teleporting

    if ((player[cp].playcontrol[PA_JUMP] || player[cp].playcontrol[PA_POGO]) && !player[cp].wm_lastenterstate)
    {   // trying to enter a level (or use a teleporter, etc)

        // get level/object marker beneath player
        x = (player[cp].x>>CSF)+4;
        y = (player[cp].y>>CSF)+4;
        lvl = map.objectlayer[x>>4][y>>4];
        if (!lvl)
        {
          y = (player[cp].y>>CSF)+8;
          lvl = map.objectlayer[x>>4][y>>4];
        }

        x = x >> 4 << 4;
        y = y >> 4 << 4;

        if (lvl)
        {
          // handle special map objects, and standard levels too

          // look through the teleporter tables and see if this is
          // a teleporter
          telfrom = 0;
          for(i=0;i<NUM_TPORTS;i++)
          {
            if (teleport_from[i]==lvl)
            { // we found it! it IS a teleporter!
              telfrom = teleport_from[i];
              teldest = teleport_dest[i];
              telsnap = teleport_snap[i];
              break;
            }
            else if (teleport_from[i]==0)
            { // nope. it's not a teleporter.
              break;
            }
          }

          // need to teleport somewhere?
          if (telfrom)
          {
             if (map_findobject(teldest, &destx, &desty))
             {
                o = spawn_object(x<<CSF,y<<CSF,OBJ_TELEPORTER);
                objects[o].ai.teleport.direction = TELEPORTING_OUT;
                objects[o].ai.teleport.destx = destx << 4 << CSF;
                objects[o].ai.teleport.desty = desty << 4 << CSF;
                objects[o].ai.teleport.whichplayer = cp;
                objects[o].ai.teleport.NoExitingTeleporter = 0;
                objects[o].ai.teleport.snap = telsnap;
                if (p_levelcontrol->episode==3)
                {
                  objects[o].ai.teleport.baseframe = TELEPORT_BASEFRAME_EP3;
                  objects[o].ai.teleport.idleframe = TELEPORT_IDLEFRAME_EP3;
                }
                else
                { // the teleporter in ep1
                  objects[o].ai.teleport.baseframe = TELEPORT_GRAY_BASEFRAME_EP1;
                  objects[o].ai.teleport.idleframe = TELEPORT_GRAY_IDLEFRAME_EP1;
                }
                g_pSound->playStereofromCoord(SOUND_TELEPORT, PLAY_NOW, objects[player[cp].useObject].scrx);

                player[cp].hideplayer = 1;
             }
          }
          else
          {
              switch(lvl)
              {
              case NESSIE_PATH: break;
              case NESSIE_PAUSE: break;
              case NESSIE_MOUNTPOINT: break;

              // ep1 bonus teleporter
              case LVLS_TELEPORTER_BONUS:
                o = spawn_object(x<<CSF,y<<CSF,OBJ_TELEPORTER);
                objects[o].ai.teleport.direction = TELEPORTING_OUT;
                objects[o].ai.teleport.destx = TELEPORT_BONUS_DESTX;
                objects[o].ai.teleport.desty = TELEPORT_BONUS_DESTY;
                objects[o].ai.teleport.whichplayer = cp;
                objects[o].ai.teleport.baseframe = TELEPORT_RED_BASEFRAME_EP1;
                objects[o].ai.teleport.idleframe = TELEPORT_RED_IDLEFRAME_EP1;
                objects[o].ai.teleport.NoExitingTeleporter = 1;
                objects[o].ai.teleport.snap = 1;
                player[cp].hideplayer = 1;
                break;

              case LVLS_SHIP:
                if (p_levelcontrol->episode==1)
                {
                  YourShipNeedsTheseParts(pCKP);
                }
                else
                {
                  ShipEp3(pCKP);
                }
                break;

              default:      // a regular level

            	p_levelcontrol->command = LVLC_CHANGE_LEVEL;
            	p_levelcontrol->chglevelto = (lvl & 0x7fff);
                endlevel(1, &(pCKP->Control.levelcontrol));
                g_pMusicPlayer->stop();
                g_pSound->playStereofromCoord(SOUND_ENTER_LEVEL, PLAY_NOW, objects[player[cp].useObject].scrx);

                break;

              } // end switch(level)
          } // end isn't a teleporter
        } // end if(lvl)
    } // end trying to enter a level

    player[cp].wm_lastenterstate = (player[cp].playcontrol[PA_JUMP] || player[cp].playcontrol[PA_POGO]);*/
}

bool CPlayer::isWMSolid(int xb, int yb, bool *levels_completed)
{
//int level_coordinates;
  // for map tiles solidl and solidr are always gonna be the same...
  // so we can get away with this.

  stTile *p_tiles = g_pGfxEngine->Tilemap->mp_tiles;

  if ( p_tiles[mp_map->at(xb, yb)].bleft ) return true;

  /*level_coordinates = mp_map->getObjectat(xb>>4, yb>>4);

  if (level_coordinates & 0x8000)
  {
	  if(levels_completed[level_coordinates & 0x7fff] && options[OPT_LVLREPLAYABILITY].value) // check if level is done, but can be replayed
		  return 0;

	  if(g_pInput->getHoldedKey(KTAB) && g_pInput->getHoldedKey(KSHIFT))
	  {
		  return 0;
	  }
	  else
	  {
		  return 1;
	  }
  }*/
  return false;
}

void CPlayer::selectFrameOnWorldMap()
{
    // select base frame for current direction
    if (pshowdir==RIGHT) playframe = PMAPRIGHTFRAME;
    else if (pshowdir==LEFT) playframe = PMAPLEFTFRAME;
    else if (pshowdir==UP) playframe = PMAPUPFRAME;
    else if (pshowdir==DOWN) playframe = PMAPDOWNFRAME;

    // episode 3 map frames start at 31, ep1&2 at 32
    if (m_episode==3) playframe--;

    // add in walk frame if walking
    if (pwalking) playframe += pwalkframe;
}

void CPlayer::AllowMountUnmountNessie()
{
/*int objmarker;
   if (!player[cp].mounted)
   {  // not mounted. find out if he's trying to mount
      // if the upper quarter of the map (mortimer's castle mount point)
      // he's trying to mount if he's on a NESSIE_MOUNTPOINT object marker
      // and he's going right and is blockedr and/or is going down and is
      // blockedd. in the bottom quarter (secret island mount point)
      // it's up and blockedu.

      // make sure he's on a mount point
      objmarker = getlevelat((player[cp].x>>CSF)+4, (player[0].y>>CSF)+9);
      if (objmarker != NESSIE_MOUNTPOINT) return;

      // is he trying to mount?
      if (player[cp].y>>CSF>>4 < map.ysize>>2)
      {  // at mortimer's castle mount point
          if ( (player[cp].playcontrol[PA_X] > 0 && player[cp].blockedr) ||
              (player[cp].playcontrol[PA_Y] > 0 && player[cp].blockedd))
          {
            // YES! if nessie is at that mount point, mount her!!
              MountNessieIfAvailable(cp);
          }
      }
      else
      {  // at secret island mount point
          if (player[cp].playcontrol[PA_Y] < 0 && player[cp].blockedu)
          {
             MountNessieIfAvailable(cp);
          }
      }
   }
   else
   {  // mounted. find out if he's trying to unmount.
      if (objects[NessieObjectHandle].ai.nessie.pausetimer)
      {  // nessie is paused
         if (objects[NessieObjectHandle].y>>CSF>>4 < map.ysize>>2)
         {  // nessie is at mortimer's castle mount point
            if (player[cp].playcontrol[PA_Y] < 0)
            {
               // unmount nessie
               objects[NessieObjectHandle].ai.nessie.mounted[cp] = 0;
               player[cp].mounted = 0;
               player[cp].hideplayer = 0;
            }
         }
         else if (objects[NessieObjectHandle].y>>CSF>>4 > map.ysize>>1)
         {  // nessie is at secret island mount point
            if (player[cp].playcontrol[PA_Y] > 0)
            {
               // unmount nessie
               objects[NessieObjectHandle].ai.nessie.mounted[cp] = 0;
               player[cp].mounted = 0;
               player[cp].hideplayer = 0;
               player[cp].y += (18<<CSF);
               player[cp].x += (8<<CSF);
            }
         }
      }

   }*/
}
