// MdiFrames.cpp : implementation file
//

#include "stdafx.h"
#include "TabWnd.h"
#include "MdiFrames.h"

#include "DummyTree.h"
#include "DummyList.h"
#include "DummyEdit.h"
#include "DummyForm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSimpleViewFrame

IMPLEMENT_DYNCREATE(CSimpleViewFrame, CMDIChildWnd)

CSimpleViewFrame::CSimpleViewFrame()
{
}

CSimpleViewFrame::~CSimpleViewFrame()
{
}


BEGIN_MESSAGE_MAP(CSimpleViewFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CSimpleViewFrame)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSimpleViewFrame message handlers

BOOL CSimpleViewFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
  TVisualObject *pView = new TVisualObject(1,pContext,RUNTIME_CLASS(CDummyList));
  
  m_Framework.Add(pView);
  
  return m_Framework.Create(this);
	//return CMDIChildWnd::OnCreateClient(lpcs, pContext);
}
void CSimpleViewFrame::OnDestroy() 
{
	CMDIChildWnd::OnDestroy();
	
	// TODO: Add your message handler code here
  m_Framework.Destroy();
	
}

/////////////////////////////////////////////////////////////////////////////
// CSplitterFrame

IMPLEMENT_DYNCREATE(CSplitterFrame, CMDIChildWnd)

CSplitterFrame::CSplitterFrame()
{
}

CSplitterFrame::~CSplitterFrame()
{
}


BEGIN_MESSAGE_MAP(CSplitterFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CSplitterFrame)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSplitterFrame message handlers

BOOL CSplitterFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
  // Simple splitter (1 row, 2 columns)
  TVisualObject *pSplitter = new TVisualObject(1,"Test", 1, 2, pContext);
  TVisualObject *pView1 = new TVisualObject(2,0,0,pContext, RUNTIME_CLASS(CDummyTree), CSize(150,0));
  TVisualObject *pView2 = new TVisualObject(3,0,1,pContext, RUNTIME_CLASS(CDummyEdit), CSize(0,0));
  
  m_Framework.Add(pSplitter);
  m_Framework.Add(pSplitter, pView1);
  m_Framework.Add(pSplitter, pView2);
  
  return m_Framework.Create(this);
	
	//return CMDIChildWnd::OnCreateClient(lpcs, pContext);
}

void CSplitterFrame::OnDestroy() 
{
	CMDIChildWnd::OnDestroy();
	
	// TODO: Add your message handler code here
  m_Framework.Destroy();
}
/////////////////////////////////////////////////////////////////////////////
// CSimpleTabFrame

IMPLEMENT_DYNCREATE(CSimpleTabFrame, CMDIChildWnd)

CSimpleTabFrame::CSimpleTabFrame()
{
}

CSimpleTabFrame::~CSimpleTabFrame()
{
}


BEGIN_MESSAGE_MAP(CSimpleTabFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CSimpleTabFrame)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSimpleTabFrame message handlers

BOOL CSimpleTabFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
  // Simple tab with 2 panes
  TVisualObject *pTab = new TVisualObject(1,"",pContext,RUNTIME_CLASS(TTabWnd));
  TVisualObject *pTabView1 = new TVisualObject(2,"Cars",pContext,RUNTIME_CLASS(CDummyList));
  TVisualObject *pTabView2 = new TVisualObject(3,"Fruits", pContext,RUNTIME_CLASS(CDummyTree));

  pTabView1->SetIcon(IDI_ICON_A);
  pTabView2->SetIcon(IDI_ICON_A);

  m_Framework.Add(pTab);
  m_Framework.Add(pTab, pTabView1);
  m_Framework.Add(pTab, pTabView2);

  return m_Framework.Create(this);
	
	//return CMDIChildWnd::OnCreateClient(lpcs, pContext);
}

void CSimpleTabFrame::OnDestroy() 
{
	CMDIChildWnd::OnDestroy();
	
	// TODO: Add your message handler code here
  m_Framework.Destroy();
}
/////////////////////////////////////////////////////////////////////////////
// CSplitterTabFrame

