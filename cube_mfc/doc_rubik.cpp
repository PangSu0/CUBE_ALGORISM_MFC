// 
// DOC_RUBIK.CPP
//
// This software is licensed/distributed under the GPL 
// WITHOUT ANY WARRANTY - see gpl.license.txt for details
// Written by Ross Wolin/Mystic Industries - Copyright 2003
//



#include "stdafx.h"
#include "rubik.h"

#include "doc_rubik.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



IMPLEMENT_DYNCREATE(CRubikDoc, CDocument)

BEGIN_MESSAGE_MAP(CRubikDoc, CDocument)
	//{{AFX_MSG_MAP(CRubikDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



const char m_szFileHeader[] = "Rubik's Cube Simulation Data File";   //All data files have this
const WORD m_wLatestFileFormat = 0x0002;                             //Specifies the file format version



CRubikDoc::CRubikDoc()
{
}

CRubikDoc::~CRubikDoc()
{
}



//Normally the doc would have all the data objects, but in this simple case
//I've stored all the data in the view object, so tell view object to reset
//all the data.
BOOL CRubikDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

   CRubikView* pView = getView();
   if (!pView)
      return FALSE;

   pView->reset();
	return TRUE;
}




//Normally the doc would have all the data objects, but in this simple case
//I've stored all the data in the view object, so tell it to serialize...
void CRubikDoc::Serialize(CArchive& ar)
{
   WORD wFileFormat = m_wLatestFileFormat;

   CRubikView* pView = getView();
   if (!pView)
      return;

	if (ar.IsStoring()) {
      ar.WriteString(m_szFileHeader);
      ar.WriteString("\r\n");
      ar << m_wLatestFileFormat;
   }
   else {
      CString csz;
      if (!ar.ReadString(csz) || csz != m_szFileHeader) {
         AfxThrowArchiveException(CArchiveException::badIndex, ar.GetFile()->GetFileName()); 
         return;
      }
      ar >> wFileFormat;

      if (wFileFormat > m_wLatestFileFormat) {
         AfxThrowArchiveException(CArchiveException::badIndex, ar.GetFile()->GetFileName()); 
         return;
      }
   }



   pView->serialize(ar, wFileFormat);

	if (!ar.IsStoring()) 
      UpdateAllViews(NULL);
}




CRubikView* CRubikDoc::getView(void)
{
   POSITION nPos = GetFirstViewPosition();
   return (CRubikView*)GetNextView(nPos);
}





/////////////////////////////////////////////////////////////////////////////
// CRubikDoc diagnostics
/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void CRubikDoc::AssertValid() const          {CDocument::AssertValid();}
void CRubikDoc::Dump(CDumpContext& dc) const {CDocument::Dump(dc);}
#endif //_DEBUG

