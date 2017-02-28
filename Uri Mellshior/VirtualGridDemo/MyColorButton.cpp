// ColorButton.cpp : implementation file
//

#include "stdafx.h"
#include "virtualgriddemo.h"
#include "MyColorButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyColorButton

CMyColorButton::CMyColorButton()
{
	m_crColor = RGB(0, 0, 0);
}

CMyColorButton::~CMyColorButton()
{
}


BEGIN_MESSAGE_MAP(CMyColorButton, CButton)
	//{{AFX_MSG_MAP(CMyColorButton)
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyColorButton message handlers

void CMyColorButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CRect rect;
	CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	GetClientRect(rect);
	pDC->FillSolidRect(rect, m_crColor);
	
}

void CMyColorButton::SetColor(COLORREF crNewColor)
{
	m_crColor = crNewColor;
	Invalidate();
}

COLORREF CMyColorButton::GetColor()
{
	return m_crColor;
}


//  ---------------------------------------------
void CMyColorButton::OnClicked() 
{
	int nRetVal;
	CColorDialog dlg;

	nRetVal = dlg.DoModal();
	if (nRetVal == IDOK)  {
		m_crColor = dlg.GetColor();
		Invalidate();
	}
}


