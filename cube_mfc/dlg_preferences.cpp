// 
// DLG_PREFERENCES.CPP
// CPreferencesDlg
//
// This software is licensed/distributed under the GPL 
// WITHOUT ANY WARRANTY - see gpl.license.txt for details
// Written by Ross Wolin/Mystic Industries - Copyright 2003
//


#include "stdafx.h"
#include "rubik.h"
#include "dlg_preferences.h"
#include "util.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




//Global preference variables for read access by any module
//(Editting should be limited to the Preferences dialog)
float g_fMinMouseLength = 0.0f;     //Minimum length of mouse vector required to count as section rotation      
float g_fMaxMouseAngle = 0.0f;      //How close mouse move vector needs to be to cube side X or Y vector to register

UINT  g_nRotationDelay = 0;         //Delay in ms between rotation animation steps
UINT  g_nRotationSteps = 0;         //Number of animation steps in a 90 degree rotation

BOOL g_bSwapMouseButtons = FALSE;   //TRUE when the left/right mouse button functions should be swapped


#define MOUSE_LENGTH_DEFAULT    "0.5"   
#define MIN_MOUSE_LENGTH        0.0f
#define MAX_MOUSE_LENGTH        20.0f

#define MOUSE_ANGLE_DEFAULT     "135.0"     
#define MIN_MOUSE_ANGLE         0.0f
#define MAX_MOUSE_ANGLE         135.0f

#define ROT_DELAY_DEFAULT        0
#define MIN_ROT_DELAY            0
#define MAX_ROT_DELAY            500

#define ROT_STEPS_DEFAULT        30
#define MIN_ROT_STEPS            0
#define MAX_ROT_STEPS            500

#define SWAP_MOUSE_BUTTONS_DEFAULT  FALSE








BEGIN_MESSAGE_MAP(CPreferencesDlg, CDialog)
	//{{AFX_MSG_MAP(CPreferencesDlg)
	ON_BN_CLICKED(IDB_DEFAULTS, OnDefaults)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()







CPreferencesDlg::CPreferencesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPreferencesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPreferencesDlg)
	//}}AFX_DATA_INIT
   m_fMinMouseLength = g_fMinMouseLength;
   m_fMaxMouseAngle  = g_fMaxMouseAngle;
	m_nRotationDelay  = g_nRotationDelay;
	m_nRotationSteps  = g_nRotationSteps;
	m_bSwapMouseButtons = g_bSwapMouseButtons;
}



/*virtual*/ CPreferencesDlg::~CPreferencesDlg()
{
   char szBuf[256];
   CWinApp& app = *AfxGetApp();

   _gcvt(g_fMinMouseLength, 6, szBuf);
   app.WriteProfileString("Settings", "Min Mouse Length", szBuf);

	_gcvt(g_fMaxMouseAngle, 6, szBuf);
	app.WriteProfileString("Settings", "Max Mouse Angle", szBuf);

	app.WriteProfileInt("Settings", "Rotation Delay", g_nRotationDelay);
	app.WriteProfileInt("Settings", "Rotation Steps", g_nRotationSteps);

	app.WriteProfileInt("Settings", "Swap Mouse Buttons", g_bSwapMouseButtons);
}



void CPreferencesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPreferencesDlg)
	DDX_Text(pDX, IDE_ROTATION_DELAY, m_nRotationDelay);
	DDV_MinMaxUInt(pDX, m_nRotationDelay, MIN_ROT_DELAY, MAX_ROT_DELAY);
	DDX_Text(pDX, IDE_ROTATION_INCREMENTS, m_nRotationSteps);
	DDV_MinMaxUInt(pDX, m_nRotationSteps, MIN_ROT_STEPS, MAX_ROT_STEPS);
	DDX_Text(pDX, IDE_MOUSE_REJECT_LENGTH, m_fMinMouseLength);
	DDV_MinMaxFloat(pDX, m_fMinMouseLength, MIN_MOUSE_LENGTH, MAX_MOUSE_LENGTH);
	DDX_Text(pDX, IDE_MOUSE_REJECT_ANGLE, m_fMaxMouseAngle);
	DDV_MinMaxFloat(pDX, m_fMaxMouseAngle, MIN_MOUSE_ANGLE, MAX_MOUSE_ANGLE);
	DDX_Check(pDX, IDC_SWAP_BUTTONS, m_bSwapMouseButtons);
	//}}AFX_DATA_MAP
}




/*static*/ void CPreferencesDlg::initPreferences(void)
{
   CString csz;
   CWinApp& app = *AfxGetApp();

	csz = app.GetProfileString("Settings", "Min Mouse Length", MOUSE_LENGTH_DEFAULT);
   g_fMinMouseLength = minmax(MIN_MOUSE_LENGTH, (float)atof(csz), MAX_MOUSE_LENGTH);

	csz = app.GetProfileString("Settings", "Max Mouse Angle", MOUSE_ANGLE_DEFAULT);
   g_fMaxMouseAngle = minmax(MIN_MOUSE_ANGLE, (float)atof(csz), MAX_MOUSE_ANGLE);

	g_nRotationDelay = app.GetProfileInt("Settings", "Rotation Delay", ROT_DELAY_DEFAULT);
	g_nRotationSteps = app.GetProfileInt("Settings", "Rotation Steps", ROT_STEPS_DEFAULT);

	g_bSwapMouseButtons = app.GetProfileInt("Settings", "Swap Mouse Buttons", SWAP_MOUSE_BUTTONS_DEFAULT);
}



void CPreferencesDlg::OnDefaults() 
{
   m_fMinMouseLength = (float)atof(MOUSE_LENGTH_DEFAULT);
   m_fMaxMouseAngle  = (float)atof(MOUSE_ANGLE_DEFAULT);
	m_nRotationDelay = ROT_DELAY_DEFAULT;
	m_nRotationSteps = ROT_STEPS_DEFAULT;
	m_bSwapMouseButtons = SWAP_MOUSE_BUTTONS_DEFAULT;

   CDataExchange de(this, FALSE);
   DoDataExchange(&de);
}



void CPreferencesDlg::OnOK() 
{
	CDialog::OnOK();

   g_fMinMouseLength = m_fMinMouseLength;
   g_fMaxMouseAngle  = m_fMaxMouseAngle;
	g_nRotationDelay  = m_nRotationDelay;
	g_nRotationSteps  = m_nRotationSteps;
	g_bSwapMouseButtons = m_bSwapMouseButtons;
}

