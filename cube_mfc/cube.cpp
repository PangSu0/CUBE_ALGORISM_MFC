//
// CUBE.CPP
// Rubik's cube object
// A complete cube is a collection of 27 "pieces"
//
// This software is licensed/distributed under the GPL 
// WITHOUT ANY WARRANTY - see gpl.license.txt for details
// Written by Ross Wolin/Mystic Industries - Copyright 2003
//



#include "stdafx.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
#include <float.h>

#include "cube.h"
#include "vec2f.h"
#include "dlg_preferences.h"
#include "sound.h"



#define ALMOST_ZERO   1e-6f        //Error tolerance for float comparisons, etc




WCube::WCube(CWnd* pWnd)
: m_pWnd(pWnd)
{
   memset(m_pPieces, 0, sizeof(WCubePiece*)*3*3*3);    //Start w/ NULL pointers for all pieces
}



/*virtual*/ WCube::~WCube()
{
   for (int x=0; x<3; x++) {
      for (int y=0; y<3; y++) {
         for (int z=0; z<3; z++) {
            if (m_pPieces[x][y][z])
               delete m_pPieces[x][y][z];
         }
      }
   }
}



void WCube::init(void)
{
   reset();
}



void WCube::draw(void)
{
   int x,y,z;

   for (x=-1; x<=1; x++) {
      for (y=-1; y<=1; y++) {
         for (z=-1; z<=1; z++) {
            getPiece(x,y,z)->draw(x,y,z);
         }
      }
   }
}



void WCube::reset(void)
{
   for (int x=0; x<3; x++) {
      for (int y=0; y<3; y++) {
         for (int z=0; z<3; z++) {
            if (m_pPieces[x][y][z])
               delete m_pPieces[x][y][z];

            m_pPieces[x][y][z] = new WCubePiece(BYTEVEC(x-1, y-1, z-1));
         }
      }
   }

   m_moves.RemoveAll();
}





