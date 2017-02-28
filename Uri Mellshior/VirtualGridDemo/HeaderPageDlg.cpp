// HeaderPageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "virtualgriddemo.h"
#include "HeaderPageDlg.h"
#include "VirtualGridCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHeaderPageDlg dialog


CHeaderPageDlg::CHeaderPageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHeaderPageDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHeaderPageDlg)
	m_bAutoHeight = FALSE;
	m_bAutoSynchronize = FALSE;
	m_bFullSynchronizing = FALSE;
	m_bFlat = FALSE;
	m_bGridColor = FALSE;
	m_nSectionHeight = 0;
	//}}AFX_DATA_INIT
}


void CHeaderPageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHeaderPageDlg)
	DDX_Control(pDX, IDC_SPIN_SECTION_HEIGHT, m_spinSectionHeight);
	DDX_Check(pDX, IDC_CHECK_AUTO_HEIGHT, m_bAutoHeight);
	DDX_Check(pDX, IDC_CHECK_AUTO_SYNCHRONIZE, m_bAutoSynchronize);
	DDX_Check(pDX, IDC_CHECK_FULL_SYNCHRONIZING, m_bFullSynchronizing);
	DDX_Check(pDX, IDC_CHECK_FLAT, m_bFlat);
	DDX_Check(pDX, IDC_CHECK_GRID_COLOR, m_bGridColor);
	DDX_Text(pDX, IDC_EDIT_SECTION_HEIGHT, m_nSectionHeight);
	DDV_MinMaxInt(pDX, m_nSectionHeight, 1, 128);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHeaderPageDlg, CDialog)
	//{{AFX_MSG_MAP(CHeaderPageDlg)
	ON_BN_CLICKED(IDC_CHECK_FLAT, OnCheckFlat)
	ON_BN_CLICKED(IDC_CHECK_AUTO_HEIGHT, OnCheckAutoHeight)
	ON_BN_CLICKED(IDC_CHECK_AUTO_SYNCHRONIZE, OnCheckAutoSynchronize)
	ON_BN_CLICKED(IDC_CHECK_FULL_SYNCHRONIZING, OnCheckFullSynchronizing)
	ON_BN_CLICKED(IDC_CHECK_GRID_COLOR, OnCheckGridColor)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SECTION_HEIGHT, OnDeltaposSpinSectionHeight)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHeaderPageDlg message handlers


BOOL CHeaderPageDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_spinSectionHeight.SetRange(1, 256);

	return TRUE;
}


//  ----
void CHeaderPageDlg::OnCheckAutoHeight() 
{
	BOOL bSuccess;
	CGridHeader *pHeader;

	pHeader = m_pGrid->GetHeader();

	bSuccess = UpdateData();
	ASSERT(bSuccess);

	pHeader->SetAutoHeight(m_bAutoHeight);

	if (m_bAutoHeight)  {
		m_spinSectionHeight.SetPos(pHeader->GetSectionHeight());
		m_spinSectionHeight.EnableWindow(FALSE);
	}  else  {
		m_spinSectionHeight.EnableWindow();
	}
}


//  ----
void CHeaderPageDlg::OnCheckAutoSynchronize() 
{
	BOOL bSuccess;
	CGridHeader *pHeader;

	pHeader = m_pGrid->GetHeader();

	bSuccess = UpdateData();
	ASSERT(bSuccess);

	pHeader->SetAutoSynchronize(m_bAutoSynchronize);
}


//  ----
void CHeaderPageDlg::OnCheckFlat() 
{
	BOOL bSuccess;
	CGridHeader *pHeader;

	pHeader = m_pGrid->GetHeader();

	bSuccess = UpdateData();
	ASSERT(bSuccess);

	pHeader->SetFlat(m_bFlat);
}


//  ---
void CHeaderPageDlg::OnCheckFullSynchronizing() 
{
	BOOL bSuccess;
	CGridHeader *pHeader;

	pHeader = m_pGrid->GetHeader();

	bSuccess = UpdateData();
	ASSERT(bSuccess);

	pHeader->SetFullSynchronizing(m_bFullSynchronizing);
}


//  ---
void CHeaderPageDlg::OnCheckGridColor() 
{
	BOOL bSuccess;
	CGridHeader *pHeader;

	pHeader = m_pGrid->GetHeader();

	bSuccess = UpdateData();
	ASSERT(bSuccess);

	pHeader->SetGridColor(m_bGridColor);
}


//  ----
void CHeaderPageDlg::OnDeltaposSpinSectionHeight(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CGridHeader *pHeader;

	pHeader = m_pGrid->GetHeader();

	pHeader->SetSectionHeight(pNMUpDown->iPos + pNMUpDown->iDelta);

	
	*pResult = 0;
}
