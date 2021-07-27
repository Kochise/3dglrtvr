// RowsPageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "virtualgriddemo.h"
#include "RowsPageDlg.h"
#include "VirtualGridCtrl.h"
#include "VirtualGridDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRowsPageDlg dialog


CRowsPageDlg::CRowsPageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRowsPageDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRowsPageDlg)
	m_bAutoHeight = FALSE;
	m_nCount = 0;
	m_nHeight = 0;
	//}}AFX_DATA_INIT
}


void CRowsPageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRowsPageDlg)
	DDX_Control(pDX, IDC_SPIN_HEIGHT, m_spinHeight);
	DDX_Control(pDX, IDC_SPIN_COUNT, m_spinCount);
	DDX_Check(pDX, IDC_CHECK_AUTO_HEIGHT, m_bAutoHeight);
	DDX_Text(pDX, IDC_EDIT_COUNT, m_nCount);
	DDV_MinMaxInt(pDX, m_nCount, 0, 1024);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_nHeight);
	DDV_MinMaxInt(pDX, m_nHeight, 1, 128);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRowsPageDlg, CDialog)
	//{{AFX_MSG_MAP(CRowsPageDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_COUNT, OnDeltaposSpinCount)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_HEIGHT, OnDeltaposSpinHeight)
	ON_BN_CLICKED(IDC_CHECK_AUTO_HEIGHT, OnCheckAutoHeight)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRowsPageDlg message handlers

BOOL CRowsPageDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_spinCount.SetRange(0, 1024);
	m_spinHeight.SetRange(1, 512);

	return TRUE;
}


//  --------
void CRowsPageDlg::OnDeltaposSpinCount(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	int nCount = pNMUpDown->iPos + pNMUpDown->iDelta;

	m_pDemoData->SetSize(nCount);
	m_pGrid->SetRowCount(nCount);
	
	*pResult = 0;
}


//  ---------
void CRowsPageDlg::OnDeltaposSpinHeight(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	m_pGrid->SetRowHeight(pNMUpDown->iPos + pNMUpDown->iDelta);
	
	*pResult = 0;
}


//  ----
void CRowsPageDlg::OnCheckAutoHeight() 
{
	BOOL bSuccess;
	CGridRows *pRows;

	bSuccess = UpdateData();
	ASSERT(bSuccess);

	pRows = m_pGrid->GetRows();

	pRows->SetAutoHeight(m_bAutoHeight);
	if (m_bAutoHeight)  {
		m_spinHeight.SetPos(pRows->GetHeight());
		m_spinHeight.EnableWindow(FALSE);
	}  else  {
		m_spinHeight.EnableWindow();
	}
}