//Rotate a section of the cube based on a mouse move and the current GL settings.
//This function manipulates the cube object, but is also fairly GL aware.
//Returns TRUE if a section rotated (and cube needs to be redrawn), 
//FALSE otherwise
BOOL WCube::rotate(GLdouble* mxProjection, GLdouble* mxModel, GLint* nViewPort, 
                   CPoint& wndSize, CPoint& ptMouseWnd, CPoint& ptLastMouseWnd)
{
   //Turn y mouse coordinate "upside down" so it matches viewport coordinate system
   CPoint ptMouse(ptMouseWnd.x, wndSize.y-ptMouseWnd.y);
   CPoint ptLastMouse(ptLastMouseWnd.x, wndSize.y-ptLastMouseWnd.y);

   //First figure out if we're on the cube or not.

   //Project all the corners of the cube onto the screen cooridinants and 
   //store the corners in a local array.   The cube to corner number orientation goes with the 
   //diagram in my notebook.  
   PT3D vCubeCorner[8];

   //"Front"
   gluProject( 1.5, 1.5, 1.5, mxModel, mxProjection, nViewPort, &vCubeCorner[0].x, &vCubeCorner[0].y, &vCubeCorner[0].z);
   gluProject( 1.5,-1.5, 1.5, mxModel, mxProjection, nViewPort, &vCubeCorner[1].x, &vCubeCorner[1].y, &vCubeCorner[1].z);
   gluProject(-1.5,-1.5, 1.5, mxModel, mxProjection, nViewPort, &vCubeCorner[2].x, &vCubeCorner[2].y, &vCubeCorner[2].z);
   gluProject(-1.5, 1.5, 1.5, mxModel, mxProjection, nViewPort, &vCubeCorner[3].x, &vCubeCorner[3].y, &vCubeCorner[3].z);

   //"Back"
   gluProject( 1.5, 1.5,-1.5, mxModel, mxProjection, nViewPort, &vCubeCorner[4].x, &vCubeCorner[4].y, &vCubeCorner[4].z);
   gluProject( 1.5,-1.5,-1.5, mxModel, mxProjection, nViewPort, &vCubeCorner[5].x, &vCubeCorner[5].y, &vCubeCorner[5].z);
   gluProject(-1.5,-1.5,-1.5, mxModel, mxProjection, nViewPort, &vCubeCorner[6].x, &vCubeCorner[6].y, &vCubeCorner[6].z);
   gluProject(-1.5, 1.5,-1.5, mxModel, mxProjection, nViewPort, &vCubeCorner[7].x, &vCubeCorner[7].y, &vCubeCorner[7].z);




   //Find the min/max X and Y to do a rough bounding box test.
   float xMin=FLT_MAX, yMin=FLT_MAX;
   float xMax=FLT_MIN, yMax=FLT_MIN;
   for (int i=0; i<8; i++) {
      xMin = min(xMin, (float)vCubeCorner[i].x);
      yMin = min(yMin, (float)vCubeCorner[i].y);
      xMax = max(xMax, (float)vCubeCorner[i].x);
      yMax = max(yMax, (float)vCubeCorner[i].y);
   }


   //Check if point was outside rough test and return if it was.
   if (!(xMin <= ptLastMouse.x && ptLastMouse.x <= xMax
         && yMin <= ptLastMouse.y && ptLastMouse.y <= yMax))
   {
      return FALSE;        //Failed rough bounding box test
   }



   //Build an array of the corners for each side of the cube.
   //(initializers must be in same order as WPiece::SIDE enum)
   //0->1 must be positive direction, X for side piece
   //1->2 must be positive direction, Y for side piece
   PT3D vCorner[6][4] = {
      {vCubeCorner[5], vCubeCorner[1], vCubeCorner[0], vCubeCorner[4]},    //Right
      {vCubeCorner[6], vCubeCorner[2], vCubeCorner[3], vCubeCorner[7]},    //Left
      {vCubeCorner[7], vCubeCorner[4], vCubeCorner[0], vCubeCorner[3]},    //Top
      {vCubeCorner[6], vCubeCorner[5], vCubeCorner[1], vCubeCorner[2]},    //Bottom
      {vCubeCorner[2], vCubeCorner[1], vCubeCorner[0], vCubeCorner[3]},    //Front
      {vCubeCorner[6], vCubeCorner[5], vCubeCorner[4], vCubeCorner[7]},    //Back
   };

   double fMinZ = DBL_MAX;
   double fTmp;
   SIDE nSide = (SIDE)-1;

   //Test each side for a hit.  If it's a hit and it's closer to the front than the last
   //hit, store which side it was and update the z distance.
   for (int i=0; i<6; i++) {
      if (poly4InsideTest(vCorner[i], ptLastMouse)) {
         fTmp = min(min(min(vCorner[i][0].z, vCorner[i][1].z), vCorner[i][2].z), vCorner[i][3].z);
         if (fTmp < fMinZ) {
            nSide = (SIDE)i;
            fMinZ = fTmp;
         }
      }
   }

   if (nSide==-1) 
      return FALSE;        //Missed all the sides.


   #ifdef _DEBUG
   {
      char* sz;
      switch (nSide) {
         case SD_FRONT:    sz="FRONT";    break;
         case SD_BACK:     sz="BACK";     break;
         case SD_TOP:      sz="TOP";      break;
         case SD_BOTTOM:   sz="BOTTOM";   break;
         case SD_LEFT:     sz="LEFT";     break;
         case SD_RIGHT:    sz="RIGHT";    break;
      }
      TRACE("Hit %s side\n", sz);
   }
   #endif //_DEBUG



   //Figured out which side we hit, now get up and right vector for side, and figure out
   //which way we're trying to rotate the section

   //For each side's corners, when the axis is projected on the screen in a normal rotation,
   //vector from corner 1 to 0 is "X axis" 
   //and vector from corner 2 to 1 is "Y axis"
   Vec2f vX(
      (float)(vCorner[nSide][1].x - vCorner[nSide][0].x),
      (float)(vCorner[nSide][1].y - vCorner[nSide][0].y)
   );

   Vec2f vY(
      (float)(vCorner[nSide][2].x - vCorner[nSide][1].x),
      (float)(vCorner[nSide][2].y - vCorner[nSide][1].y)
   );


   //Compute mouse vector
   Vec2f vMouse(
      (float)(ptMouse.x - ptLastMouse.x), 
      (float)(ptMouse.y - ptLastMouse.y)
   );


   //If the mouse didn't move at least this far, discard the move.   
   if (vMouse.getLength() < g_fMinMouseLength) 
      return FALSE;


   //Figure out what "direction" we were moving, by checking the angle between the mouse vector and the 
   //X/Y vector for this cube side.
   vX.normalize();
   vY.normalize();
   vMouse.normalize();

   float xDiff = Vec2f::getAngle(vX, vMouse);
   float yDiff = Vec2f::getAngle(vY, vMouse);
   float minDiff = (float)min(min(min(fabs(xDiff), fabs(yDiff)), fabs(xDiff-180)), fabs(yDiff-180));

   //If the angle to the closest rotation direction doesn't beat the angle limit, then discard the gesture
   if (minDiff > g_fMaxMouseAngle) 
      return FALSE;        //No drunken Rubik's cubing!


   //Figure out which section we were trying to rotate.   
   //What we do is dependent on which side we're looking at too.
   INT8 nSection;

   minDiff += ALMOST_ZERO;       //Add a fudge factor for comparison against the original values.
                                 //(without this, sometimes comparisons fail)
   if (fabs(xDiff) <= minDiff) {
      nSection = getYsection(vCorner[nSide], ptLastMouse);
      switch (nSide) {
         case SD_FRONT:    rotateYSection(nSection, TRUE, TRUE, TRUE);     break;
         case SD_BACK:     rotateYSection(nSection, FALSE, TRUE, TRUE);    break;
         case SD_LEFT:     rotateYSection(nSection, TRUE, TRUE, TRUE);     break;
         case SD_RIGHT:    rotateYSection(nSection, FALSE, TRUE, TRUE);    break;
         case SD_TOP:      rotateZSection(nSection, FALSE, TRUE, TRUE);     break;
         case SD_BOTTOM:   rotateZSection(nSection, TRUE, TRUE, TRUE);    break;
      }
   }
   else if (fabs(xDiff-180) <= minDiff) {
      nSection = getYsection(vCorner[nSide], ptLastMouse);
      switch (nSide) {
         case SD_FRONT:    rotateYSection(nSection, FALSE, TRUE, TRUE);    break;   
         case SD_BACK:     rotateYSection(nSection, TRUE, TRUE, TRUE);     break;
         case SD_LEFT:     rotateYSection(nSection, FALSE, TRUE, TRUE);    break;
         case SD_RIGHT:    rotateYSection(nSection, TRUE, TRUE, TRUE);     break;
         case SD_TOP:      rotateZSection(nSection, TRUE, TRUE, TRUE);    break;
         case SD_BOTTOM:   rotateZSection(nSection, FALSE, TRUE, TRUE);     break;
      }
   }
   else if (fabs(yDiff) <= minDiff) {
      nSection = getXsection(vCorner[nSide], ptLastMouse);
      switch (nSide) {
         case SD_FRONT:    rotateXSection(nSection, FALSE, TRUE, TRUE);    break;
         case SD_BACK:     rotateXSection(nSection, TRUE,  TRUE, TRUE);    break;
         case SD_LEFT:     rotateZSection(nSection, FALSE, TRUE, TRUE);     break;
         case SD_RIGHT:    rotateZSection(nSection, TRUE, TRUE, TRUE);    break;
         case SD_TOP:      rotateXSection(nSection, TRUE,  TRUE, TRUE);    break;
         case SD_BOTTOM:   rotateXSection(nSection, FALSE, TRUE, TRUE);    break;
      }
   }
   else if (fabs(yDiff-180) <= minDiff) {
      nSection = getXsection(vCorner[nSide], ptLastMouse);
      switch (nSide) {
         case SD_FRONT:    rotateXSection(nSection, TRUE,  TRUE, TRUE);    break;
         case SD_BACK:     rotateXSection(nSection, FALSE, TRUE, TRUE);    break;
         case SD_LEFT:     rotateZSection(nSection, TRUE, TRUE, TRUE);    break;
         case SD_RIGHT:    rotateZSection(nSection, FALSE, TRUE, TRUE);     break;
         case SD_TOP:      rotateXSection(nSection, FALSE, TRUE, TRUE);    break;
         case SD_BOTTOM:   rotateXSection(nSection, TRUE,  TRUE, TRUE);    break;
      }
   }
   else {
      ASSERT(FALSE);    //We already tossed out gestures that were too far off the 
      return FALSE;     //rotation direction, so this shouldn't happen!
   }

   return TRUE;      
}




