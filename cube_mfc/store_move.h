//
// STORE_MOVE.H
// WMoveStore object
// Stores dynamic number of cube rotations
//
// This software is licensed/distributed under the GPL 
// WITHOUT ANY WARRANTY - see gpl.license.txt for details
// Written by Ross Wolin/Mystic Industries - Copyright 2003
//


#ifndef STORE_MOVE_H
#define STORE_MOVE_H

#include "util.h"
#include "arr_thing.h"



typedef enum {X_AXIS, Y_AXIS, Z_AXIS} AXIS;


//Move storage structure
class MOVEINFO
{
public:
   AXIS  nAxis;
   INT8  nSection;      /* -1, 0, or 1*/
   BOOL  bCW;

public:
   MOVEINFO()  {};
   MOVEINFO(AXIS  nAxis, INT8  nSection, BOOL bCW);
};




class WMoveStore : public  WThingArray<MOVEINFO>
{
public:
   BOOL Add(AXIS nAxis, INT8 nSection, BOOL bCW);
   void serialize(CArchive& ar, WORD wFileFormat);
};


#endif //STORE_MOVE_H

