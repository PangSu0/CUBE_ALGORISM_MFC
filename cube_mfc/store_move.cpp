//
// STORE_MOVE.H
// WMoveStore object
// Stores dynamic number of cube rotations
//
// 
// This software is licensed/distributed under the GPL 
// WITHOUT ANY WARRANTY - see gpl.license.txt for details
// Written by Ross Wolin/Mystic Industries - Copyright 2003
//

#include "stdafx.h"
#include "store_move.h"



MOVEINFO::MOVEINFO(AXIS nAxis_, INT8  nSection_, BOOL bCW_)
: nAxis(nAxis_), nSection(nSection_), bCW(bCW_)
{
}



BOOL WMoveStore::Add(AXIS nAxis, INT8 nSection, BOOL bCW)
{
   return WThingArray<MOVEINFO>::Add(MOVEINFO(nAxis, nSection, bCW));
}


void WMoveStore::serialize(CArchive& ar, WORD wFileFormat)
{
   MOVEINFO mi;
   int nCount;

   if (ar.IsLoading()) {
      RemoveAll();               //Empty out previously stored moves

      if (wFileFormat < 2)       //If format does not support storing moves, we're done
         return;

      ar >> nCount;
      for (int i=0; i<nCount; i++) {
         ar.Read(&mi, sizeof(MOVEINFO));
         WThingArray<MOVEINFO>::Add(mi);
      }
   }
   else {                        //Always save in the most current format
      nCount = GetSize();
      ar << nCount;

      if (nCount==0)
         return;

      for (int i=0; i<nCount; i++)
         ar.Write(&ElementAt(i), sizeof(MOVEINFO));
   }    
}
