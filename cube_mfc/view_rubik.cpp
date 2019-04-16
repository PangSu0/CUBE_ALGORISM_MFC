//
// VIEW_RUBIK.CPP
//
// This software is licensed/distributed under the GPL 
// WITHOUT ANY WARRANTY - see gpl.license.txt for details
// Written by Ross Wolin/Mystic Industries - Copyright 2003
//

#include "stdafx.h"
#include <gl\gl.h>
#include <gl\glu.h>
#include <math.h>
#include <stdlib.h>

#include "rubik.h"
#include "view_rubik.h"
#include "dlg_preferences.h"
#include "dlg_scramble.h"
#include "run.h"


#define SPHERE_DIAMETER    3*3.5f

#define ZOOM_MAX           2.00f
#define ZOOM_MIN           0.10f
#define MOUSE_ZOOM_SCALE   0.004f




#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CRubikView, CView)

BEGIN_MESSAGE_MAP(CRubikView, CView)
	//{{AFX_MSG_MAP(CRubikView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_COMMAND(IDM_SCRAMBLE, OnScramble)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(IDM_PREFERENCES, OnPreferences)
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_EDIT_RUN, &CRubikView::OnEditRun)
END_MESSAGE_MAP()







CRubikView::CRubikView()
: m_cube(this), 
  m_hDC(NULL), m_hRC(NULL), m_fScale(1.0f), 
  m_bLButtonDown(FALSE), m_ptLastRButtonDown(BAD_POINT), m_ptLastZoom(BAD_POINT)
{
}



CRubikView::~CRubikView()
{
}



int CRubikView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
   TRACE("CRubikView::OnCreate\n");
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

   if (!initGL()) {
      MessageBox("OpenGL initialization error", "Yikes!", MB_ICONHAND|MB_OK);
      return -1;
   }

   m_cube.init();
	return 0;
}



BOOL CRubikView::PreCreateWindow(CREATESTRUCT& cs)
{
   cs.style |= WS_CLIPCHILDREN|WS_CLIPSIBLINGS;

	return CView::PreCreateWindow(cs);
}



void CRubikView::OnDestroy() 
{
   wglMakeCurrent(NULL, NULL); 

   if (m_hRC) {
		wglDeleteContext(m_hRC);
      m_hRC = NULL;
   }   

   if (m_hDC) {
      ::ReleaseDC(m_hWnd, m_hDC);
      m_hDC = NULL;
   }

	CView::OnDestroy();
}



void CRubikView::OnDraw(CDC* pDC)
{
	CRubikDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

   //clear color and depth buffers 
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glPushMatrix();
      gluLookAt(0,0,1+SPHERE_DIAMETER/2, 0,0,0, 0,1,0);       
      glScalef(m_fScale, m_fScale, m_fScale);
      glMultMatrixf((GLfloat*)m_trackball.getRotMatrix());
//TESTING
      glRotatef(30,1,0,0);
      glRotatef(45,0,1,0);
//TESTING
      glGetDoublev(GL_MODELVIEW_MATRIX, m_mxLastModel);  //Save latest model matrix
      m_cube.draw();
   glPopMatrix();

   SwapBuffers(m_hDC);
}



void CRubikView::OnEditUndo() 
{
   m_cube.undoMove();
	InvalidateRect(NULL);
}



BOOL CRubikView::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;            //Don't erase the background
}



void CRubikView::OnPreferences() 
{
   CPreferencesDlg dlg;
   dlg.DoModal();
}



//Randomly mess up the cube.
//These moves are not recorded in the move list, although they could be.
void CRubikView::OnScramble() 
{
   BOOL bCW;
   INT8 nSection;
   int  nAxis;

   CScrambleDlg dlg;
   if (IDCANCEL == dlg.DoModal())
      return;

   CWaitCursor wc;
   srand((unsigned)time(NULL));
 
   for (int i=0; i<(int)dlg.m_ctRotations; i++) {
      bCW = (rand() >= RAND_MAX/2);             //CW or CCW rotation
      nSection =  rand()*3/(RAND_MAX+1) - 1;    //Section from -1 to 1
      nAxis = rand()*3/(RAND_MAX+1);

      if (nAxis < 1) 
         m_cube.rotateXSection(nSection, bCW, dlg.m_bShowRotations, FALSE);
      else if (nAxis < 2)
         m_cube.rotateYSection(nSection, bCW, dlg.m_bShowRotations, FALSE);
      else
         m_cube.rotateZSection(nSection, bCW, dlg.m_bShowRotations, FALSE);

      if (dlg.m_bShowRotations) {
         InvalidateRect(NULL);
         UpdateWindow();
      }
   }	

   m_cube.resetMoves();       //Clear out any moves that were saved: they're no good now.

   setModifiedFlag(TRUE);
   InvalidateRect(NULL);
}




