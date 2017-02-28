// DummyList.cpp : implementation file
//

#include "stdafx.h"
#include "tabwnd.h"
#include "DummyList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDummyList

IMPLEMENT_DYNCREATE(CDummyList, CListView)

CDummyList::CDummyList()
{
}

CDummyList::~CDummyList()
{
}


BEGIN_MESSAGE_MAP(CDummyList, CListView)
	//{{AFX_MSG_MAP(CDummyList)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDummyList drawing

void CDummyList::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CDummyList diagnostics

#ifdef _DEBUG
void CDummyList::AssertValid() const
{
	CListView::AssertValid();
}

void CDummyList::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDummyList message handlers

void CDummyList::OnInitialUpdate() 
{
	CListView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	
}

int CDummyList::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
  CListCtrl& lc = GetListCtrl();
  // insert columns
  lc.InsertColumn(0,"Model",LVCFMT_LEFT,150);
  lc.InsertColumn(1,"Type",LVCFMT_LEFT,100);
  lc.InsertColumn(2,"Country",LVCFMT_LEFT,170);
  // insert rows
  lc.InsertItem(0,"BMW 317i");
  lc.SetItemText(0,1,"Sport");
  lc.SetItemText(0,2,"Germany");
  lc.InsertItem(0,"Ford Fiesta");
  lc.SetItemText(0,1,"Family");
  lc.SetItemText(0,2,"USA");
  lc.InsertItem(0,"Suzuki SWIFT GTI");
  lc.SetItemText(0,1,"Racing");
  lc.SetItemText(0,2,"Japan");
  lc.InsertItem(0,"Landrover Discovery");
  lc.SetItemText(0,1,"Cross country");
  lc.SetItemText(0,2,"Great Britain");
  lc.InsertItem(0,"Nisan Primera");
  lc.SetItemText(0,1,"Luxury");
  lc.SetItemText(0,2,"Japan");
  lc.InsertItem(0,"Mitsubishi Colt");
  lc.SetItemText(0,1,"Sport");
  lc.SetItemText(0,2,"Japan");
  lc.InsertItem(0,"Skoda Octavia");
  lc.SetItemText(0,1,"Chech");
  lc.SetItemText(0,2,"Sedan");
	return 0;
}

BOOL CDummyList::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style |= (LVS_SHOWSELALWAYS | LVS_REPORT | 
               /*LVS_SINGLESEL | */LVS_NOSORTHEADER);
	
	return CListView::PreCreateWindow(cs);
}

void CDummyList::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CListView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}