void WCube::animateRotation(WCubePiece* piece[], int ctPieces, Vec3f v, float fAngle)
{
   int x;

   ASSERT(m_pWnd);

   if (g_nRotationSteps <= 1)
      return;

   for (UINT i=0; i<g_nRotationSteps; i++) {
      float fRotAngle = fAngle * i/g_nRotationSteps;

      for (int x=0; x<ctPieces; x++) 
         piece[x]->setRotation(fRotAngle, v);
      
      m_pWnd->Invalidate();
      m_pWnd->UpdateWindow();

      if (g_nRotationDelay)
         Sleep(g_nRotationDelay);
   }

   for (x=0; x<ctPieces; x++) 
      piece[x]->clrRotation();
}




//nSection is the offset along the axis (-1 to 1)
// and bCCW is TRUE for CW rotation, FALSE for CCW
//bRecord=TRUE if we should save this rotation in the move list
void WCube::rotateXSection(INT8 nSection, BOOL bCW, BOOL bAnimate, BOOL bRecord)
{
   WCubePiece* pTmp;
   int i;

   ASSERT(-1 <= nSection && nSection <= 1);

   playSound(ST_XROTATE);

   WCubePiece* pieces[] = {
      getPiece(nSection, 1, 1),     //Corner pieces
      getPiece(nSection,-1, 1),
      getPiece(nSection,-1,-1),
      getPiece(nSection, 1,-1),
      getPiece(nSection, 1, 0),     //Side pieces
      getPiece(nSection, 0, 1),
      getPiece(nSection,-1, 0),
      getPiece(nSection, 0,-1),
      getPiece(nSection, 0, 0),     //Center piece
   };

   float fAngle = bCW ? 90.0f : -90.0f;
   if (bAnimate) 
      animateRotation(pieces, ELEMENTS_OF(pieces), Vec3f(1,0,0), fAngle);


   //Rotate pieces for viewer
   for (i=0; i<ELEMENTS_OF(pieces); i++) 
      pieces[i]->rotateX(bCW);


   //"Rotate" pieces in cube structure
   if (bCW) {
      pTmp = getPiece(nSection, 1, 1);
      getPiece(nSection, 1, 1) = getPiece(nSection, 1,-1);
      getPiece(nSection, 1,-1) = getPiece(nSection,-1,-1);
      getPiece(nSection,-1,-1) = getPiece(nSection,-1, 1);
      getPiece(nSection,-1, 1) = pTmp;

      pTmp = getPiece(nSection, 1, 0);
      getPiece(nSection, 1, 0) = getPiece(nSection, 0,-1);
      getPiece(nSection, 0,-1) = getPiece(nSection,-1, 0);
      getPiece(nSection,-1, 0) = getPiece(nSection, 0, 1);
      getPiece(nSection, 0, 1) = pTmp;
   }
   else {
      //"Rotate" pieces in cube structure
      pTmp = getPiece(nSection, 1, 1);
      getPiece(nSection, 1, 1) = getPiece(nSection,-1, 1);
      getPiece(nSection,-1, 1) = getPiece(nSection,-1,-1);
      getPiece(nSection,-1,-1) = getPiece(nSection, 1,-1);
      getPiece(nSection, 1,-1) = pTmp;

      pTmp = getPiece(nSection, 1, 0);
      getPiece(nSection, 1, 0) = getPiece(nSection, 0, 1);
      getPiece(nSection, 0, 1) = getPiece(nSection,-1, 0);
      getPiece(nSection,-1, 0) = getPiece(nSection, 0,-1);
      getPiece(nSection, 0,-1) = pTmp;
 
   }      

   if (bRecord)
      m_moves.Add(X_AXIS, nSection, bCW);
}



