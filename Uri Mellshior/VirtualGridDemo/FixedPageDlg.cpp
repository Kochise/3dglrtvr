// FixedPageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "virtualgriddemo.h"
#include "FixedPageDlg.h"
#include "VirtualGridCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFixedPageDlg dialog


CFixedPageDlg::CFixedPageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFixedPageDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFixedPageDlg)
	m_bFlat = FALSE;
	m_bGridColor = FALSE;
	m_nCount = 0;
	//}}AFX_DATA_INIT
}


void CFixedPageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFixedPageDlg)
	DDX_Control(pDX, IDC_SPIN_COUNT, m_spinCount);
	DDX_Check(pDX, IDC_CHECK_FLAT, m_bFlat);
	DDX_Check(pDX, IDC_CHECK_GRID_COLOR, m_bGridColor);
	DDX_Text(pDX, IDC_EDIT_COUNT, m_nCount);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFixedPageDlg, CDialog)
	//{{AFX_MSG_MAP(CFixedPageDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_COUNT, OnDeltaposSpinCount)
	ON_BN_CLICKED(IDC_CHECK_FLAT, OnCheckFlat)
	ON_BN_CLICKED(IDC_CHECK_GRID_COLOR, OnCheckGridColor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFixedPageDlg message handlers

BOOL CFixedPageDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_spinCount.SetRange(0, 4);
	
	return TRUE;
}


//  ---
void CFixedPageDlg::OnDeltaposSpinCount(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CGridFixed *pFixed = m_pGrid->GetFixed();

	pFixed->SetCount(pNMUpDown->iPos + pNMUpDown->iDelta);
	
	*pResult = 0;
}


//  ---
void CFixedPageDlg::OnCheckFlat() 
{
	BOOL bSuccess;
	CGridFixed *pFixed = m_pGrid->GetFixed();

	bSuccess = UpdateData();
	ASSERT(bSuccess);

	pFixed->SetFlat(m_bFlat);
}


//  --
void CFixedPageDlg::OnCheckGridColor() 
{
	BOOL bSuccess;
	CGridFixed *pFixed = m_pGrid->GetFixed();

	bSuccess = UpdateData();
	ASSERT(bSuccess);

	pFixed->SetGridColor(m_bGridColor);
}
