// StylesPageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "virtualgriddemo.h"
#include "StylesPageDlg.h"
#include "VirtualGridCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStylesPageDlg dialog


CStylesPageDlg::CStylesPageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStylesPageDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStylesPageDlg)
	m_bDotLines = FALSE;
	m_bFullHorzLine = FALSE;
	m_bFullVertLine = FALSE;
	m_bHorzLine = FALSE;
	m_bListView = FALSE;
	m_bVertLine = FALSE;
	//}}AFX_DATA_INIT
}


void CStylesPageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStylesPageDlg)
	DDX_Check(pDX, IDC_CHECK_DOT_LINES, m_bDotLines);
	DDX_Check(pDX, IDC_CHECK_FULL_HORZ_LINE, m_bFullHorzLine);
	DDX_Check(pDX, IDC_CHECK_FULL_VERT_LINE, m_bFullVertLine);
	DDX_Check(pDX, IDC_CHECK_HOR_LINE, m_bHorzLine);
	DDX_Check(pDX, IDC_CHECK_LIST_VIEW, m_bListView);
	DDX_Check(pDX, IDC_CHECK_VERT_LINE, m_bVertLine);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStylesPageDlg, CDialog)
	//{{AFX_MSG_MAP(CStylesPageDlg)
	ON_BN_CLICKED(IDC_CHECK_HOR_LINE, OnCheckHorLine)
	ON_BN_CLICKED(IDC_CHECK_VERT_LINE, OnCheckVertLine)
	ON_BN_CLICKED(IDC_CHECK_FULL_HORZ_LINE, OnCheckFullHorzLine)
	ON_BN_CLICKED(IDC_CHECK_FULL_VERT_LINE, OnCheckFullVertLine)
	ON_BN_CLICKED(IDC_CHECK_LIST_VIEW, OnCheckListView)
	ON_BN_CLICKED(IDC_CHECK_DOT_LINES, OnCheckDotLines)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStylesPageDlg message handlers


//  ------------  Horizontal grid lines: to show or not to show  --------
void CStylesPageDlg::OnCheckHorLine() 
{
	BOOL bSuccess;
	UINT nStyle;

	bSuccess = UpdateData();
	ASSERT(bSuccess);

	nStyle = m_pGrid->GetGridStyle();

	if (m_bHorzLine)
		m_pGrid->SetGridStyle(nStyle | gsHorzLine);
	else
		m_pGrid->SetGridStyle(nStyle & ~gsHorzLine);	
}


//  ------------  Vertical grid lines: to show or not to show  --------
void CStylesPageDlg::OnCheckVertLine() 
{
	BOOL bSuccess;
	UINT nStyle;

	bSuccess = UpdateData();
	ASSERT(bSuccess);

	nStyle = m_pGrid->GetGridStyle();

	if (m_bVertLine)
		m_pGrid->SetGridStyle(nStyle | gsVertLine);
	else
		m_pGrid->SetGridStyle(nStyle & ~gsVertLine);
}


//  -----
void CStylesPageDlg::OnCheckFullHorzLine() 
{
	BOOL bSuccess;
	UINT nStyle;

	bSuccess = UpdateData();
	ASSERT(bSuccess);

	nStyle = m_pGrid->GetGridStyle();

	if (m_bFullHorzLine)
		m_pGrid->SetGridStyle(nStyle | gsFullHorzLine);
	else
		m_pGrid->SetGridStyle(nStyle & ~gsFullHorzLine);
}


//  ---
void CStylesPageDlg::OnCheckFullVertLine() 
{
	BOOL bSuccess;
	UINT nStyle;

	bSuccess = UpdateData();
	ASSERT(bSuccess);

	nStyle = m_pGrid->GetGridStyle();

	if (m_bFullVertLine)
		m_pGrid->SetGridStyle(nStyle | gsFullVertLine);
	else
		m_pGrid->SetGridStyle(nStyle & ~gsFullVertLine);
}


//  ---
void CStylesPageDlg::OnCheckListView()
{
	BOOL bSuccess;
	UINT nStyle;

	bSuccess = UpdateData();
	ASSERT(bSuccess);

	nStyle = m_pGrid->GetGridStyle();

	if (m_bListView)
		m_pGrid->SetGridStyle(nStyle | gsListViewLike);
	else
		m_pGrid->SetGridStyle(nStyle & ~gsListViewLike);
}


//  ----
void CStylesPageDlg::OnCheckDotLines()
{
	BOOL bSuccess;
	UINT nStyle;

	bSuccess = UpdateData();
	ASSERT(bSuccess);

	nStyle = m_pGrid->GetGridStyle();

	if (m_bDotLines)
		m_pGrid->SetGridStyle(nStyle | gsDotLines);
	else
		m_pGrid->SetGridStyle(nStyle & ~gsDotLines);
}

