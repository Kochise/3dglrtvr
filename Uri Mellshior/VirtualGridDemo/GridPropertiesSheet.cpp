// GridPropertiesSheet.cpp : implementation file
//

#include "stdafx.h"
#include "VirtualGridDemo.h"
#include "GridPropertiesSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGridPropertiesSheet

IMPLEMENT_DYNAMIC(CGridPropertiesSheet, CPropertySheet)

CGridPropertiesSheet::CGridPropertiesSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}


//  =====================================================================================
CGridPropertiesSheet::CGridPropertiesSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	m_psh.dwFlags &= ~PSH_HASHELP;

	AddPage(&m_columnsPage);
	AddPage(&m_headersPage);
	AddPage(&m_codeGenPage);
}

CGridPropertiesSheet::~CGridPropertiesSheet()
{
}


BEGIN_MESSAGE_MAP(CGridPropertiesSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CGridPropertiesSheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridPropertiesSheet message handlers