//nSection is the offset along the axis (-1 to 1)
// and bCW is TRUE for CW rotation, FALSE for CCW
//bRecord=TRUE if we should save this rotation in the move list
void WCube::rotateYSection(INT8 nSection, BOOL bCW, BOOL bAnimate, BOOL bRecord)
{
   WCubePiece* pTmp;
   int i;

   ASSERT(-1 <= nSection && nSection <= 1);

   playSound(ST_YROTATE);

   WCubePiece* pieces[] = {
      getPiece( 1, nSection, 1),    //Corner pieces
      getPiece(-1, nSection, 1),
      getPiece(-1, nSection,-1),
      getPiece( 1, nSection,-1),
      getPiece( 1, nSection, 0),    //Side pieces
      getPiece( 0, nSection, 1),
      getPiece(-1, nSection, 0),
      getPiece( 0, nSection,-1),
      getPiece( 0, nSection, 0)     //Center piece
   };

   float fAngle = bCW ? 90.0f : -90.0f;
   if (bAnimate) 
      animateRotation(pieces, ELEMENTS_OF(pieces), Vec3f(0,1,0), fAngle);


   //Rotate pieces for viewer
   for (i=0; i<ELEMENTS_OF(pieces); i++) 
      pieces[i]->rotateY(bCW);


   if (bCW) {
      pTmp = getPiece(1, nSection, 1);
      getPiece( 1, nSection, 1) = getPiece(-1, nSection, 1);
      getPiece(-1, nSection, 1) = getPiece(-1, nSection,-1);
      getPiece(-1, nSection,-1) = getPiece( 1, nSection,-1);
      getPiece( 1, nSection,-1) = pTmp;

      pTmp = getPiece(1, nSection, 0);
      getPiece( 1, nSection, 0) = getPiece( 0, nSection, 1);
      getPiece( 0, nSection, 1) = getPiece(-1, nSection, 0);
      getPiece(-1, nSection, 0) = getPiece( 0, nSection,-1);
      getPiece( 0, nSection,-1) = pTmp;
   } 
   else {
      pTmp = getPiece(1, nSection, 1);
      getPiece( 1, nSection, 1) = getPiece( 1, nSection,-1);
      getPiece( 1, nSection,-1) = getPiece(-1, nSection,-1);
      getPiece(-1, nSection,-1) = getPiece(-1, nSection, 1);
      getPiece(-1, nSection, 1) = pTmp;

      pTmp = getPiece(1, nSection, 0);
      getPiece( 1, nSection, 0) = getPiece( 0, nSection,-1);
      getPiece( 0, nSection,-1) = getPiece(-1, nSection, 0);
      getPiece(-1, nSection, 0) = getPiece( 0, nSection, 1);
      getPiece( 0, nSection, 1) = pTmp;
   }      

   if (bRecord)
      m_moves.Add(Y_AXIS, nSection, bCW);
}



