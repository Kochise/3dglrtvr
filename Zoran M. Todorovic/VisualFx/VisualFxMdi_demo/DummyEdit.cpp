// DummyEdit.cpp : implementation file
//

#include "stdafx.h"
#include "tabwnd.h"
#include "DummyEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDummyEdit

IMPLEMENT_DYNCREATE(CDummyEdit, CEditView)

CDummyEdit::CDummyEdit()
{
}

CDummyEdit::~CDummyEdit()
{
}


BEGIN_MESSAGE_MAP(CDummyEdit, CEditView)
	//{{AFX_MSG_MAP(CDummyEdit)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDummyEdit drawing

void CDummyEdit::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CDummyEdit diagnostics

#ifdef _DEBUG
void CDummyEdit::AssertValid() const
{
	CEditView::AssertValid();
}

void CDummyEdit::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDummyEdit message handlers
