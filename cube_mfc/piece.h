//
// PIECE.H
// WCubePiece class
// Rubik's cube section/piece object
// A complete cube is made up of 27 pieces (although the center piece never moves.)
//
// This software is licensed/distributed under the GPL 
// WITHOUT ANY WARRANTY - see gpl.license.txt for details
// Written by Ross Wolin/Mystic Industries - Copyright 2003
//


#ifndef PIECE_H
#define PIECE_H


#include "util.h"
#include "vec3f.h"





//Define sides of piece.   Sides are numbered:
// 1, 0, 0 = 0    right
//-1, 0, 0 = 1    left
// 0, 1, 0 = 2    top
// 0,-1, 0 = 3    bottom
// 0, 0, 1 = 4    front
// 0, 0,-1 = 5    back
//although it doesn't really matter - use the constants...
typedef enum {
   SD_RIGHT,
   SD_LEFT,
   SD_TOP,
   SD_BOTTOM,
   SD_FRONT,
   SD_BACK,
} SIDE;




//Define colors for side
typedef enum {
   RED,
   GREEN,
   BLUE,
   PURPLE,
   ORANGE,
   YELLOW,
   BLACK,
   WHITE
} SIDECOLOR;

//RGB combinations for each SIDECOLOR, based on glColor3ub function
//(Must match SIDECOLOR enum)
const struct {
   BYTE r, g, b;
} m_rgb[] = {
#ifndef USE_SCOTT_COLORS
   {255,  0,  0,},              //Red
   {  0, 255,   0,},            //Green
   {  0,   0, 255,},            //Blue
   {255,   0, 255,},            //Purple
   {255, 127,   0,},            //Orange
   {255, 255,   0,},            //Yellow
   {  0,   0,   0,},            //Black
   {255, 255, 255,},            //White
#else
   { 190,  42,  63,},            //Red
   {   2, 124,   3,},            //Green
   {  21,  62, 152,},            //Blue
   { 127,   0, 127,},            //Purple
   { 237, 130,  94,},            //Orange
   { 238, 208,  78,},            //Yellow
   {   0,   0,   0,},            //Black
   { 255, 255, 255,},            //White
#endif //USE_SCOTT_COLORS
};

//Macro for making up a color to pass to glColor3b, based on cube side
#define MAKECOLOR(nColor)        m_rgb[nColor].r, m_rgb[nColor].g, m_rgb[nColor].b
#define MAKESIDECOLOR(nSide)     MAKECOLOR(m_nSideColor[nSide])






//3D Rubik's cube position structure
//For a 3x3 cube, cooridinates range from -1 to 1
class BYTEVEC 
{
public:
   INT8 x,y,z;

public:
   BYTEVEC() {};
   BYTEVEC(INT8 xx, INT8 yy, INT8 zz)   {x=xx; y=yy; z=zz;};
};





class WCubePiece
{
protected:
   SIDECOLOR m_nSideColor[6];       //Color of each side.   
   BYTEVEC m_posHome;               //Home position of this piece (this is for informational purposes, not sure
                                    //if it's useful for anything.

   //Rotation animation state variables
   float m_fRotationAngle;     
   Vec3f m_vRotation;     


public:
   WCubePiece(BYTEVEC pos);
   virtual ~WCubePiece();

   void draw(INT8 x, INT8 y, INT8 z);

   void rotateX(BOOL bCW);
   void rotateY(BOOL bCW);
   void rotateZ(BOOL bCW);

   void setRotation(float fAngle, Vec3f vRotation)       {m_fRotationAngle=fAngle; m_vRotation=vRotation;};
   void clrRotation(void)                                {m_fRotationAngle=0;};
   SIDECOLOR getSidecolor(SIDE);
   virtual void serialize(CArchive& ar, WORD wFileFormat);
};


#endif //PIECE_H
