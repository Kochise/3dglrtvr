// GeneralPageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "virtualgriddemo.h"
#include "GeneralPageDlg.h"
#include "VirtualGridCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGeneralPageDlg dialog


CGeneralPageDlg::CGeneralPageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGeneralPageDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGeneralPageDlg)
	m_bAllowEdit = FALSE;
	m_bAllowSelect = FALSE;
	m_bAlwaysEdit = FALSE;
	m_bAlwaysSelected = FALSE;
	m_bCancelOnExit = FALSE;
	m_bCheckBoxes = FALSE;
	m_nCheckStyle = -1;
	m_bRowSelect = FALSE;
	m_bReadOnly = FALSE;
	m_bDoubleBuffered = FALSE;
	m_bGridLines = FALSE;
	//}}AFX_DATA_INIT
}


void CGeneralPageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGeneralPageDlg)
	DDX_Check(pDX, IDC_CHECK_ALLOW_EDIT, m_bAllowEdit);
	DDX_Check(pDX, IDC_CHECK_ALLOW_SELECT, m_bAllowSelect);
	DDX_Check(pDX, IDC_CHECK_ALWAYS_EDIT, m_bAlwaysEdit);
	DDX_Check(pDX, IDC_CHECK_ALWAYS_SELECTED, m_bAlwaysSelected);
	DDX_Check(pDX, IDC_CHECK_CANCEL_ON_EXIT, m_bCancelOnExit);
	DDX_Check(pDX, IDC_CHECK_CHECK_BOXES, m_bCheckBoxes);
	DDX_CBIndex(pDX, IDC_COMBO_CHECK_STYLE, m_nCheckStyle);
	DDX_Check(pDX, IDC_CHECK_ROW_SELECT, m_bRowSelect);
	DDX_Check(pDX, IDC_CHECK_READ_ONLY, m_bReadOnly);
	DDX_Check(pDX, IDC_CHECK_DOUBLE_BUFF, m_bDoubleBuffered);
	DDX_Check(pDX, IDC_CHECK_GRID_LINES, m_bGridLines);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGeneralPageDlg, CDialog)
	//{{AFX_MSG_MAP(CGeneralPageDlg)
	ON_BN_CLICKED(IDC_CHECK_ALLOW_EDIT, OnCheckAllowEdit)
	ON_BN_CLICKED(IDC_CHECK_ROW_SELECT, OnCheckRowSelect)
	ON_BN_CLICKED(IDC_CHECK_ALLOW_SELECT, OnCheckAllowSelect)
	ON_BN_CLICKED(IDC_CHECK_ALWAYS_EDIT, OnCheckAlwaysEdit)
	ON_BN_CLICKED(IDC_CHECK_ALWAYS_SELECTED, OnCheckAlwaysSelected)
	ON_BN_CLICKED(IDC_CHECK_CANCEL_ON_EXIT, OnCheckCancelOnExit)
	ON_BN_CLICKED(IDC_CHECK_CHECK_BOXES, OnCheckCheckBoxes)
	ON_CBN_SELCHANGE(IDC_COMBO_CHECK_STYLE, OnSelchangeComboCheckStyle)
	ON_BN_CLICKED(IDC_CHECK_READ_ONLY, OnCheckReadOnly)
	ON_BN_CLICKED(IDC_CHECK_DOUBLE_BUFF, OnCheckDoubleBuff)
	ON_BN_CLICKED(IDC_CHECK_GRID_LINES, OnCheckGridLines)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGeneralPageDlg message handlers


//  ----------  To allow or not to allow edit the grid  --------------
void CGeneralPageDlg::OnCheckAllowEdit()
{
	BOOL bSuccess;

	bSuccess = UpdateData();
	ASSERT(bSuccess);

	m_pGrid->SetAllowEdit(m_bAllowEdit);
}




//  -----
void CGeneralPageDlg::OnCheckRowSelect() 
{
	BOOL bSuccess;

	bSuccess = UpdateData();
	ASSERT(bSuccess);

	m_pGrid->SetRowSelect(m_bRowSelect);
}


//  ---
void CGeneralPageDlg::OnCheckAllowSelect()
{
	BOOL bSuccess;

	bSuccess = UpdateData();
	ASSERT(bSuccess);

	m_pGrid->SetAllowSelect(m_bAllowSelect);
}


//  ---
void CGeneralPageDlg::OnCheckAlwaysEdit()
{
	BOOL bSuccess;

	bSuccess = UpdateData();
	ASSERT(bSuccess);

	m_pGrid->SetAlwaysEdit(m_bAlwaysEdit);
}


//  ---
void CGeneralPageDlg::OnCheckAlwaysSelected()
{
	BOOL bSuccess;

	bSuccess = UpdateData();
	ASSERT(bSuccess);

	m_pGrid->SetAlwaysSelected(m_bAlwaysSelected);
}


//  ----
void CGeneralPageDlg::OnCheckCancelOnExit()
{
	BOOL bSuccess;

	bSuccess = UpdateData();
	ASSERT(bSuccess);

	m_pGrid->SetCancelOnExit(m_bCancelOnExit);
}


//  ----
void CGeneralPageDlg::OnCheckCheckBoxes()
{
	BOOL bSuccess;

	bSuccess = UpdateData();
	ASSERT(bSuccess);

	m_pGrid->SetCheckBoxes(m_bCheckBoxes);
}


//  ---
void CGeneralPageDlg::OnSelchangeComboCheckStyle()
{
	BOOL bSuccess;

	bSuccess = UpdateData();
	ASSERT(bSuccess);

	m_pGrid->SetCheckStyle((GridCheckStyle)m_nCheckStyle);
}



//  -----
void CGeneralPageDlg::OnCheckReadOnly()
{
	BOOL bSuccess;

	bSuccess = UpdateData();
	ASSERT(bSuccess);

	m_pGrid->SetReadOnly(m_bReadOnly);
}


//  ---
void CGeneralPageDlg::OnCheckDoubleBuff()
{
	BOOL bSuccess;

	bSuccess = UpdateData();
	ASSERT(bSuccess);
	
	m_pGrid->SetDoubleBuffered(m_bDoubleBuffered);	
}


//  ---
void CGeneralPageDlg::OnCheckGridLines()
{
	BOOL bSuccess;

	bSuccess = UpdateData();
	ASSERT(bSuccess);

	m_pGrid->SetGridLines(m_bGridLines);
}
