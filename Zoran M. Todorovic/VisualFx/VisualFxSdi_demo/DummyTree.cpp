// DummyTree.cpp : implementation file
//

#include "stdafx.h"
#include "tabwnd.h"
#include "DummyTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDummyTree

IMPLEMENT_DYNCREATE(CDummyTree, CTreeView)

CDummyTree::CDummyTree()
{
}

CDummyTree::~CDummyTree()
{
}


BEGIN_MESSAGE_MAP(CDummyTree, CTreeView)
	//{{AFX_MSG_MAP(CDummyTree)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDummyTree drawing

void CDummyTree::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CDummyTree diagnostics

#ifdef _DEBUG
void CDummyTree::AssertValid() const
{
	CTreeView::AssertValid();
}

void CDummyTree::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDummyTree message handlers

void CDummyTree::OnInitialUpdate() 
{
	CTreeView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	
}

int CDummyTree::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
  CTreeCtrl& tc = GetTreeCtrl();
  // Set tree control styles
  long style = GetWindowLong(tc.m_hWnd,GWL_STYLE);
  style |= (TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | 
            TVS_SHOWSELALWAYS | TVS_DISABLEDRAGDROP);
  SetWindowLong(tc.m_hWnd,GWL_STYLE,style);
  // Insert some items
  HTREEITEM h = tc.InsertItem("Fruits");
  tc.InsertItem("Apple", h);
  tc.InsertItem("Peach", h);
  h = tc.InsertItem("Vegetables");
  tc.InsertItem("Potato", h);
  tc.InsertItem("Carrot", h);
	
	return 0;
}

void CDummyTree::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CTreeView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}