//nSection is the offset along the axis (-1 to 1)
// and bCW is TRUE for CW rotation, FALSE for CCW
//bRecord=TRUE if we should save this rotation in the move list
void WCube::rotateZSection(INT8 nSection, BOOL bCW, BOOL bAnimate, BOOL bRecord)
{
   WCubePiece* pTmp;
   int i;

   ASSERT(-1 <= nSection && nSection <= 1);

   playSound(ST_ZROTATE);

   WCubePiece* pieces[] = {
      getPiece( 1, 1, nSection),          //Corner pieces
      getPiece(-1, 1, nSection),
      getPiece(-1,-1, nSection),
      getPiece( 1,-1, nSection),
      getPiece( 1, 0, nSection),          //Side pieces
      getPiece( 0, 1, nSection), 
      getPiece(-1, 0, nSection),
      getPiece( 0,-1, nSection),
      getPiece( 0, 0, nSection)           //Center piece
   };

   float fAngle = bCW ? 90.0f : -90.0f;
   if (bAnimate) 
      animateRotation(pieces, ELEMENTS_OF(pieces), Vec3f(0,0,1), fAngle);


   //Rotate pieces for viewer
   for (i=0; i<ELEMENTS_OF(pieces); i++) 
      pieces[i]->rotateZ(bCW);


   //"Rotate" pieces in cube structure
   if (bCW) {
      pTmp = getPiece( 1, 1, nSection);
      getPiece( 1, 1, nSection) = getPiece( 1,-1, nSection);
      getPiece( 1,-1, nSection) = getPiece(-1,-1, nSection);
      getPiece(-1,-1, nSection) = getPiece(-1, 1, nSection);
      getPiece(-1, 1, nSection) = pTmp;

      pTmp = getPiece( 1, 0, nSection);
      getPiece( 1, 0, nSection) = getPiece( 0,-1, nSection);
      getPiece( 0,-1, nSection) = getPiece(-1, 0, nSection);
      getPiece(-1, 0, nSection) = getPiece( 0, 1, nSection);
      getPiece( 0, 1, nSection) = pTmp;
   }      
   else {
      pTmp = getPiece( 1, 1, nSection);
      getPiece( 1, 1, nSection) = getPiece(-1, 1, nSection);
      getPiece(-1, 1, nSection) = getPiece(-1,-1, nSection);
      getPiece(-1,-1, nSection) = getPiece( 1,-1, nSection);
      getPiece( 1,-1, nSection) = pTmp;

      pTmp = getPiece( 1, 0, nSection);
      getPiece( 1, 0, nSection) = getPiece( 0, 1, nSection);
      getPiece( 0, 1, nSection) = getPiece(-1, 0, nSection);
      getPiece(-1, 0, nSection) = getPiece( 0,-1, nSection);
      getPiece( 0,-1, nSection) = pTmp;   
   }

   if (bRecord)
      m_moves.Add(Z_AXIS, nSection, bCW);
}



//Undo the last rotation that was put on the cube
void WCube::undoMove(void)
{
   MOVEINFO mi;

   if (m_moves.IsEmpty())
      return;
   
   mi = m_moves.GetLast();

   switch (mi.nAxis) {
      case X_AXIS:   rotateXSection(mi.nSection, !mi.bCW, TRUE, FALSE);    break;
      case Y_AXIS:   rotateYSection(mi.nSection, !mi.bCW, TRUE, FALSE);    break;
      case Z_AXIS:   rotateZSection(mi.nSection, !mi.bCW, TRUE, FALSE);    break;
   }

   m_moves.RemoveLast();
}



//////////////////////////////////////////////////////////////////////////
// Serialization stuff
//////////////////////////////////////////////////////////////////////////

void WCube::serialize(CArchive& ar, WORD wFileFormat)
{
   if (ar.IsStoring()) {
   }
   else {
   }

   //Whether storing or retreiving, call serialize for each piece in the cube
   for (int x=0; x<3; x++) {
      for (int y=0; y<3; y++) {
         for (int z=0; z<3; z++) {
            m_pPieces[x][y][z]->serialize(ar, wFileFormat);
         }
      }
   }

   m_moves.serialize(ar, wFileFormat);
}
