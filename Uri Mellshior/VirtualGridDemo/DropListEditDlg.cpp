// DropListEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VirtualGridDemo.h"
#include "DropListEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDropListEditDlg dialog


CDropListEditDlg::CDropListEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDropListEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDropListEditDlg)
	m_strDropList = _T("");
	//}}AFX_DATA_INIT
}


void CDropListEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDropListEditDlg)
	DDX_Control(pDX, IDC_EDIT_DROP_LIST, m_editDropList);
	DDX_Text(pDX, IDC_EDIT_DROP_LIST, m_strDropList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDropListEditDlg, CDialog)
	//{{AFX_MSG_MAP(CDropListEditDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDropListEditDlg message handlers

void CDropListEditDlg::OnOK() 
{
	int i;
	int nLength;
	TCHAR szBuffer[1024];		//  Enough?

	for (i = 0; i < m_editDropList.GetLineCount(); i ++)  {
		nLength = m_editDropList.GetLine(i, szBuffer, 1024);
		if (nLength)
			m_pickList.Add(szBuffer);
	}
	
	CDialog::OnOK();
}
