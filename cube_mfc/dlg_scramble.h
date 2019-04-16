//
// DLG_SCRAMBLE.H
//
// This software is licensed/distributed under the GPL 
// WITHOUT ANY WARRANTY - see gpl.license.txt for details
// Written by Ross Wolin/Mystic Industries - Copyright 2003
//


#ifndef DLG_SCRAMBLE_H
#define DLG_SCRAMBLE_H




class CScrambleDlg : public CDialog
{
public:
	CScrambleDlg(CWnd* pParent = NULL);   // standard constructor




public:
	//{{AFX_DATA(CScrambleDlg)
	enum { IDD = IDD_SCRAMBLE };
	BOOL	m_bShowRotations;
	UINT	m_ctRotations;
	//}}AFX_DATA


	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScrambleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	// Generated message map functions
	//{{AFX_MSG(CScrambleDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif //DLG_SCRAMBLE_H
