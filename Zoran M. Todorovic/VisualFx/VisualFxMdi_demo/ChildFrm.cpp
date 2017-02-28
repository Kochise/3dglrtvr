// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "TabWnd.h"

#include "DummyTree.h"
#include "DummyList.h"
#include "DummyEdit.h"
#include "DummyForm.h"
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers


BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
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

void CChildFrame::OnDestroy() 
{
	CMDIChildWnd::OnDestroy();
	
	// TODO: Add your message handler code here
	m_Framework.Destroy();
}
