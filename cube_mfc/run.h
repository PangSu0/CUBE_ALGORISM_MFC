#ifndef RUN_H
#define RUN_H
#include <gl/gl.h>
#include "doc_rubik.h"
#include "cube.h"
#include "trackball.h"
#include "vec3f.h"

enum color { cR, cG, cB, cP, cO, cY, cBl, cW, None };
enum conect { up, down, left, right , front, back};
enum rotation { U, D, L, R, F, B, U_, D_, L_, R_, F_, B_ ,EndRotation, PassRotation };
enum position { sd_top, sd_bottom, sd_left, sd_right, sd_front, sd_back };

class piece
{
public:
	piece();
	void Setcolor(color);
	void Setconnect(conect, piece*);
	piece* Getconnect(conect) const;
	color Getcolor() const;
private:
	color col;
	piece* conn[6];
};

class CUBE
{
public:
	CUBE(WCube*, rotation[]);
	void Rotate(rotation);
	void Rotater(const char[]);
	void Setpiece(piece[][3][3]);
	void Chapter1();
	void Chapter2();
	void Chapter3();
	void Chapter4();
	void Chapter5();
	void Chapter6();
	void Chapter7();
	void Run();
private:
	conect OppositeConnect(conect) const;
	void CrossConnect(piece&, conect, piece&, conect);
	void Conversion(int, const char[], char[]) const;
	void CP1_SearchColor(piece*, conect, color, const char[]);
	void CP2_SearchColor(piece*, conect, color, color, const  char[], int);
	void CP3_SearchColor(int, color, color);
	void CP1_ChangeColor(int);
	void CP2_ChangeColor(int);
private:
	WCube* pcb;
	color sd_position[6];
	piece pie[6][3][3];
	rotation* formula;
	int formulaIndex;
};
#endif //RUN_H