void CRubikView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);

   if (cy<=0 || cx <=0)
      return;

   if (!m_hDC || !m_hRC)
      return;

   m_ptWndSize.x = cx;
   m_ptWndSize.y = cy;

   glViewport (0,0, cx,cy);



   GLfloat zNear  = 1.0f;     //Pretty much always want this.
   GLfloat zFar   = zNear + SPHERE_DIAMETER;

   GLfloat left   = 0.0f - SPHERE_DIAMETER/2;
   GLfloat right  = 0.0f + SPHERE_DIAMETER/2;
   GLfloat bottom = 0.0f - SPHERE_DIAMETER/2;
   GLfloat top    = 0.0f + SPHERE_DIAMETER/2;

   GLfloat aspect = (float)cx/cy;

   if (aspect < 1.0) {        //Fix the ortho project for aspect ratio
      bottom /= aspect;    
      top /= aspect; 
   }
   else {
      left *= aspect;      
      right *= aspect;
   }

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(left, right, bottom, top, zNear, zFar);


   glMatrixMode(GL_MODELVIEW);         //Leave this loaded for draw, with current RC
}

////////////////////////////////////////////////

void CRubikView::OnEditRun()
{
	rotation formula[300];
	CUBE temcube(&m_cube, formula);
	piece pie[6][3][3];
	
	for (int a = 0; a < 3; a++)
		for (int b = 0; b < 3; b++)
		{
			pie[0][a][b].Setcolor(color(m_cube.getPiece(b-1, 1, a-1)->getSidecolor(SD_TOP)));
			pie[1][a][b].Setcolor(color(m_cube.getPiece(-1, 1 - a, b - 1)->getSidecolor(SD_LEFT)));
			pie[2][a][b].Setcolor(color(m_cube.getPiece(b - 1, 1 - a, 1)->getSidecolor(SD_FRONT)));
			pie[3][a][b].Setcolor(color(m_cube.getPiece(1, 1 - a , 1 - b)->getSidecolor(SD_RIGHT)));
			pie[4][a][b].Setcolor(color(m_cube.getPiece(1 - b, 1 - a, -1)->getSidecolor(SD_BACK)));
			pie[5][a][b].Setcolor(color(m_cube.getPiece(b - 1 , -1, 1 - a)->getSidecolor(SD_BOTTOM)));
		}
	temcube.Setpiece(pie);
	temcube.Run();
	for (int i = 0; formula[i] != EndRotation; i++)
	{
		switch (formula[i])
		{
		case U:		m_cube.rotateYSection(1, false, true, false);		break;
		case U_:	m_cube.rotateYSection(1, true, true, false);		break;
		case L:		m_cube.rotateXSection(-1, true, true, false);		break;
		case L_:	m_cube.rotateXSection(-1, false, true, false);		break;
		case F:		m_cube.rotateZSection(1, false, true, false);		break;
		case F_:	m_cube.rotateZSection(1, true, true, false);		break;
		case R:		m_cube.rotateXSection(1, false, true, false);		break;
		case R_:	m_cube.rotateXSection(1, true, true, false);		break;
		case B:		m_cube.rotateZSection(-1, true, true, false);		break;
		case B_:	m_cube.rotateZSection(-1, false, true, false);		break;
		case D:		m_cube.rotateYSection(-1, true, true, false);		break;
		case D_:	m_cube.rotateYSection(-1, false, true, false);		break;
		}
		InvalidateRect(NULL);
		UpdateWindow();
	}
	
	m_cube.resetMoves();       //Clear out any moves that were saved: they're no good now.

	setModifiedFlag(TRUE);
	InvalidateRect(NULL);
}


void CRubikView::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
   pCmdUI->Enable(m_cube.getMoveCount());
}



///////////////////////////////////////////////////////////////
// Mouse processing stuff
///////////////////////////////////////////////////////////////

//Among other things, sets the m_bLButtonDown flag.   This is necessary
//(rather than just checking nFlags in OnMouseMove) because sometimes
//the button goes down outside our window, and in this case we don't 
//want to pick up a mouse move with the button down.
void CRubikView::OnLButtonDown(UINT nFlags, CPoint ptMouse) 
{
   if (!g_bSwapMouseButtons)
      onRotateButtonDown(nFlags, ptMouse);
   else
      onSectionButtonDown(nFlags, ptMouse);
      
	CView::OnLButtonDown(nFlags, ptMouse);
}


void CRubikView::OnLButtonUp(UINT nFlags, CPoint ptMouse) 
{
   if (!g_bSwapMouseButtons)
      onRotateButtonUp(nFlags, ptMouse);
   else
      onSectionButtonUp(nFlags, ptMouse);

	CView::OnLButtonUp(nFlags, ptMouse);
}



//The code for mapping a 2D mouse to a 3D logical sphere came from a really nice
//example called SierpinskiSolids that I found on the Internet.
void CRubikView::OnMouseMove(UINT nFlags, CPoint ptMouse) 
{
   BOOL bIsCtrl = isCtrlKey();

   //Rotate the cube, but only if the CTRL key isn't down.
   if (m_bLButtonDown && !bIsCtrl) {
      if (m_trackball.OnMouseMove(ptMouse, m_ptWndSize)) {
         setModifiedFlag(TRUE);
         InvalidateRect(NULL);
      }
   }

   //Zoom if the CTRL key is down.
   if (bIsCtrl) {
      if (isGoodPoint(m_ptLastZoom)) {         //TRUE if zooming
         m_fScale = max(min(m_fScale*1-(ptMouse.x-m_ptLastZoom.x)*MOUSE_ZOOM_SCALE,  ZOOM_MAX), ZOOM_MIN);
         setModifiedFlag(TRUE);
         InvalidateRect(NULL);
      }
   	m_ptLastZoom = ptMouse;
   }
   else
      m_ptLastZoom = BAD_POINT;                //Not zooming


	CView::OnMouseMove(nFlags, ptMouse);
}






