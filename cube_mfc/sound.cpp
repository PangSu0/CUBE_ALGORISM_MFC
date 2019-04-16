//
// SOUNDS.CPP
// Windows dependent sound stuff
//
// This software is licensed/distributed under the GPL 
// WITHOUT ANY WARRANTY - see gpl.license.txt for details
// Written by Ross Wolin/Mystic Industries - Copyright 2003
//

#include "stdafx.h"
#include <mmsystem.h>
#include "sound.h"


void playSound(SOUNDTYPE st)
{
   PlaySound(NULL, NULL, 0);     //Stop any currently playing sound

   switch (st) {
      case ST_XROTATE:  PlaySound("sounds\\xrotate.wav",  NULL, SND_FILENAME|SND_ASYNC);  break;
      case ST_YROTATE:  PlaySound("sounds\\yrotate.wav",  NULL, SND_FILENAME|SND_ASYNC);  break;
      case ST_ZROTATE:  PlaySound("sounds\\zrotate.wav",  NULL, SND_FILENAME|SND_ASYNC);  break;
   }
}
