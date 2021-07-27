// Demo2Doc.cpp : implementation of the CDemo2Doc class
//

#include "stdafx.h"
#include "Demo2.h"

#include "Demo2Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDemo2Doc

IMPLEMENT_DYNCREATE(CDemo2Doc, CDocument)

BEGIN_MESSAGE_MAP(CDemo2Doc, CDocument)
	//{{AFX_MSG_MAP(CDemo2Doc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDemo2Doc construction/destruction

CDemo2Doc::CDemo2Doc()
{
	// TODO: add one-time construction code here

}

CDemo2Doc::~CDemo2Doc()
{
}

BOOL CDemo2Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	((CEditView*)m_viewList.GetHead())->SetWindowText(NULL);

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CDemo2Doc serialization

void CDemo2Doc::Serialize(CArchive& ar)
{
	// CEditView contains an edit control which handles all serialization
	((CEditView*)m_viewList.GetHead())->SerializeRaw(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CDemo2Doc diagnostics

#ifdef _DEBUG
void CDemo2Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDemo2Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDemo2Doc commands
