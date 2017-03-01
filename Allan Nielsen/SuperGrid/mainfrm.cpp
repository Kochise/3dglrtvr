// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "SuperGrid.h"
#include "SuperVw.h"
#include "SplitterControl.h"

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
	ON_WM_SIZE()
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
m_bInitSplitter = FALSE;
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	if (!m_wndReBar.Create(this) || !m_wndReBar.AddBar(&m_wndToolBar))
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Remove this if you don't want tool tips
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);



	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFrameWnd::PreCreateWindow(cs);
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

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
CRect cr; 
GetClientRect( &cr);



	m_wndSplitter1.Create(WS_CHILD | WS_VISIBLE, cr, this, IDC_SPLITTER1);
	m_wndSplitter1.SetRange(50, 50, -1);

	m_wndSplitter2.Create(WS_CHILD | WS_VISIBLE, cr, this, IDC_SPLITTER2);
	m_wndSplitter2.SetRange(50, 50, -1);





/*
if ( !m_mainSplitter.CreateStatic( this, 2, 1 ) ) 
{ 
MessageBox( "Error setting up splitter frames!", "Init Error!", MB_OK | MB_ICONERROR ); 
return FALSE; 
}


if ( !m_mainSplitter.CreateView( 0, 0, RUNTIME_CLASS(CSuperVw), CSize(cr.Width(), cr.Height()/2), pContext ) ) 
{ 
MessageBox( "Error setting up splitter frames!", "Init Error!", MB_OK | MB_ICONERROR );
return FALSE; 
}
if ( !m_mainSplitter.CreateView( 1, 0, RUNTIME_CLASS(CSuperVw), CSize(cr.Width(), cr.Height()/2), pContext ) ) 
{ 
MessageBox( "Error setting up splitter frames!", "Init Error!", MB_OK | MB_ICONERROR );
return FALSE; 
}
m_bInitSplitter = TRUE;

return TRUE;
*/
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
/*
	CFrameWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
	CRect cr;
	GetWindowRect(&cr);

	if (m_bInitSplitter && nType != SIZE_MINIMIZED )
	{
		m_mainSplitter.SetColumnInfo( 0, cx, 50 );
		m_mainSplitter.SetRowInfo( 0, cr.Height() / 2, 50);
		m_mainSplitter.SetRowInfo( 1, cr.Height() / 2, 50);
		
		m_mainSplitter.RecalcLayout();
	}	
*/
}
