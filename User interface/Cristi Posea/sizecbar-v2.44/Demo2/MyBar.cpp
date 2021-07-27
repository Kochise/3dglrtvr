// mybar.cpp : implementation file
//

#include "stdafx.h"
#include "mybar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyBar

CMyBar::CMyBar()
{
}

CMyBar::~CMyBar()
{
}


BEGIN_MESSAGE_MAP(CMyBar, baseCMyBar)
	//{{AFX_MSG_MAP(CMyBar)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMyBar message handlers

int CMyBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (baseCMyBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetSCBStyle(GetSCBStyle() | SCBS_SIZECHILD);

	if (!m_wndChild.Create(WS_CHILD|WS_VISIBLE|
			ES_MULTILINE|ES_WANTRETURN|ES_AUTOVSCROLL,
		CRect(0,0,0,0), this, 123))
		return -1;

	m_wndChild.ModifyStyleEx(0, WS_EX_CLIENTEDGE);

	// older versions of Windows* (NT 3.51 for instance)
	// fail with DEFAULT_GUI_FONT
	if (!m_font.CreateStockObject(DEFAULT_GUI_FONT))
		if (!m_font.CreatePointFont(80, "MS Sans Serif"))
			return -1;

	m_wndChild.SetFont(&m_font);

	return 0;
}
