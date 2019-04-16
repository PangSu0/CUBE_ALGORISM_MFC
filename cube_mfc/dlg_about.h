//
// DLG_ABOUT.H
//
// This software is licensed/distributed under the GPL 
// WITHOUT ANY WARRANTY - see gpl.license.txt for details
// Written by Ross Wolin/Mystic Industries - Copyright 2003
//



#ifndef DLG_ABOUT_H
#define DLG_ABOUT_H

#include "resource.h"


class CAboutDlg : public CDialog
{
public:
	CAboutDlg();







// Dialog Data
public:
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CString	m_cszBuildDate;
	//}}AFX_DATA
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif //DLG_ABOUT_H