IMPLEMENT_DYNCREATE(CSplitterTabFrame, CMDIChildWnd)

CSplitterTabFrame::CSplitterTabFrame()
{
}

CSplitterTabFrame::~CSplitterTabFrame()
{
}


BEGIN_MESSAGE_MAP(CSplitterTabFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CSplitterTabFrame)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSplitterTabFrame message handlers

BOOL CSplitterTabFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
  // Splitter (1 row, 2 columns). Second column is a tab with 2 panes
  TVisualObject *pSplitter = new TVisualObject(1,"Test", 1, 2, pContext);
  TVisualObject *pView1 = new TVisualObject(2,0,0,pContext, RUNTIME_CLASS(CDummyTree), CSize(150,0));
  TVisualObject *pTab = new TVisualObject(3,0,1,pContext, RUNTIME_CLASS(TTabWnd), CSize(0,0));
  TVisualObject *pTabView1 = new TVisualObject(4,"Cars",pContext,RUNTIME_CLASS(CDummyList));
  TVisualObject *pTabView2 = new TVisualObject(5,"Fruits", pContext,RUNTIME_CLASS(CDummyTree));

  pTabView1->SetIcon(IDI_ICON_A);
  pTabView2->SetIcon(IDI_ICON_A);

  m_Framework.Add(pSplitter);
  m_Framework.Add(pSplitter, pView1);
  m_Framework.Add(pSplitter, pTab);
  m_Framework.Add(pTab, pTabView1);
  m_Framework.Add(pTab, pTabView2);
	
  return m_Framework.Create(this);

  //return CMDIChildWnd::OnCreateClient(lpcs, pContext);
}

void CSplitterTabFrame::OnDestroy() 
{
	CMDIChildWnd::OnDestroy();
	
	// TODO: Add your message handler code here
  m_Framework.Destroy();
}
/////////////////////////////////////////////////////////////////////////////
// CTabSplitterFrame

IMPLEMENT_DYNCREATE(CTabSplitterFrame, CMDIChildWnd)

CTabSplitterFrame::CTabSplitterFrame()
{
}

CTabSplitterFrame::~CTabSplitterFrame()
{
}


BEGIN_MESSAGE_MAP(CTabSplitterFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CTabSplitterFrame)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabSplitterFrame message handlers

BOOL CTabSplitterFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
  // Tab. First pane is edit view. Second pane is a splitter (1 row, 2 columns)
  TVisualObject *pTab = new TVisualObject(1,"",pContext,RUNTIME_CLASS(TTabWnd));
  TVisualObject *pTabView1 = new TVisualObject(2,"Edit", pContext, RUNTIME_CLASS(CDummyEdit));
  TVisualObject *pTabView2 = new TVisualObject(3,"Cars && Fruits", 1,2,pContext);
  TVisualObject *pView1 = new TVisualObject(4,0,0,pContext, RUNTIME_CLASS(CDummyTree),CSize(100,0));
  TVisualObject *pView2 = new TVisualObject(5,0,1,pContext, RUNTIME_CLASS(CDummyList),CSize(0,0));

  m_Framework.Add(pTab);
  m_Framework.Add(pTab, pTabView1);
  m_Framework.Add(pTab, pTabView2);
  m_Framework.Add(pTabView2, pView1);
  m_Framework.Add(pTabView2, pView2);

  return m_Framework.Create(this);
	
	//return CMDIChildWnd::OnCreateClient(lpcs, pContext);
}

void CTabSplitterFrame::OnDestroy() 
{
	CMDIChildWnd::OnDestroy();
	
	// TODO: Add your message handler code here
  m_Framework.Destroy();
}
/////////////////////////////////////////////////////////////////////////////
// CComplexFrameA

IMPLEMENT_DYNCREATE(CComplexFrameA, CMDIChildWnd)

CComplexFrameA::CComplexFrameA()
{
}

CComplexFrameA::~CComplexFrameA()
{
}


BEGIN_MESSAGE_MAP(CComplexFrameA, CMDIChildWnd)
	//{{AFX_MSG_MAP(CComplexFrameA)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComplexFrameA message handlers

