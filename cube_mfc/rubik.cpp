//
// RUBIK.CPP
// CRubikApp application class
//
// This software is licensed/distributed under the GPL 
// WITHOUT ANY WARRANTY - see gpl.license.txt for details
// Written by Ross Wolin/Mystic Industries - Copyright 2003
//


#include "stdafx.h"
#include "rubik.h"

#include "wnd_main.h"
#include "doc_rubik.h"
#include "view_rubik.h"
#include "dlg_about.h"
#include "dlg_preferences.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CRubikApp gApp;



BEGIN_MESSAGE_MAP(CRubikApp, CWinApp)
	//{{AFX_MSG_MAP(CRubikApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()





CRubikApp::CRubikApp()
{
}



BOOL CRubikApp::InitInstance()
{
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	SetRegistryKey(_T("Mystic Industries"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register document templates
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CRubikDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CRubikView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;


   CPreferencesDlg::initPreferences();

	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}



void CRubikApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}




//We override this because if the user selects the same file again, we want
//to reopen it (well maybe.)   The default implementation does not reopen the 
//file - it just sets the view window with the document active.
/*virtual*/ CDocument* CRubikApp::OpenDocumentFile(LPCTSTR szFileName)
{
   CString cszFileName;
   CRubikDoc* pDoc;

   CView* pView = ((CFrameWnd*)AfxGetMainWnd())->GetActiveView();
   if (!pView) {
      ASSERT(FALSE);
      goto dropThrough;
   }

   pDoc = (CRubikDoc*)pView->GetDocument();
   if (!pDoc) {
      ASSERT(FALSE);
      goto dropThrough;
   }      

   cszFileName = pDoc->GetPathName();
   if (cszFileName == szFileName && pDoc->IsModified()) {
      CString cszPrompt;
      cszPrompt.Format("Revert to saved cube session %s, losing your changes?", pDoc->GetPathName());
      if (IDYES == AfxMessageBox(cszPrompt, MB_ICONQUESTION|MB_YESNO)) {
         pDoc->SetModifiedFlag(FALSE);    //Tell framework the old file wasn't modified, so we can
         OnFileNew();                     //reopen the file again without getting the "Do you want to 
      }                                   //save" dlg.   
   }

dropThrough:
   return CWinApp::OpenDocumentFile(szFileName);
}
