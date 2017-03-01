// NewItem.cpp : implementation file
//

#include "stdafx.h"
#include "SuperGrid.h"
#include "NewItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewItem dialog


CNewItem::CNewItem(CWnd* pParent /*=NULL*/)
	: CDialog(CNewItem::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewItem)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CNewItem::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewItem)
	DDX_Control(pDX, IDC_EDIT1, m_Edit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewItem, CDialog)
	//{{AFX_MSG_MAP(CNewItem)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewItem message handlers

void CNewItem::OnOK() 
{
	// TODO: Add extra validation here
	m_Edit.GetWindowText(m_strItem);	
	CDialog::OnOK();
}