BOOL CComplexFrameA::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
  // Splitter (1 row, 2 columns). Second column is a nested splitter (2 rows, 1 columns).
  // First view is list view. Second view is a tab with 2 panes.
  TVisualObject *pSplitter = new TVisualObject(1,"Test", 1, 2, pContext);
  TVisualObject *pView1 = new TVisualObject(2,0,0,pContext, RUNTIME_CLASS(CDummyTree), CSize(150,0));
  TVisualObject *pView2 = new TVisualObject(3,0,1,2,1,pContext);
  TVisualObject *pNestedView1 = new TVisualObject(4,0,0,pContext,RUNTIME_CLASS(CDummyList),CSize(0,70));
  TVisualObject *pNestedView2 = new TVisualObject(5,1,0,pContext,RUNTIME_CLASS(TTabWnd),CSize(0,0));
  TVisualObject *pPane1 = new TVisualObject(6,"Form", pContext, RUNTIME_CLASS(CDummyForm));
  TVisualObject *pPane2 = new TVisualObject(7,"Edit", pContext, RUNTIME_CLASS(CDummyEdit));

  pView1->SetHotKey('1');
  pNestedView1->SetHotKey('2');
  pPane1->SetHotKey('3');
  pPane2->SetHotKey('4');

  m_Framework.Add(pSplitter);
  m_Framework.Add(pSplitter, pView1);
  m_Framework.Add(pSplitter, pView2);
  m_Framework.Add(pView2, pNestedView1);
  m_Framework.Add(pView2, pNestedView2);
  m_Framework.Add(pNestedView2, pPane1);
  m_Framework.Add(pNestedView2, pPane2);
	
  return m_Framework.Create(this);
	//return CMDIChildWnd::OnCreateClient(lpcs, pContext);
}

void CComplexFrameA::OnDestroy() 
{
	CMDIChildWnd::OnDestroy();
	
	// TODO: Add your message handler code here
  m_Framework.Destroy();
	
}
/////////////////////////////////////////////////////////////////////////////
// CComplexFrameB

IMPLEMENT_DYNCREATE(CComplexFrameB, CMDIChildWnd)

CComplexFrameB::CComplexFrameB()
{
}

CComplexFrameB::~CComplexFrameB()
{
}


BEGIN_MESSAGE_MAP(CComplexFrameB, CMDIChildWnd)
	//{{AFX_MSG_MAP(CComplexFrameB)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComplexFrameB message handlers

BOOL CComplexFrameB::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
  // Tab. First pane is another tab. Second pane is a view
  TVisualObject *pTab1 = new TVisualObject(1,"",pContext,RUNTIME_CLASS(TTabWnd));
  TVisualObject *pTab2 = new TVisualObject(2,"Form && Edit", pContext,RUNTIME_CLASS(TTabWnd));
  TVisualObject *pPane21 = new TVisualObject(3,"Form", pContext, RUNTIME_CLASS(CDummyForm));
  TVisualObject *pPane22 = new TVisualObject(4,"Edit", pContext, RUNTIME_CLASS(CDummyEdit));
  TVisualObject *pPane11 = new TVisualObject(5,"Cars", pContext, RUNTIME_CLASS(CDummyList));

  pPane11->SetHotKey('1');
  pPane21->SetHotKey('2');
  pPane22->SetHotKey('3');

  m_Framework.Add(pTab1);
  m_Framework.Add(pTab1, pPane11);
  m_Framework.Add(pTab1, pTab2);
  m_Framework.Add(pTab2, pPane21);
  m_Framework.Add(pTab2, pPane22);
	
  return m_Framework.Create(this);
	//return CMDIChildWnd::OnCreateClient(lpcs, pContext);
}

void CComplexFrameB::OnDestroy() 
{
	CMDIChildWnd::OnDestroy();
	
	// TODO: Add your message handler code here
  m_Framework.Destroy();	
}
/////////////////////////////////////////////////////////////////////////////
// CComplexFrameC

