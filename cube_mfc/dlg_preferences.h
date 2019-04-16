//
// DLG_PREFERENCES.H
// CPreferencesDlg
//
// This software is licensed/distributed under the GPL 
// WITHOUT ANY WARRANTY - see gpl.license.txt for details
// Written by Ross Wolin/Mystic Industries - Copyright 2003
//



#ifndef DLG_PREFERENCES_H
#define DLG_PREFERENCES_H

#include "resource.h"




//Global preference variables for read access by any module
//(Editting should be limited to the Preferences dialog)
extern float g_fMinMouseLength;
extern float g_fMaxMouseAngle;
extern UINT  g_nRotationDelay;
extern UINT  g_nRotationSteps;
extern BOOL  g_bSwapMouseButtons;





class CPreferencesDlg : public CDialog
{
public:
	CPreferencesDlg(CWnd* pParent = NULL); 
   virtual CPreferencesDlg::~CPreferencesDlg();

   static void initPreferences(void);




public:
	//{{AFX_DATA(CPreferencesDlg)
	enum { IDD = IDD_PREFERENCES };
	UINT	m_nRotationDelay;
	UINT	m_nRotationSteps;
   float m_fMinMouseLength;
   float m_fMaxMouseAngle;
	BOOL	m_bSwapMouseButtons;
	//}}AFX_DATA


	//{{AFX_VIRTUAL(CPreferencesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


protected:
	//{{AFX_MSG(CPreferencesDlg)
	virtual void OnOK();
	afx_msg void OnDefaults();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif //DLG_PREFERENCES_H

