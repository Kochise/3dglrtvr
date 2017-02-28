// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "TabWnd.h"

#include "DummyTree.h"
#include "DummyList.h"
#include "DummyEdit.h"
#include "DummyForm.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
  DockControlBar(&m_wndToolBar);

	NONCLIENTMETRICS metrics;
	metrics.cbSize = sizeof(metrics);
  ::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &metrics, 0);
  CWindowDC wdc(NULL);
  int nLPixY = GetDeviceCaps(wdc.m_hDC, LOGPIXELSY);
  _tcscpy(metrics.lfStatusFont.lfFaceName, _T("Verdana"));
  metrics.lfStatusFont.lfItalic = 1;
	m_Font.CreateFontIndirect(&metrics.lfStatusFont);
  

  SetTimer(100,1000,NULL);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

#define CASE_7

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
  /*
  Splitter.CreateStatic(this, 1,2);
  Splitter.CreateView(0,0,RUNTIME_CLASS(CDummyTree),CSize(70,0),pContext);
  Splitter.CreateView(0,1,RUNTIME_CLASS(CDummyForm),CSize(0,0),pContext);

  //Splitter.SetActivePane(0,1);
  //SetActiveView((CView*)Splitter.GetPane(0,1));
  */

#if defined (CASE_1)

  // Simple splitter (1 row, 2 columns)
  TVisualObject *pSplitter = new TVisualObject(1,"Test", 1, 2, pContext);
  TVisualObject *pView1 = new TVisualObject(2,0,0,pContext, RUNTIME_CLASS(CDummyTree), CSize(150,0));
  TVisualObject *pView2 = new TVisualObject(3,0,1,pContext, RUNTIME_CLASS(CDummyEdit), CSize(0,0));
  
  m_Framework.Add(pSplitter);
  m_Framework.Add(pSplitter, pView1);
  m_Framework.Add(pSplitter, pView2);

#elif defined (CASE_2)

  // Simple tab with 2 panes
  TVisualObject *pTab = new TVisualObject(1,"",pContext,RUNTIME_CLASS(TTabWnd));
  TVisualObject *pTabView1 = new TVisualObject(2,"Cars",pContext,RUNTIME_CLASS(CDummyList));
  TVisualObject *pTabView2 = new TVisualObject(3,"Fruits", pContext,RUNTIME_CLASS(CDummyTree));

  m_Framework.Add(pTab);
  m_Framework.Add(pTab, pTabView1);
  m_Framework.Add(pTab, pTabView2);

#elif defined (CASE_3)

  // Splitter (1 row, 2 columns). Second column is a tab with 2 panes
  TVisualObject *pSplitter = new TVisualObject(1,"Test", 1, 2, pContext);
  TVisualObject *pView1 = new TVisualObject(2,0,0,pContext, RUNTIME_CLASS(CDummyTree), CSize(150,0));
  TVisualObject *pTab = new TVisualObject(3,0,1,pContext, RUNTIME_CLASS(TTabWnd), CSize(0,0));
  TVisualObject *pTabView1 = new TVisualObject(4,"Cars",pContext,RUNTIME_CLASS(CDummyList));
  TVisualObject *pTabView2 = new TVisualObject(5,"Fruits", pContext,RUNTIME_CLASS(CDummyTree));

  m_Framework.Add(pSplitter);
  m_Framework.Add(pSplitter, pView1);
  m_Framework.Add(pSplitter, pTab);
  m_Framework.Add(pTab, pTabView1);
  m_Framework.Add(pTab, pTabView2);

#elif defined (CASE_4)

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

#elif defined (CASE_5)

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

#elif defined (CASE_6)

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

#elif defined (CASE_7)

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

  pTabView1->SetIcon(IDI_TESTICON_A);
  pTabView2->SetIcon(IDI_TESTICON_A);
  pTabView3->SetIcon(IDI_TESTICON_A);
  pPane1->SetIcon(IDI_TESTICON_A);
  pPane2->SetIcon(IDI_TESTICON_A);
  pPane3->SetIcon(IDI_TESTICON_A);
  pSplitViewSplit->SetIcon(IDI_TESTICON_A);

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

#elif defined (CASE_8)

  TVisualObject *pView = new TVisualObject(1,pContext,RUNTIME_CLASS(CDummyList));

  m_Framework.Add(pView);

#endif

  m_Framework.Create(this);

  TVisualFrameworkIterator it(m_Framework);
  while (!it.End()) {
    TVisualObject *pObject = it.Get();
    it++;
  }

  //m_Framework.Enable(4,FALSE);
  //m_Framework.Enable(6,FALSE);
  //m_Framework.Enable(9,FALSE);

  return TRUE;
	//return CFrameWnd::OnCreateClient(lpcs, pContext);
}

void CMainFrame::OnDestroy() 
{
	CFrameWnd::OnDestroy();
	
	// TODO: Add your message handler code here
  m_Framework.Destroy();

  KillTimer(100);
	
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
  CView *pView = GetActiveView();
  TVisualObject *pObject = m_Framework.GetActivePane();
  if (pObject->GetID() == 7) {
    m_Framework.SetFont(&m_Font);
  }
	
	CFrameWnd::OnTimer(nIDEvent);
}