void CRubikView::OnRButtonDown(UINT nFlags, CPoint ptMouse) 
{
   if (!g_bSwapMouseButtons)
      onSectionButtonDown(nFlags, ptMouse);
   else
      onRotateButtonDown(nFlags, ptMouse);

	CView::OnRButtonDown(nFlags, ptMouse);
}



void CRubikView::OnRButtonUp(UINT nFlags, CPoint ptMouse) 
{
   if (!g_bSwapMouseButtons)
      onSectionButtonUp(nFlags, ptMouse);
   else
      onRotateButtonUp(nFlags, ptMouse);
      
	CView::OnRButtonUp(nFlags, ptMouse);
}







void CRubikView::onRotateButtonDown(UINT, CPoint ptMouse) 
{
   m_trackball.OnButtonDown(ptMouse, m_ptWndSize);

 	m_ptLastZoom = isCtrlKey()                      //Update zoom pos if the CTRL key is down.
                  ? ptMouse                        //Zooming
                  : BAD_POINT;                     //Not zooming

   m_bLButtonDown = TRUE;
}



void CRubikView::onRotateButtonUp(UINT, CPoint) 
{
   m_bLButtonDown = FALSE;
}



void CRubikView::onSectionButtonDown(UINT nFlags, CPoint ptMouse) 
{
   //If control key is down, ignore mouse move, otherwise, save for right button up
	m_ptLastRButtonDown = isCtrlKey()
                         ? BAD_POINT     
                         : ptMouse;
	
}




void CRubikView::onSectionButtonUp(UINT nFlags, CPoint ptMouse) 
{
   GLdouble mxProjection[16];
   GLint nViewPort[4];
 
   glGetDoublev(GL_PROJECTION_MATRIX, mxProjection);
   glGetIntegerv(GL_VIEWPORT, nViewPort);

   //Rotate section, but only if the CTRL key isn't down.
   if (!isCtrlKey() 
       && isGoodPoint(m_ptLastRButtonDown))
   {
      if (m_cube.rotate(mxProjection, m_mxLastModel, nViewPort, m_ptWndSize, ptMouse, m_ptLastRButtonDown)) {
         setModifiedFlag(TRUE);
         InvalidateRect(NULL);
      }
   }

}

///////////////////////////////////////////////////////////////
// Non-event functions
///////////////////////////////////////////////////////////////

BOOL CRubikView::initGL(void)
{
   CRect rc;
	PIXELFORMATDESCRIPTOR pfd;

   m_hDC = ::GetDC(m_hWnd);

	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize = sizeof(pfd);            // version number 
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;     // RGBA type 
   pfd.cColorBits = 24;                // 24-bit color depth 
	pfd.cDepthBits = 16;                // 16-bit z-buffer 
	pfd.iLayerType = PFD_MAIN_PLANE;    // main layer 


   int format = ChoosePixelFormat(m_hDC, &pfd); 
   if (format==0) 
      goto error;

   if (!SetPixelFormat(m_hDC, format, &pfd))  
      goto error;


   //Pixel formatting's done, now set up rendering context
	m_hRC = wglCreateContext(m_hDC);
	wglMakeCurrent(m_hDC, m_hRC); 

	glShadeModel(GL_FLAT);
   glEnable(GL_DEPTH_TEST);

   glEnable(GL_LINE_SMOOTH);
// glLineWidth(2.0);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);


   //Size the screen
   GetClientRect(&rc);
   OnSize(0, rc.Width(), rc.Height());

   return TRUE;



error:
   if (m_hDC) {
      ::ReleaseDC(m_hWnd, m_hDC);
      m_hDC = NULL;
   }
   return FALSE; 
}




//This is called by document in response to OnNewDocument()
void CRubikView::reset(void) 
{
   m_trackball.reset();    //Clear out any trackball
   m_cube.reset();         //  and cube rotations
}





//////////////////////////////////////////////////////////////////////////
// Serialization stuff
//////////////////////////////////////////////////////////////////////////
void CRubikView::serialize(CArchive& ar, WORD wFileFormat)
{
	if (ar.IsStoring()) {
      ar << m_fScale;
	}
	else {
      ar >> m_fScale;
	}

   m_trackball.serialize(ar, wFileFormat);
   m_cube.serialize(ar, wFileFormat);
}





/////////////////////////////////////////////////////////////////////////////
// CRubikView diagnostics
/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void CRubikView::AssertValid() const               {CView::AssertValid();}
void CRubikView::Dump(CDumpContext& dc) const      {CView::Dump(dc);}

CRubikDoc* CRubikView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRubikDoc)));
	return (CRubikDoc*)m_pDocument;
}
#endif //_DEBUG














