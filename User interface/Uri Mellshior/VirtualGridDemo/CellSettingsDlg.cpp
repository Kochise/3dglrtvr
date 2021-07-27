// CellSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VirtualGridDemo.h"
#include "CellSettingsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCellSettingsDlg dialog


CCellSettingsDlg::CCellSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCellSettingsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCellSettingsDlg)
	m_strCellText = _T("");
	//}}AFX_DATA_INIT
}


void CCellSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCellSettingsDlg)
	DDX_Control(pDX, IDC_BUTTON_TEXT_COLOR, m_buttonTextColor);
	DDX_Control(pDX, IDC_BUTTON_BK_COLOR, m_buttonBkColor);
	DDX_Text(pDX, IDC_EDIT_CELL_TEXT, m_strCellText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCellSettingsDlg, CDialog)
	//{{AFX_MSG_MAP(CCellSettingsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCellSettingsDlg message handlers

BOOL CCellSettingsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_buttonBkColor.SetColor(m_crBkColor);
	
	return TRUE;
}

//  ---------
void CCellSettingsDlg::OnOK() 
{
	BOOL bSuccess;

	bSuccess = UpdateData();
	ASSERT(bSuccess);

	m_crBkColor = m_buttonBkColor.GetColor();
	m_crTextColor = m_buttonTextColor.GetColor();

	CDialog::OnOK();
}
