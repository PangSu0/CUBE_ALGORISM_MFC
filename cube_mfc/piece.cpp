//
// PIECE.CPP
// WCubePiece class
// Rubik's cube section/piece object
// A complete cube is made up of 27 pieces (although the center piece never moves.)
//
// This software is licensed/distributed under the GPL 
// WITHOUT ANY WARRANTY - see gpl.license.txt for details
// Written by Ross Wolin/Mystic Industries - Copyright 2003
//


#include "stdafx.h"
#include <gl/gl.h>
#include "piece.h"
#include "vec3f.h"








//Figure out which sides should be colored and which sides should
//be black for a 3x3 cube centered at origin, based on the piece
//position within the cube (specific init functionwhich has a little 
//knowledge of the cube container - not purist OO.)   
WCubePiece::WCubePiece(BYTEVEC posHome)
: m_posHome(posHome), m_fRotationAngle(0), m_vRotation(0,0,0)
{
   m_nSideColor[SD_RIGHT]  = posHome.x== 1 ? GREEN	: BLACK;
   m_nSideColor[SD_LEFT]   = posHome.x==-1 ? BLUE	: BLACK;
   m_nSideColor[SD_TOP]    = posHome.y== 1 ? WHITE	: BLACK;
   m_nSideColor[SD_BOTTOM] = posHome.y==-1 ? YELLOW	: BLACK;
   m_nSideColor[SD_FRONT]  = posHome.z== 1 ? ORANGE	: BLACK;
   m_nSideColor[SD_BACK]   = posHome.z==-1 ? RED	: BLACK;
}


WCubePiece::~WCubePiece()
{
}



void WCubePiece::draw(INT8 x, INT8 y, INT8 z)
{
   glPushMatrix();
      if (m_fRotationAngle)
         glRotatef(m_fRotationAngle, m_vRotation.x, m_vRotation.y, m_vRotation.z);

      glTranslatef((float)x, (float)y, (float)z);

      glBegin(GL_QUADS);
		   glColor3ub(MAKESIDECOLOR(SD_TOP));
		   glVertex3f( 0.5f, 0.5f,-0.5f);					// Top Right Of The Quad (Top)
		   glVertex3f(-0.5f, 0.5f,-0.5f);					// Top Left Of The Quad (Top)
		   glVertex3f(-0.5f, 0.5f, 0.5f);					// Bottom Left Of The Quad (Top)
		   glVertex3f( 0.5f, 0.5f, 0.5f);					// Bottom Right Of The Quad (Top)

		   glColor3ub(MAKESIDECOLOR(SD_BOTTOM));
		   glVertex3f( 0.5f,-0.5f, 0.5f);					// Top Right Of The Quad (Bottom)
		   glVertex3f(-0.5f,-0.5f, 0.5f);					// Top Left Of The Quad (Bottom)
		   glVertex3f(-0.5f,-0.5f,-0.5f);					// Bottom Left Of The Quad (Bottom)
		   glVertex3f( 0.5f,-0.5f,-0.5f);					// Bottom Right Of The Quad (Bottom)

		   glColor3ub(MAKESIDECOLOR(SD_FRONT));
		   glVertex3f( 0.5f, 0.5f, 0.5f);					// Top Right Of The Quad (Front)
		   glVertex3f(-0.5f, 0.5f, 0.5f);					// Top Left Of The Quad (Front)
		   glVertex3f(-0.5f,-0.5f, 0.5f);					// Bottom Left Of The Quad (Front)
		   glVertex3f( 0.5f,-0.5f, 0.5f);					// Bottom Right Of The Quad (Front)

		   glColor3ub(MAKESIDECOLOR(SD_BACK));
		   glVertex3f( 0.5f,-0.5f,-0.5f);					// Top Right Of The Quad (Back)
		   glVertex3f(-0.5f,-0.5f,-0.5f);					// Top Left Of The Quad (Back)
		   glVertex3f(-0.5f, 0.5f,-0.5f);					// Bottom Left Of The Quad (Back)
		   glVertex3f( 0.5f, 0.5f,-0.5f);					// Bottom Right Of The Quad (Back)

		   glColor3ub(MAKESIDECOLOR(SD_LEFT));
		   glVertex3f(-0.5f, 0.5f, 0.5f);					// Top Right Of The Quad (Left)
		   glVertex3f(-0.5f, 0.5f,-0.5f);					// Top Left Of The Quad (Left)
		   glVertex3f(-0.5f,-0.5f,-0.5f);					// Bottom Left Of The Quad (Left)
		   glVertex3f(-0.5f,-0.5f, 0.5f);					// Bottom Right Of The Quad (Left)

		   glColor3ub(MAKESIDECOLOR(SD_RIGHT));
		   glVertex3f( 0.5f, 0.5f,-0.5f);					// Top Right Of The Quad (Right)
		   glVertex3f( 0.5f, 0.5f, 0.5f);					// Top Left Of The Quad (Right)
		   glVertex3f( 0.5f,-0.5f, 0.5f);					// Bottom Left Of The Quad (Right)
		   glVertex3f( 0.5f,-0.5f,-0.5f);					// Bottom Right Of The Quad (Right)
      glEnd();

      glColor3ub(MAKECOLOR(BLACK));				   

      glBegin(GL_LINE_STRIP);
		   glVertex3f( 0.5f, 0.5f,-0.5f);					// Top Right Of The Quad (Top)
		   glVertex3f(-0.5f, 0.5f,-0.5f);					// Top Left Of The Quad (Top)
		   glVertex3f(-0.5f, 0.5f, 0.5f);					// Bottom Left Of The Quad (Top)
		   glVertex3f( 0.5f, 0.5f, 0.5f);					// Bottom Right Of The Quad (Top)
		   glVertex3f( 0.5f, 0.5f,-0.5f);					// Top Right Of The Quad (Top)
      glEnd();

      glBegin(GL_LINE_STRIP);
		   glVertex3f( 0.5f,-0.5f,-0.5f);					// Top Right Of The Quad (Top)
		   glVertex3f(-0.5f,-0.5f,-0.5f);					// Top Left Of The Quad (Top)
		   glVertex3f(-0.5f,-0.5f, 0.5f);					// Bottom Left Of The Quad (Top)
		   glVertex3f( 0.5f,-0.5f, 0.5f);					// Bottom Right Of The Quad (Top)
		   glVertex3f( 0.5f,-0.5f,-0.5f);					// Top Right Of The Quad (Top)
      glEnd();

      glBegin(GL_LINES);
		   glVertex3f( 0.5f, 0.5f,-0.5f);					// Top Right Of The Quad (Top)
		   glVertex3f( 0.5f,-0.5f,-0.5f);					// Top Right Of The Quad (Top)

		   glVertex3f(-0.5f, 0.5f,-0.5f);					// Top Left Of The Quad (Top)
		   glVertex3f(-0.5f,-0.5f,-0.5f);					// Top Left Of The Quad (Top)

		   glVertex3f(-0.5f, 0.5f, 0.5f);					// Bottom Left Of The Quad (Top)
		   glVertex3f(-0.5f,-0.5f, 0.5f);					// Bottom Left Of The Quad (Top)

		   glVertex3f( 0.5f, 0.5f, 0.5f);					// Bottom Right Of The Quad (Top)
		   glVertex3f( 0.5f,-0.5f, 0.5f);					// Bottom Right Of The Quad (Top)
      glEnd();

   glPopMatrix();
}





