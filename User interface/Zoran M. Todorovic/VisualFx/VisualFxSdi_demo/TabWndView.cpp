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
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabWndView construction/destruction

CTabWndView::CTabWndView()
{
	// TODO: add construction code here

}

CTabWndView::~CTabWndView()
{
}

BOOL CTabWndView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTabWndView drawing

void CTabWndView::OnDraw(CDC* pDC)
{
	CTabWndDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CTabWndView printing

BOOL CTabWndView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTabWndView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTabWndView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
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
