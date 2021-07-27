// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Demo2.h"

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
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_MYTOOLBAR, OnViewMytoolbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MYTOOLBAR, OnUpdateViewMytoolbar)
	ON_COMMAND(ID_VIEW_INSTANT, OnViewInstant)
	ON_UPDATE_COMMAND_UI(ID_VIEW_INSTANT, OnUpdateViewInstant)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_VIEW_1, ID_VIEW_4, OnViewMy)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_1, ID_VIEW_4, OnUpdateViewMy)
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

	// second toolbar must have different ID
	if (!m_wndToolBar2.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,
		CRect(0, 0, 0, 0), 122) ||
		!m_wndToolBar2.LoadToolBar(IDR_TOOLBAR1))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;		// fail to create
	}
	m_wndToolBar2.SetWindowText(_T("Toolbar 2"));

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	for (int i = 0; i < 4; i++)
	{
		CString sTitle;
		sTitle.Format(_T("My Bar %d"), i + 1);
		if (!m_wndMyBars[i].Create(sTitle, this, 123 + i))
		{
			TRACE0("Failed to create mybar\n");
			return -1;		// fail to create
		}
	}
	
	// --- instant bar ---
	if (!m_wndInstantBar.Create(_T("Instant Bar"), this, 127))
	{
		TRACE0("Failed to create instant bar\n");
		return -1;		// fail to create
	}
	m_wndInstantBar.SetSCBStyle(m_wndInstantBar.GetSCBStyle() |
		SCBS_SIZECHILD);
	if (!m_wndIBTree.Create(WS_CHILD|WS_VISIBLE|
		TVS_HASLINES|TVS_HASBUTTONS|TVS_LINESATROOT,
		CRect(0, 0, 0, 0), &m_wndInstantBar, 100))
	{
		TRACE0("Failed to create instant bar child\n");
		return -1;		// fail to create
	}
	m_wndIBTree.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	
	// bring the tooltips to front
	CWnd* pTT = FromHandle((HWND) m_wndIBTree.SendMessage(TVM_GETTOOLTIPS));
    if (pTT != NULL)
		pTT->SetWindowPos(&wndTopMost, 0, 0, 0, 0,
		SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
	
	// populate the treectrl
	HTREEITEM hti = m_wndIBTree.InsertItem(_T("Node 1"));
	m_wndIBTree.InsertItem(_T("Node 2"));
	m_wndIBTree.InsertItem(_T("SubNode 11"), hti);
	m_wndIBTree.InsertItem(_T("SubNode 12"), hti);
	// --- end instant bar creation and child setup ---

	for (i = 0; i < 4; i++)
		m_wndMyBars[i].SetBarStyle(m_wndMyBars[i].GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	
	m_wndInstantBar.SetBarStyle(m_wndInstantBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar2.EnableDocking(CBRS_ALIGN_ANY);
	for (i = 0; i < 4; i++)
		m_wndMyBars[i].EnableDocking(CBRS_ALIGN_ANY);
	// enable docking only on the left of right sides (or floating)
	m_wndInstantBar.EnableDocking(CBRS_ALIGN_LEFT|CBRS_ALIGN_RIGHT);

	EnableDocking(CBRS_ALIGN_ANY);
#ifdef _SCB_REPLACE_MINIFRAME
    m_pFloatingFrameClass = RUNTIME_CLASS(CSCBMiniDockFrameWnd);
#endif //_SCB_REPLACE_MINIFRAME

	DockControlBar(&m_wndToolBar);
	DockControlBar(&m_wndToolBar2, AFX_IDW_DOCKBAR_RIGHT);
	DockControlBar(&m_wndMyBars[0], AFX_IDW_DOCKBAR_RIGHT);

	// dock mybar2 on the same row with mybar1
	RecalcLayout();
	CRect rBar;
	m_wndMyBars[0].GetWindowRect(rBar);
	rBar.OffsetRect(0, 1);
	DockControlBar(&m_wndMyBars[1], AFX_IDW_DOCKBAR_RIGHT, rBar);
	
	DockControlBar(&m_wndMyBars[2], AFX_IDW_DOCKBAR_BOTTOM);
	FloatControlBar(&m_wndMyBars[3], CPoint(300, 200));
	
	DockControlBar(&m_wndInstantBar, AFX_IDW_DOCKBAR_LEFT);
	
	CString sProfile = _T("BarState");
	if (VerifyBarState(sProfile))
	{
		CSizingControlBar::GlobalLoadState(this, sProfile);
		LoadBarState(sProfile);
	}

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

BOOL CMainFrame::DestroyWindow() 
{
	CString sProfile = _T("BarState");
	CSizingControlBar::GlobalSaveState(this, sProfile);
	SaveBarState(sProfile);

	return CFrameWnd::DestroyWindow();
}

void CMainFrame::OnViewMytoolbar() 
{
	ShowControlBar(&m_wndToolBar2, !m_wndToolBar2.IsVisible(), FALSE);
}

void CMainFrame::OnUpdateViewMytoolbar(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(m_wndToolBar2.IsVisible());
}

void CMainFrame::OnViewMy(UINT nID) 
{
	int nBar = nID - ID_VIEW_1;
	BOOL bShow = m_wndMyBars[nBar].IsVisible();
	ShowControlBar(&m_wndMyBars[nBar], !bShow, FALSE);
}

void CMainFrame::OnUpdateViewMy(CCmdUI* pCmdUI) 
{
	int nBar = pCmdUI->m_nID - ID_VIEW_1;
	pCmdUI->Enable();
	pCmdUI->SetCheck(m_wndMyBars[nBar].IsVisible());
}

void CMainFrame::OnViewInstant() 
{
	ShowControlBar(&m_wndInstantBar, !m_wndInstantBar.IsVisible(), FALSE);
}

void CMainFrame::OnUpdateViewInstant(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(m_wndInstantBar.IsVisible());
}

// This function is Copyright (c) 2000, Cristi Posea.
// See www.datamekanix.com for more control bars tips&tricks.
BOOL CMainFrame::VerifyBarState(LPCTSTR lpszProfileName)
{
    CDockState state;
    state.LoadState(lpszProfileName);

    for (int i = 0; i < state.m_arrBarInfo.GetSize(); i++)
    {
        CControlBarInfo* pInfo = (CControlBarInfo*)state.m_arrBarInfo[i];
        ASSERT(pInfo != NULL);
        int nDockedCount = pInfo->m_arrBarID.GetSize();
        if (nDockedCount > 0)
        {
            // dockbar
            for (int j = 0; j < nDockedCount; j++)
            {
                UINT nID = (UINT) pInfo->m_arrBarID[j];
                if (nID == 0) continue; // row separator
                if (nID > 0xFFFF)
                    nID &= 0xFFFF; // placeholder - get the ID
                if (GetControlBar(nID) == NULL)
                    return FALSE;
            }
        }
        
        if (!pInfo->m_bFloating) // floating dockbars can be created later
            if (GetControlBar(pInfo->m_nBarID) == NULL)
                return FALSE; // invalid bar ID
    }

    return TRUE;
}