void WCubePiece::rotateX(BOOL bCW)
{
   SIDECOLOR nTmp;

   if (bCW) {
      nTmp                    = m_nSideColor[SD_TOP];
      m_nSideColor[SD_TOP]    = m_nSideColor[SD_BACK];
      m_nSideColor[SD_BACK]   = m_nSideColor[SD_BOTTOM];
      m_nSideColor[SD_BOTTOM] = m_nSideColor[SD_FRONT];
      m_nSideColor[SD_FRONT]  = nTmp;
   }
   else {
      nTmp                    = m_nSideColor[SD_TOP];
      m_nSideColor[SD_TOP]    = m_nSideColor[SD_FRONT];
      m_nSideColor[SD_FRONT]  = m_nSideColor[SD_BOTTOM];
      m_nSideColor[SD_BOTTOM] = m_nSideColor[SD_BACK];
      m_nSideColor[SD_BACK]   = nTmp;
   }
}



void WCubePiece::rotateY(BOOL bCW)
{
   SIDECOLOR nTmp;

   if (bCW) {
      nTmp                    = m_nSideColor[SD_FRONT];
      m_nSideColor[SD_FRONT]  = m_nSideColor[SD_LEFT];
      m_nSideColor[SD_LEFT]   = m_nSideColor[SD_BACK];
      m_nSideColor[SD_BACK]   = m_nSideColor[SD_RIGHT];
      m_nSideColor[SD_RIGHT]  = nTmp;
   }
   else {
      nTmp                    = m_nSideColor[SD_FRONT];
      m_nSideColor[SD_FRONT]  = m_nSideColor[SD_RIGHT];
      m_nSideColor[SD_RIGHT]  = m_nSideColor[SD_BACK];
      m_nSideColor[SD_BACK]   = m_nSideColor[SD_LEFT];
      m_nSideColor[SD_LEFT]   = nTmp;
   }
}



void WCubePiece::rotateZ(BOOL bCW)
{
   SIDECOLOR nTmp;

   if (bCW) {
      nTmp                    = m_nSideColor[SD_TOP];
      m_nSideColor[SD_TOP]    = m_nSideColor[SD_RIGHT];
      m_nSideColor[SD_RIGHT]  = m_nSideColor[SD_BOTTOM];
      m_nSideColor[SD_BOTTOM] = m_nSideColor[SD_LEFT];
      m_nSideColor[SD_LEFT]   = nTmp;
   }
   else {
      nTmp                    = m_nSideColor[SD_TOP];
      m_nSideColor[SD_TOP]    = m_nSideColor[SD_LEFT];
      m_nSideColor[SD_LEFT]   = m_nSideColor[SD_BOTTOM];
      m_nSideColor[SD_BOTTOM] = m_nSideColor[SD_RIGHT];
      m_nSideColor[SD_RIGHT]  = nTmp;
   }
}







//////////////////////////////////////////////////////////////////////////
// Serialization stuff
//////////////////////////////////////////////////////////////////////////

SIDECOLOR WCubePiece::getSidecolor(SIDE si)
{
	return m_nSideColor[si];
}

void WCubePiece::serialize(CArchive& ar, WORD wFileFormat)
{
   if (ar.IsStoring()) {
      ar.Write(m_nSideColor, sizeof(m_nSideColor));    //Color of each side.   
      ar.Write(&m_posHome, sizeof(m_posHome));          //Home position of this piece
   }
   else {
      ar.Read(m_nSideColor, sizeof(m_nSideColor));    //Color of each side.   
      ar.Read(&m_posHome, sizeof(m_posHome));          //Home position of this piece
   }
}