IMPLEMENT_DYNCREATE(CComplexFrameC, CMDIChildWnd)

CComplexFrameC::CComplexFrameC()
{
}

CComplexFrameC::~CComplexFrameC()
{
}


BEGIN_MESSAGE_MAP(CComplexFrameC, CMDIChildWnd)
	//{{AFX_MSG_MAP(CComplexFrameC)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComplexFrameC message handlers

BOOL CComplexFrameC::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
  // Tab. First pane is edit view. Second pane is a splitter (1 row, 2 columns)
  // First column is tree, second column is a tab. First pane is form, second pane 
  // is a list
  TVisualObject *pTab1 = new TVisualObject(1,"",pContext,RUNTIME_CLASS(TTabWnd),TVisualObject::TOS_TABTOP);
  TVisualObject *pTabView1 = new TVisualObject(2,"Edit", pContext, RUNTIME_CLASS(CDummyEdit));
  TVisualObject *pTabView2 = new TVisualObject(3,"Splitter",1,2,pContext);
  TVisualObject *pTabView3 = new TVisualObject(8,"Tree", pContext, RUNTIME_CLASS(CDummyTree));
  TVisualObject *pSplitView1 = new TVisualObject(4,0,0,pContext,RUNTIME_CLASS(CDummyTree),CSize(100,0));
  TVisualObject *pSplitView2 = new TVisualObject(5,0,1,pContext,RUNTIME_CLASS(TTabWnd),CSize(0,0),TVisualObject::TOS_TABBOTTOM);
  TVisualObject *pPane1 = new TVisualObject(6,"Form",pContext,RUNTIME_CLASS(CDummyForm));
  TVisualObject *pPane2 = new TVisualObject(7,"List",pContext,RUNTIME_CLASS(CDummyList),TVisualObject::TOS_SELECTED);
  TVisualObject *pPane3 = new TVisualObject(9,"Tree",pContext,RUNTIME_CLASS(CDummyTree));

  pTabView1->SetHotKey('1');
  pSplitView1->SetHotKey('2');
  pPane1->SetHotKey('3');
  pPane2->SetHotKey('4');
  pPane3->SetHotKey('5');

  m_Framework.Add(pTab1);
  m_Framework.Add(pTab1,pTabView1);
  m_Framework.Add(pTab1,pTabView2);
  m_Framework.Add(pTab1,pTabView3);
  m_Framework.Add(pTabView2,pSplitView1);
  m_Framework.Add(pTabView2,pSplitView2);
  m_Framework.Add(pSplitView2,pPane1);
  m_Framework.Add(pSplitView2,pPane2);
  m_Framework.Add(pSplitView2,pPane3);
	
  return m_Framework.Create(this);
	//return CMDIChildWnd::OnCreateClient(lpcs, pContext);
}

void CComplexFrameC::OnDestroy() 
{
	CMDIChildWnd::OnDestroy();
	
	// TODO: Add your message handler code here
  m_Framework.Destroy();	
}
/////////////////////////////////////////////////////////////////////////////
// CComplexFrameD

IMPLEMENT_DYNCREATE(CComplexFrameD, CMDIChildWnd)

CComplexFrameD::CComplexFrameD()
{
}

CComplexFrameD::~CComplexFrameD()
{
}


BEGIN_MESSAGE_MAP(CComplexFrameD, CMDIChildWnd)
	//{{AFX_MSG_MAP(CComplexFrameD)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComplexFrameD message handlers

void CComplexFrameD::OnDestroy() 
{
	CMDIChildWnd::OnDestroy();
	
	// TODO: Add your message handler code here
  m_Framework.Destroy();	
}

