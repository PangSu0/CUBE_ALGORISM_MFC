// 
// DLG_SCRAMBLE.CPP
//
// This software is licensed/distributed under the GPL 
// WITHOUT ANY WARRANTY - see gpl.license.txt for details
// Written by Ross Wolin/Mystic Industries - Copyright 2003
//


#include "stdafx.h"
#include "rubik.h"
#include "dlg_scramble.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




BEGIN_MESSAGE_MAP(CScrambleDlg, CDialog)
	//{{AFX_MSG_MAP(CScrambleDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()





CScrambleDlg::CScrambleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CScrambleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CScrambleDlg)
	//}}AFX_DATA_INIT
	m_ctRotations = AfxGetApp()->GetProfileInt("Settings", "Rotation Count", 100);
	m_bShowRotations = AfxGetApp()->GetProfileInt("Settings", "Show Rotations", 1) ? TRUE : FALSE;
}


void CScrambleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScrambleDlg)
	DDX_Check(pDX, IDC_SHOW_ROTATIONS, m_bShowRotations);
	DDX_Text(pDX, IDE_ROT_COUNT, m_ctRotations);
	DDV_MinMaxUInt(pDX, m_ctRotations, 0, 5000);
	//}}AFX_DATA_MAP
}



void CScrambleDlg::OnOK() 
{
	CDialog::OnOK();

	AfxGetApp()->WriteProfileInt("Settings", "Rotation Count", m_ctRotations);
	AfxGetApp()->WriteProfileInt("Settings", "Show Rotations", m_bShowRotations ? 1 : 0);
}
