//
// SOUND.H
// Windows dependent sound stuff
//
// This software is licensed/distributed under the GPL 
// WITHOUT ANY WARRANTY - see gpl.license.txt for details
// Written by Ross Wolin/Mystic Industries - Copyright 2003
//


#ifndef SOUND_H
#define SOUND_H

typedef enum {
   ST_XROTATE,
   ST_YROTATE,
   ST_ZROTATE,
} SOUNDTYPE;


void playSound(SOUNDTYPE st);

#endif //SOUND_H
