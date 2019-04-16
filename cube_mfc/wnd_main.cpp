//
// WND_MAIN.CPP
// CMainFrame class
//
// This software is licensed/distributed under the GPL 
// WITHOUT ANY WARRANTY - see gpl.license.txt for details
// Written by Ross Wolin/Mystic Industries - Copyright 2003
//


#include "stdafx.h"
#include "rubik.h"
#include "wnd_main.h"
#include "view_rubik.h"
#include "util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(IDM_HELP_CONTENTS, OnHelpContents)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_MOVE_COUNT, OnUpdateMoveCount)
END_MESSAGE_MAP()


static UINT indicators[] =
{
//	ID_SEPARATOR,           // status line indicator
   ID_INDICATOR_MOVE_COUNT,
//	ID_INDICATOR_CAPS,
//	ID_INDICATOR_NUM,
//	ID_INDICATOR_SCRL,
};






CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}



int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this) 
		 || !m_wndStatusBar.SetIndicators(indicators, ELEMENTS_OF(indicators)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	return 0;
}



void CMainFrame::OnHelpContents() 
{
   CWaitCursor wc;
   ::ShellExecute(0, "open", "doc/index.html", "", "", SW_NORMAL);
}



BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	return TRUE;
}




/////////////////////////////////////////////////////////////////////////////
// UI idle loop update stuff
/////////////////////////////////////////////////////////////////////////////

void CMainFrame::OnUpdateMoveCount(CCmdUI* pCmdUI) 
{
   pCmdUI->Enable(); 

   CRubikView* pView = (CRubikView*)GetActiveView();   
   if (!pView)
      return;

   CString csz;
   csz.Format("Move Count = %d", pView->getMoveCount()); 
   pCmdUI->SetText(csz); 
}




/////////////////////////////////////////////////////////////////////////////
// Debug stuff
/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void CMainFrame::AssertValid() const            {CFrameWnd::AssertValid();}
void CMainFrame::Dump(CDumpContext& dc) const   {CFrameWnd::Dump(dc);}
#endif //_DEBUG


