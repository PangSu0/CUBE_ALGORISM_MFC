//
// DOC_RUBIK_H
//
// This software is licensed/distributed under the GPL 
// WITHOUT ANY WARRANTY - see gpl.license.txt for details
// Written by Ross Wolin/Mystic Industries - Copyright 2003
//


#ifndef DOC_RUBIK_H
#define DOC_RUBIK_H



class CRubikView;



class CRubikDoc : public CDocument
{
public:
	virtual ~CRubikDoc();

protected: 
	CRubikDoc();      // create from serialization only

   CRubikView* getView(void);




#ifdef _DEBUG
public:
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRubikDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL


protected:
	//{{AFX_MSG(CRubikDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	DECLARE_DYNCREATE(CRubikDoc)
};


#include "view_rubik.h"       //Avoid circular include



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif //DOC_RUBIK_H
