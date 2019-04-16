//
// RUBIK.H
//
// This software is licensed/distributed under the GPL 
// WITHOUT ANY WARRANTY - see gpl.license.txt for details
// Written by Ross Wolin/Mystic Industries - Copyright 2003
//

#ifndef RUBIK_H
#define RUBIK_H


#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols





class CRubikApp : public CWinApp
{
public:
	CRubikApp();

   virtual CDocument* OpenDocumentFile(LPCTSTR szFileName);




	//{{AFX_VIRTUAL(CRubikApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CRubikApp)
	afx_msg void OnAppAbout(void);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif //RUBIK_H
