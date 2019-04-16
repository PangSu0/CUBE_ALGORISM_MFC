//
// VIEW_RUBIK.H
//
// This software is licensed/distributed under the GPL 
// WITHOUT ANY WARRANTY - see gpl.license.txt for details
// Written by Ross Wolin/Mystic Industries - Copyright 2003
//


#ifndef VIEW_RUBIK_H
#define VIEW_RUBIK_H


#include <gl/gl.h>
#include "doc_rubik.h"
#include "cube.h"
#include "trackball.h"
#include "vec3f.h"


#define BAD_POINT    CPoint(-1,-1)



class CRubikView : public CView
{
protected:
   WCube m_cube;                 //Cube object - technically this should probably be in the doc object.

   HDC   m_hDC;
   HGLRC m_hRC;
   float m_fScale;
	CPoint m_ptWndSize;            // The current window size in terms of pixels.

   WTrackball m_trackball;
   Vec3f m_vLastMouse;

   CPoint m_ptLastRButtonDown;   //Mouse position last time right mouse button was depressed
   GLdouble m_mxLastModel[16];   //Model matrix from most recent screen update

   BOOL   m_bLButtonDown;        //TRUE when left mouse button is down (and went down in our window!)
   CPoint m_ptLastZoom;          //Mouse position last time left mouse button and CTRL was depressed



public:
	virtual ~CRubikView();
	CRubikDoc* GetDocument();

   void reset(void);
   void serialize(CArchive& ar, WORD wFileFormat);

   int getMoveCount(void)       {return m_cube.getMoveCount();};

protected: 
	CRubikView();                 // create from serialization only
   void drawCube(void);
   BOOL initGL();
   void initDisplayLists(void);

   BOOL isCtrlKey(void)                         {return GetKeyState(VK_CONTROL) & 0x8000;};
   BOOL isGoodPoint(CPoint& pt)                 {return pt!=BAD_POINT;};
   
   void setModifiedFlag(BOOL bModified=TRUE)    {GetDocument()->SetModifiedFlag(bModified);};


   void onRotateButtonDown(UINT nFlags, CPoint ptMouse); 
   void onRotateButtonUp(UINT nFlags, CPoint ptMouse); 
   void onSectionButtonDown(UINT nFlags, CPoint ptMouse); 
   void onSectionButtonUp(UINT nFlags, CPoint ptMouse);





public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRubikView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation


// Generated message map functions
protected:
	//{{AFX_MSG(CRubikView)
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnScramble();
	afx_msg void OnEditRun();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnEditUndo();
	afx_msg void OnPreferences();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	DECLARE_DYNCREATE(CRubikView)
};

#ifndef _DEBUG  // debug version in view_3D.cpp
inline CRubikDoc* CRubikView::GetDocument()     {return (CRubikDoc*)m_pDocument;}
#endif


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif //VIEW_RUBIK_H
