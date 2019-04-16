//
// CUBE.H
// Rubik's cube object
// A complete cube is a collection of 27 "pieces"
//
// This software is licensed/distributed under the GPL 
// WITHOUT ANY WARRANTY - see gpl.license.txt for details
// Written by Ross Wolin/Mystic Industries - Copyright 2003
//


#ifndef CUBE_H
#define CUBE_H


#include "util.h"
#include "piece.h"
#include "store_move.h"
#include <gl/gl.h>


class WCube
{
protected:
   WCubePiece* m_pPieces[3][3][3];
   WMoveStore  m_moves;                   //Storage for all rotations put on the cube
   CWnd*       m_pWnd;                    //Ptr to window that's displaying the cube


public:
   WCube(CWnd* pWnd);
   virtual ~WCube();

   void init(void);
   void draw(void);
   void reset(void);

   //Get piece based on cube coordinates
   WCubePiece*& getPiece(INT8 x, INT8 y, INT8 z)    {return m_pPieces[x+1][y+1][z+1];};

   //Translate a mouse movement to a cube rotation.
   BOOL rotate(GLdouble* mxProjection, GLdouble* mxModel, GLint* nViewport,CPoint& wndSize, CPoint& ptMouseWnd, CPoint& ptLastMouseWnd);
   void rotateXSection(INT8 nSection, BOOL bCW, BOOL bAnimate, BOOL bRecord);
   void rotateYSection(INT8 nSection, BOOL bCW, BOOL bAnimate, BOOL bRecord);
   void rotateZSection(INT8 nSection, BOOL bCW, BOOL bAnimate, BOOL bRecord);

   void serialize(CArchive& ar, WORD wFileFormat);


   //Move store manipulation
   int getMoveCount(void)     {return m_moves.GetSize();};
   void undoMove(void);
   void resetMoves(void)      {m_moves.RemoveAll();};


protected:
   void animateRotation(WCubePiece* piece[], int ctPieces, Vec3f v, float fAngle);
};




#endif //CUBE_H
