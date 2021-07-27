// TabWndDoc.cpp : implementation of the CTabWndDoc class
//

#include "stdafx.h"
#include "TabWnd.h"

#include "TabWndDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabWndDoc

IMPLEMENT_DYNCREATE(CTabWndDoc, CDocument)

BEGIN_MESSAGE_MAP(CTabWndDoc, CDocument)
	//{{AFX_MSG_MAP(CTabWndDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabWndDoc construction/destruction

CTabWndDoc::CTabWndDoc()
{
}

CTabWndDoc::~CTabWndDoc()
{
}

BOOL CTabWndDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CTabWndDoc serialization

void CTabWndDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTabWndDoc diagnostics

#ifdef _DEBUG
void CTabWndDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTabWndDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTabWndDoc commands
