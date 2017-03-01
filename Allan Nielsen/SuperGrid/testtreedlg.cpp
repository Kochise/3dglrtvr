// TestTreeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SuperGrid.h"
#include "TestTreeDlg.h"
#include "NewItem.h"
#include "SuperGridCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestTreeDlg dialog


CTestTreeDlg::CTestTreeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestTreeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestTreeDlg)
	//}}AFX_DATA_INIT
}


void CTestTreeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestTreeDlg)
	DDX_Control(pDX, IDC_STATIC_FRAME, m_Frame);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTestTreeDlg, CDialog)
	//{{AFX_MSG_MAP(CTestTreeDlg)
	ON_BN_CLICKED(IDC_BUTTON_INSERT, OnButtonInsert)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestTreeDlg message handlers

BOOL CTestTreeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CRect rc;
	m_Frame.GetWindowRect(rc);
	ScreenToClient(rc);
	m_Frame.ShowWindow(SW_HIDE);
	if(m_List.GetSafeHwnd())
			m_List.MoveWindow(rc);

	return TRUE;  
	              
}


void CTestTreeDlg::OnButtonInsert() 
{
	int nIndex = m_List.GetSelectedItem();
	if(nIndex!=-1)
	{
		CNewItem dlg;
		if(dlg.DoModal()==IDOK)
			m_List.HowToInsertItemsAfterTheGridHasBeenInitialized(nIndex, dlg.m_strItem);
	}
}


int CTestTreeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_List.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect(0,0,0,0),this,0x1001);	
	m_List.InitializeGrid();
	m_List.SetExtendedStyle(0);
	m_List.ModifyStyleEx(0,WS_EX_CLIENTEDGE);
	return 0;
}


