// Demo1Doc.cpp : implementation of the CDemo1Doc class
//

#include "stdafx.h"
#include "Demo1.h"

#include "Demo1Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDemo1Doc

IMPLEMENT_DYNCREATE(CDemo1Doc, CDocument)

BEGIN_MESSAGE_MAP(CDemo1Doc, CDocument)
	//{{AFX_MSG_MAP(CDemo1Doc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDemo1Doc construction/destruction

CDemo1Doc::CDemo1Doc()
{
	// TODO: add one-time construction code here

}

CDemo1Doc::~CDemo1Doc()
{
}

BOOL CDemo1Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	((CEditView*)m_viewList.GetHead())->SetWindowText(NULL);

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CDemo1Doc serialization

void CDemo1Doc::Serialize(CArchive& ar)
{
	// CEditView contains an edit control which handles all serialization
	((CEditView*)m_viewList.GetHead())->SerializeRaw(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CDemo1Doc diagnostics

#ifdef _DEBUG
void CDemo1Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDemo1Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDemo1Doc commands
