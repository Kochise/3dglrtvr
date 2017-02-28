// GridListBox.cpp : implementation file
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
#include "GridListBox.h"
#include "VirtualGridCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




/////////////////////////////////////////////////////////////////////////////
// CGridListBox

IMPLEMENT_DYNCREATE(CGridListBox, CListBox)


//  =========================================================================
CGridListBox::CGridListBox()
{
	m_pGrid = NULL;
	m_nLeft = 0;
	m_nTop = 0;
	m_nWidth = 0;
	m_nHeight = 0;
}


//  =========================================================================
CGridListBox::~CGridListBox()
{
}


BEGIN_MESSAGE_MAP(CGridListBox, CListBox)
	//{{AFX_MSG_MAP(CGridListBox)
	ON_WM_CHAR()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridListBox message handlers


//  ------------------------------------------------------------------------
void CGridListBox::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CListBox::OnChar(nChar, nRepCnt, nFlags);
}


//  ------------------------------------------------------------------------
void CGridListBox::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CGridEdit *pEdit;

	if (m_pGrid == NULL)
		return;

	pEdit = m_pGrid->GetEdit();

	if (pEdit && (::IsWindow(pEdit->GetSafeHwnd())))  {
		pEdit->ListLButtonUp(point);
	}

	CListBox::OnLButtonUp(nFlags, point);
}

