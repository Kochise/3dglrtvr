// TabWndView.cpp : implementation of the CTabWndView class
//

#include "stdafx.h"
#include "TabWnd.h"

#include "TabWndDoc.h"
#include "TabWndView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabWndView

IMPLEMENT_DYNCREATE(CTabWndView, CView)

BEGIN_MESSAGE_MAP(CTabWndView, CView)
	//{{AFX_MSG_MAP(CTabWndView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabWndView construction/destruction

CTabWndView::CTabWndView()
{
}

CTabWndView::~CTabWndView()
{
}

BOOL CTabWndView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTabWndView drawing

void CTabWndView::OnDraw(CDC* pDC)
{
	CTabWndDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
}

/////////////////////////////////////////////////////////////////////////////
// CTabWndView diagnostics

#ifdef _DEBUG
void CTabWndView::AssertValid() const
{
	CView::AssertValid();
}

void CTabWndView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTabWndDoc* CTabWndView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTabWndDoc)));
	return (CTabWndDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTabWndView message handlers
