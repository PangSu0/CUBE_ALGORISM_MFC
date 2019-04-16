//
// WND_MAIN.H 
//
// This software is licensed/distributed under the GPL 
// WITHOUT ANY WARRANTY - see gpl.license.txt for details
// Written by Ross Wolin/Mystic Industries - Copyright 2003
//


#ifndef WND_MAIN_H
#define WND_MAIN_H



class CMainFrame : public CFrameWnd
{
protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;

public:
	virtual ~CMainFrame();
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)






public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnHelpContents();
	//}}AFX_MSG
	afx_msg void OnUpdateMoveCount(CCmdUI *pCmdUI);
	DECLARE_MESSAGE_MAP()

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};




//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif //WND_MAIN_H