BOOL CComplexFrameD::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
  // Tab. First pane is edit view. Second pane is a splitter (1 row, 2 columns)
  // First column is tree, second column is a tab. First pane is form, second pane 
  // is a list
  TVisualObject *pTab1 = new TVisualObject(1,"",pContext,RUNTIME_CLASS(TTabWnd),TVisualObject::TOS_TABTOP);
  TVisualObject *pTabView1 = new TVisualObject(2,"Edit", pContext, RUNTIME_CLASS(CDummyEdit));
  TVisualObject *pTabView2 = new TVisualObject(3,"Splitter",1,2,pContext);
  TVisualObject *pTabView3 = new TVisualObject(8,"Tree", pContext, RUNTIME_CLASS(CDummyTree));
  TVisualObject *pSplitView1 = new TVisualObject(4,0,0,pContext,RUNTIME_CLASS(CDummyTree),CSize(100,0));
  TVisualObject *pNested = new TVisualObject(13,0,1,2,1,pContext);
  TVisualObject *pNestedPane = new TVisualObject(14,0,0,pContext,RUNTIME_CLASS(CDummyList),CSize(0,50));
  TVisualObject *pSplitView2 = new TVisualObject(5,1,0,pContext,RUNTIME_CLASS(TTabWnd),CSize(0,0),TVisualObject::TOS_TABBOTTOM);
  TVisualObject *pSplitViewSplit = new TVisualObject(10,"Split",2,1,pContext);
  TVisualObject *pSplitViewSplit1 = new TVisualObject(11,0,0,pContext,RUNTIME_CLASS(CDummyEdit),CSize(0,100));
  TVisualObject *pSplitViewSplit2 = new TVisualObject(12,1,0,pContext,RUNTIME_CLASS(CDummyForm),CSize(0,0));
  TVisualObject *pPane1 = new TVisualObject(6,"Form",pContext,RUNTIME_CLASS(CDummyForm));
  TVisualObject *pPane2 = new TVisualObject(7,"List",pContext,RUNTIME_CLASS(CDummyList),TVisualObject::TOS_SELECTED);
  TVisualObject *pPane3 = new TVisualObject(9,"Tree",pContext,RUNTIME_CLASS(CDummyTree));

  pTabView1->SetHotKey('1');
  pSplitView1->SetHotKey('2');
  pPane1->SetHotKey('3');
  pPane2->SetHotKey('4');
  pPane3->SetHotKey('5');

  pTabView1->SetDescription(_T("Edit view"));
  pTabView3->SetDescription(_T("Tree view"));
  pSplitView1->SetDescription(_T("Tree view within a splitter"));
  pPane1->SetDescription(_T("Form view as a tab within a splitter"));
  pPane2->SetDescription(_T("List view as a tab within a splitter"));
  pPane3->SetDescription(_T("Tree view as a tab within a splitter"));
  pSplitViewSplit1->SetDescription(_T("Edit view within a tab splitter"));
  pSplitViewSplit2->SetDescription(_T("Form view within a tab splitter"));
  pNestedPane->SetDescription(_T("List within a nested splitter"));

  pTabView1->SetIcon(IDI_ICON_A);
  pTabView2->SetIcon(IDI_ICON_A);
  pTabView3->SetIcon(IDI_ICON_A);
  pPane1->SetIcon(IDI_ICON_A);
  pPane2->SetIcon(IDI_ICON_A);

  m_Framework.Add(pTab1);
  m_Framework.Add(pTab1,pTabView1);
  m_Framework.Add(pTab1,pTabView2);
  m_Framework.Add(pTab1,pTabView3);
  m_Framework.Add(pTabView2,pSplitView1);
  m_Framework.Add(pTabView2,pNested);
  m_Framework.Add(pNested, pNestedPane);
  m_Framework.Add(pNested,pSplitView2);
  m_Framework.Add(pSplitView2,pPane1);
  m_Framework.Add(pSplitView2,pSplitViewSplit);
  m_Framework.Add(pSplitView2,pPane2);
  m_Framework.Add(pSplitView2,pPane3);
  m_Framework.Add(pSplitViewSplit, pSplitViewSplit1);
  m_Framework.Add(pSplitViewSplit, pSplitViewSplit2);

  if (m_Framework.Create(this)) {
    //m_Framework.SetActiveTab(pPane3);
    if (pPane3->IsTabPane())
      pPane3->SetActiveTab();
    return TRUE;
  }
  return FALSE;
	//return CMDIChildWnd::OnCreateClient(lpcs, pContext);
}
