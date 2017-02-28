// GridButton.cpp : implementation file
//

//  -----------  For use with CVirtualGridCtrl  --------------

// Written by Uri Mellshior <urizabr@hotmail.com>
// Copyright (c) 2002-2003. All Rights Reserved.
// Based on Delphi TGridView component written by Roman Mochalov, (c) 1997-2002.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name and all copyright 
// notices remains intact.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.


#include "stdafx.h"
#include "GridButton.h"
#include "VirtualGridCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGridButton

IMPLEMENT_DYNCREATE(CGridButton, CButton);


//  ========================================================================
CGridButton::CGridButton()
{
	m_bPressed = FALSE;
}



//  =======================================================================
CGridButton::~CGridButton()
{
}


BEGIN_MESSAGE_MAP(CGridButton, CButton)
	//{{AFX_MSG_MAP(CGridButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridButton message handlers



void CGridButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	UINT nFlags = 0;
	BOOL bSuccess;
	CDC *pDC;
	CGridEdit *pEdit;

	pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	pEdit = (CGridEdit *)GetParent();

	switch (pEdit->m_editStyle)  {
	case geSimple:
		break;
	case geEllipsis:
	{
		if (m_bPressed)
			nFlags = BF_FLAT;

		bSuccess = pDC->DrawEdge(&lpDrawItemStruct->rcItem, EDGE_RAISED, BF_RECT | BF_MIDDLE | nFlags);

		nFlags = (lpDrawItemStruct->rcItem.right - lpDrawItemStruct->rcItem.left) / 2 - 1 + m_bPressed;

		bSuccess = pDC->PatBlt(
			lpDrawItemStruct->rcItem.left + nFlags,
			lpDrawItemStruct->rcItem.top + nFlags,
			2,
			2,
			BLACKNESS
			);
		ASSERT(bSuccess);

		bSuccess = pDC->PatBlt(
			lpDrawItemStruct->rcItem.left + nFlags - 3,
			lpDrawItemStruct->rcItem.top + nFlags,
			2,
			2,
			BLACKNESS
			);
		ASSERT(bSuccess);

		bSuccess = pDC->PatBlt(
			lpDrawItemStruct->rcItem.left + nFlags + 3,
			lpDrawItemStruct->rcItem.top + nFlags,
			2,
			2,
			BLACKNESS
			);
		ASSERT(bSuccess);
		break;
	}
	case geDataList:
	case gePickList:
	{
		int dx;

		if (m_bPressed)
			nFlags = BF_FLAT;

		bSuccess = pDC->DrawEdge(
			&lpDrawItemStruct->rcItem,
			EDGE_RAISED,
			BF_RECT | BF_MIDDLE | nFlags
			);
		ASSERT(bSuccess);

		nFlags = (lpDrawItemStruct->rcItem.right - lpDrawItemStruct->rcItem.left) / 2 - 1 + m_bPressed;
		dx = div(lpDrawItemStruct->rcItem.right - lpDrawItemStruct->rcItem.left, 2).rem - 1;

		bSuccess = pDC->PatBlt(
			lpDrawItemStruct->rcItem.left + nFlags - 2 + dx,
			lpDrawItemStruct->rcItem.top + nFlags - 1,
			7,
			1,
			BLACKNESS
			);
		ASSERT(bSuccess);

		bSuccess = pDC->PatBlt(
			lpDrawItemStruct->rcItem.left + nFlags - 1 + dx,
			lpDrawItemStruct->rcItem.top + nFlags,
			5,
			1,
			BLACKNESS
			);
		ASSERT(bSuccess);

		bSuccess = pDC->PatBlt(
			lpDrawItemStruct->rcItem.left + nFlags + dx,
			lpDrawItemStruct->rcItem.top + nFlags + 1,
			3,
			1,
			BLACKNESS
			);
		ASSERT(bSuccess);

		bSuccess = pDC->PatBlt(
			lpDrawItemStruct->rcItem.left + nFlags + 1 + dx,
			lpDrawItemStruct->rcItem.top + nFlags + 2,
			1,
			1,
			BLACKNESS
			);
		ASSERT(bSuccess);



		break;
	}
	default:
		break;
	}
}





//  --------------------------------------------------------------------
void CGridButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CGridEdit *pEdit;
	m_bPressed = TRUE;
	Invalidate();

	pEdit = (CGridEdit *)GetParent();
	ASSERT(pEdit);
	ASSERT(::IsWindow(pEdit->GetSafeHwnd()));

	if (pEdit->m_editStyle != geEllipsis)  {
		if (pEdit->m_bDropListVisible)
			pEdit->CloseUp(FALSE);
		else
			pEdit->DropDown();
	}
	
	CButton::OnLButtonDown(nFlags, point);
}


//  -------------------------------------------------------------------
void CGridButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CGridEdit *pEdit;
	CGridCell cell;

	m_bPressed = FALSE;
	pEdit = (CGridEdit *)GetParent();

	cell = pEdit->m_pGrid->GetCellFocused();

	if (pEdit->m_pGrid && !cell.IsEmpty() && pEdit->m_editStyle == geEllipsis)  {
		CWnd *pWnd;
		LRESULT lResult;
		VG_DISPINFO dispInfo;

		pWnd = pEdit->m_pGrid->GetParent();
		ASSERT(pWnd);
		ASSERT(::IsWindow(pWnd->GetSafeHwnd()));


		dispInfo.hdr.code = VGN_EDITBUTTONPRESS;
		dispInfo.hdr.hwndFrom = pEdit->m_pGrid->m_hWnd;
		dispInfo.hdr.idFrom = pEdit->m_pGrid->GetDlgCtrlID();

		dispInfo.item.nColumn = cell.m_nColumn;
		dispInfo.item.nRow = cell.m_nRow;

		lResult = pWnd->SendMessage(WM_NOTIFY, dispInfo.hdr.idFrom, (LPARAM)&dispInfo);
	}

	CButton::OnLButtonUp(nFlags, point);
}


//  ----------------------------------------------------------------------------
void CGridButton::OnKillFocus(CWnd* pNewWnd) 
{
	CGridEdit *pEdit;

	CButton::OnKillFocus(pNewWnd);

	if (pNewWnd == GetParent())
		return;
	
	pEdit = (CGridEdit *)GetParent();
	if (pNewWnd == pEdit->m_pActiveList)
		return;
	
	if (pEdit->m_pPickList)
		pEdit->CloseUp(FALSE);
}

