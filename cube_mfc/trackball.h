//
// TRACKBALL.H
// WTrackball class
// 2D mouse to 3D trackball input control, based on code from sample
// MFC application SierpinskiSolids, author unknown.
//
// Class implemention, rotation matrix storage, and rotation/rounding
// error correction added by Ross Wolin/Mystic Industries.
// Rounding error correction/matrix normalization courtesy of 
// Chuck Bass and Gram-Schmidt.
// 
// This software is licensed/distributed under the GPL 
// WITHOUT ANY WARRANTY - see gpl.license.txt for details
// Written by Ross Wolin/Mystic Industries - Copyright 2003
//

#ifndef TRACKBALL_H
#define TRACKBALL_H

#include <gl/gl.h>
#include "vec3f.h"



class WTrackball 
{
protected:
   GLfloat m_mxRotation[16];     //Accumulated rotation matrix
   Vec3f m_vLastMouse;
   int m_ctRotations;            //# of rotations since last rotation matrix correction or reset


public:
   WTrackball();
   virtual ~WTrackball();

   void OnButtonDown(CPoint& point, CPoint& wndSize);
   BOOL OnMouseMove(CPoint& point, CPoint& wndSize);

   GLfloat* getRotMatrix(void)      {return m_mxRotation;};

   void reset(void);
   void serialize(CArchive& ar, WORD wFileFormat);



protected:
   BOOL calcRotation(Vec3f vMouse);
   void correctRotationMatrix(void);

   Vec3f map(CPoint& point, CPoint& wndSize);
};

#endif //TRACKBALL_H