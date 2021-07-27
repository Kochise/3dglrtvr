// GridEdit.cpp : implementation file
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
#include "VirtualGridCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CGridEdit

IMPLEMENT_DYNCREATE(CGridEdit, CEdit);


//  ===============================================================================
CGridEdit::CGridEdit()
{
	m_editStyle = geSimple;
	m_nDropDownCount = 8;
	m_nButtonWidth = GetSystemMetrics(SM_CXVSCROLL);
	m_bButtonPressed = FALSE;
	m_bDropListVisible = FALSE;

	m_pActiveList = NULL;
	m_pPickList = NULL;
	m_pGrid = NULL;
}


//  ==============================================================================
CGridEdit::~CGridEdit()
{
	if (m_pPickList != NULL)
		delete m_pPickList;	//  Bug fix: thanks to mr. QuiOui
}


//  -----
CRect CGridEdit::GetButtonRect()
{
	CRect rect;

	GetClientRect(rect);
	rect.left = rect.Width() - m_nButtonWidth;
	rect.top = 0;		// ??
	return rect;
}


//  ---
int CGridEdit::GetLineCount()
{
	//  NYI
	return 1;
}


//  ---
void CGridEdit::SetButtonWidth(int nWidth)
{
	if (m_nButtonWidth == nWidth)
		return;		//  Nothing to change

	m_nButtonWidth = nWidth;
	Invalidate();
}


//  --
CWnd * CGridEdit::GetDropList()
{
	BOOL bSuccess;

	if (m_pPickList)
		return m_pPickList;

	m_pPickList = new CGridListBox;

	m_pPickList->m_pGrid = m_pGrid;

	CRect rect(0, 0, 0, 0);

	bSuccess = m_pPickList->Create(
		WS_CHILD | WS_BORDER | LBS_WANTKEYBOARDINPUT,
		rect,
		m_pGrid,
		0
		);
	ASSERT(bSuccess);

	return m_pPickList;
}


//  ---
void CGridEdit::SetDropListVisible(BOOL bVisible)
{
	if (bVisible)
		DropDown();
	else
		CloseUp(FALSE);
}


//  ---
void CGridEdit::SetEditStyle(GridEditStyle style)
{
	if (m_editStyle == style)
		return;		//  Nothing to change
	m_editStyle = style;
	Invalidate();
}


//  ---
void CGridEdit::SetWordWrap(BOOL bWordWrap)
{
	if (m_bWordWrap == bWordWrap)
		return;		//  Nothing to change

	m_bWordWrap = bWordWrap;
	//  NYI
}


//  ---
void CGridEdit::CreateButton()
{
	CRect rect;
	BOOL bSuccess;

	if (m_editStyle == geSimple)
		return;


	if (!(::IsWindow(m_button.GetSafeHwnd())))  {
		rect = GetButtonRect();

		bSuccess = m_button.Create(
			_T(""),
			WS_VISIBLE | BS_DEFPUSHBUTTON | BS_OWNERDRAW,
			rect,
			this,
			0
			);
		ASSERT(bSuccess);

		SetMargins(0, rect.Width() + 2);

		CStringArray myStrings;
		m_pGrid->GetEditList(m_pGrid->GetEditCell(), myStrings);
	}
}



//  ---
void CGridEdit::UpdateBounds(BOOL bScrollCaret)
{
	CRect rect;
	CRect workingRect;
	BOOL bSuccess;
	int nLeft, nTop, nHeight, nWidth;
	CPoint textOffset;

	if (m_pGrid == NULL)
		return;		//  Nothing to update

	rect = m_pGrid->GetEditRect(m_pGrid->GetEditCell());
	CRect initialRect(rect);	//  Memorize
	workingRect = m_pGrid->GetFixedRect();

	//  Adjust the line according to fixed part...
	if (rect.left < workingRect.right)
		rect.left = workingRect.right;
	if (rect.right < workingRect.right)
		rect.right = workingRect.right;

	//  Adjust the line according to the header...
	workingRect = m_pGrid->GetHeaderRect();
	if (rect.top < workingRect.bottom)
		rect.top = workingRect.bottom;
	if (rect.bottom < workingRect.bottom)
		rect.bottom = workingRect.bottom;

	//  Set position...
	bSuccess = SetWindowPos(
		&wndTop,
		rect.left,
		rect.top,
		rect.Width(),
		rect.Height(),
		SWP_SHOWWINDOW | SWP_NOREDRAW
		);
	ASSERT(bSuccess);

	//  Calculate new text borders...
	nLeft = initialRect.left - rect.left;
	nTop = initialRect.top - rect.top;
	nWidth = initialRect.Width();
	nHeight = initialRect.Height();		//  Bug fix: thanks to mr. QuiOui

	textOffset = m_pGrid->GetCellTextIndent(m_pGrid->GetEditCell());

	//  Taking the button into account...
	if (m_editStyle != geSimple)
		nWidth -= (m_nButtonWidth + 1);
	else
		nWidth -= m_pGrid->GetTextRightIndent();

	//  Set the text borders...
	rect.left = nLeft + textOffset.x;
	rect.top = nTop + textOffset.y;
	rect.right = nWidth - textOffset.x + (m_nAlignment == LVCFMT_RIGHT);
	rect.bottom = nHeight;

	SendMessage(EM_SETRECTNP, 0, (LPARAM)(LPRECT)rect);

	//  Move cursor at the line's end
	if (bScrollCaret)
		SendMessage(EM_SCROLLCARET, 0, 0);
}


//  ---
void CGridEdit::UpdateColors()
{
	//  NYI
}


//  ----
void CGridEdit::UpdateContents()
{
	CGridColumn *pColumn;

	if (m_pGrid == NULL || !m_pGrid->IsCellValid(m_pGrid->GetEditCell()))
		return;		//  Nothing to update

	pColumn = m_pGrid->GetColumn(m_pGrid->GetEditCell().m_nColumn);

	m_nMaxLength = pColumn->GetMaxLength();
	m_bReadOnly = pColumn->GetReadOnly();
	m_bWantReturns = pColumn->GetWantReturns();
	m_bWordWrap = pColumn->GetWordWrap();
	m_nAlignment = pColumn->GetAlignment();
	SetWindowText(m_pGrid->GetEditText(m_pGrid->GetEditCell()));
	SetSel(0, -1);
}



//  --
void CGridEdit::UpdateListValue(BOOL bAccept)
{
	int nIndex;
	CGridListBox *pBox;

	if (m_pActiveList == NULL)
		return;

	if (!m_pActiveList->IsKindOf(RUNTIME_CLASS(CGridListBox)))
		return;

	pBox = (CGridListBox *)m_pActiveList;

	nIndex = pBox->GetCurSel();

	if (m_pGrid)
		m_pGrid->EditCloseUp(m_pGrid->GetEditCell(), nIndex, bAccept);

	if (bAccept && nIndex > - 1)  {
		CString strText;

		pBox->GetText(nIndex, strText);
		SetWindowText(strText);
		SetSel(0, -1);
	}
}



//  ---
void CGridEdit::UpdateStyle()
{
	GridEditStyle style;

	style = geSimple;

	if (m_pGrid && !m_pGrid->GetReadOnly())
		style = m_pGrid->GetEditStyle(m_pGrid->GetEditCell());

	SetEditStyle(style);
}


//  ---
void CGridEdit::CloseUp(BOOL bAccept)
{
	const UINT nFlags = SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_HIDEWINDOW;
	CWnd *pWnd;
	BOOL bSuccess;

	if (!m_bDropListVisible)
		return;		//  Nothing to close

	pWnd = GetCapture();
	if (pWnd)
		pWnd->SendMessage(WM_CANCELMODE, 0, 0);

	//  Hide the listbox
	bSuccess = m_pActiveList->SetWindowPos(NULL, 0, 0, 0, 0, nFlags);
	ASSERT(bSuccess);

	m_bDropListVisible = FALSE;
	
	Invalidate();

	//  Set selected value
	UpdateListValue(bAccept);
}



//  ----
void CGridEdit::Deselect()
{
	SetSel(0, -1);
}


//  ---
void CGridEdit::DropDown()
{
	const UINT nFlags = SWP_NOACTIVATE | SWP_SHOWWINDOW;
	CGridListBox *pBox;
	BOOL bSuccess;
	CWnd *pWnd;


	if (m_bDropListVisible && m_pGrid == NULL)
		return;

	if (!(m_editStyle & gePickList) && !(m_editStyle & geDataList))
		return;

	m_pActiveList = GetDropList();
	if (m_pActiveList == NULL)
		return;		//  Nothing to dropdown

	if (!m_pActiveList->IsKindOf(RUNTIME_CLASS(CGridListBox)))
		return;

	pBox = (CGridListBox *)m_pActiveList;

	//  Update the list box and set its size
	UpdateListItems();
	UpdateListBounds();

	//  Show the list box in the correct place
	bSuccess = pBox->SetWindowPos(
		&wndTop,
		pBox->m_nLeft,
		pBox->m_nTop,
		pBox->m_nWidth,
		pBox->m_nHeight,
		nFlags
		);
	ASSERT(bSuccess);

	m_bDropListVisible = TRUE;

	Invalidate();

	pWnd = SetFocus();
	pBox->SetCurSel(0);
}



//  ---
void CGridEdit::Hide()
{
	if (m_pGrid && IsWindowVisible())  {
		BOOL bSuccess;
		CWnd *pWnd;

		if (::IsWindow(m_button.GetSafeHwnd()))  {
			bSuccess = m_button.DestroyWindow();
			ASSERT(bSuccess);
			SetMargins(0, 0);
		}

		//  Switch off the editing flag
		m_pGrid->m_bEditing = FALSE;

		//  Hide edit
		Invalidate();
		bSuccess = SetWindowPos(
			NULL,
			0,
			0,
			0,
			0,
			SWP_HIDEWINDOW | SWP_NOZORDER | SWP_NOREDRAW
			);

		//  Remove focus
		pWnd = GetFocus();
		if (pWnd == this)  {
			m_bDefocusing = TRUE;

			__try
			{
				pWnd = m_pGrid->SetFocus();
			}
			__finally
			{
				m_bDefocusing = FALSE;
			}
		}
	}
}


//  ---
void CGridEdit::Press()
{
	if (m_pGrid)
		m_pGrid->EditButtonPress(m_pGrid->GetEditCell());
}


//  ---
void CGridEdit::SelectNext()
{
}


//  ----
void CGridEdit::Show()
{
	BOOL bScrollCaret;

	if (m_pGrid == NULL)
		return;

	bScrollCaret = !m_pGrid->GetEditing();
	m_pGrid->m_bEditing = TRUE;
	m_pGrid->m_bCellSelected = TRUE;

	UpdateColors();

	UpdateBounds(bScrollCaret);
	if (m_pGrid == GetFocus())
		SetFocus();

	CDC *pDC;

	pDC = GetDC();

	pDC->ExcludeClipRect(GetButtonRect());

	CreateButton();
	ReleaseDC(pDC);
}





//  ----
void CGridEdit::ListLButtonUp(CPoint point)
{
	if (m_pActiveList == NULL)
		return;

	CRect rect;

	m_pActiveList->GetClientRect(rect);

	CloseUp(rect.PtInRect(point));
}




//  ---
void CGridEdit::UpdateListItems()
{
	CGridListBox *pBox;
	int i;
	int nIndex;

	if (m_pGrid == NULL || m_pActiveList == NULL)
		return;		//  Nothing to update

	if (!m_pActiveList->IsKindOf(RUNTIME_CLASS(CGridListBox)))
		return;		//  Don't know how to update

	pBox = (CGridListBox *)m_pActiveList;
	
	CStringArray items;
	m_pGrid->GetEditList(m_pGrid->GetEditCell(), items);

	pBox->ResetContent();

	for (i = 0; i < items.GetSize(); i ++)
		nIndex = pBox->AddString(items[i]);

	CString strEditText;
	GetWindowText(strEditText);
	nIndex = pBox->SelectString(-1, strEditText);
}


//  ---
void CGridEdit::UpdateListBounds()
{
	CGridListBox *pBox;
	int i;

	if (m_pGrid == NULL || m_pActiveList == NULL)
		return;		//  No bounds to update

	if (!m_pActiveList->IsKindOf(RUNTIME_CLASS(CGridListBox)))
		return;

	pBox = (CGridListBox *)m_pActiveList;
	pBox->SetFont(GetFont());

	if (pBox->GetCount())  {
		int x;
		CDC *pDC;
		CString strText;

		pBox->m_nWidth = 0;

		pDC = pBox->GetDC();

		for (i = 0; i < pBox->GetCount(); i ++)  {
			pBox->GetText(i, strText);
			x = pDC->GetTextExtent(strText).cx;

			if (pBox->m_nWidth < x)
				pBox->m_nWidth = x;
		}

		i = pBox->ReleaseDC(pDC);
//		pBox->m_nWidth += 6;
	}  else  {
		pBox->m_nWidth = 100;
	}

	//  Let's calculate the height...
	if (m_nDropDownCount < 1 || !pBox->GetCount())
		pBox->m_nHeight =  pBox->GetItemHeight(0) + 6;
	else if (pBox->GetCount() < m_nDropDownCount)
		pBox->m_nHeight = pBox->GetCount() * pBox->GetItemHeight(0) + 6;
	else
		pBox->m_nHeight = m_nDropDownCount * pBox->GetItemHeight(0);

	//  Adjust list size and position according to column size and position
	CRect rect;

	//  Adjust according to column width...
	rect = m_pGrid->GetCellRect(m_pGrid->GetEditCell());
	pBox->m_nWidth = max(pBox->m_nWidth, rect.Width());

	//  Position...
	pBox->m_nLeft = rect.left + rect.Width() - pBox->m_nWidth - 1;
	pBox->m_nTop = rect.top + rect.Height() - 1;
}



//////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CGridEdit, CEdit)
	//{{AFX_MSG_MAP(CGridEdit)
	ON_WM_GETDLGCODE()
	ON_WM_KILLFOCUS()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SETCURSOR()
	ON_WM_CONTEXTMENU()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETFOCUS()
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridEdit message handlers



//  -----------------------------------------------------------------------
UINT CGridEdit::OnGetDlgCode() 
{
	UINT nCode;

	nCode = DLGC_WANTARROWS | DLGC_WANTCHARS;
	if (m_pGrid && (gkTabs & m_pGrid->m_nCursorKeys))
		nCode |= DLGC_WANTTAB;

	return DLGC_WANTALLKEYS;
}



//  ---------------------------------------------------------------------
void CGridEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
	if (pNewWnd == &m_button || pNewWnd == m_pPickList)
		return;

	CloseUp(FALSE);
	if (m_pGrid && ::IsWindow(m_pGrid->GetSafeHwnd()))
		m_pGrid->PostMessage(VGM_EXIT);
}



//  ------------------------------------------------------------------
void CGridEdit::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CEdit::OnWindowPosChanged(lpwndpos);
	Invalidate();
}


//  -----------------------------------------------------------------
void CGridEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_bDropListVisible)
		CloseUp(FALSE);
	
	CEdit::OnLButtonDown(nFlags, point);
}


//  ---------------------------------------------------------------
void CGridEdit::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CEdit::OnLButtonDblClk(nFlags, point);
}


//  -------------------------------------------------------------
BOOL CGridEdit::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CEdit::OnSetCursor(pWnd, nHitTest, message);
}



void CGridEdit::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	
}



//  ------------------------------------------------------------------
void CGridEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch (nChar)  {
	case VK_UP:
	case VK_DOWN:
		if (IS_CTRL_PRESSED() || (!(m_bWantReturns || m_bWordWrap)))
			m_pGrid->CancelEdit();
		break;
	case VK_RETURN:
		m_pGrid->SetEditing(FALSE);
		break;
	default:
		break;
	}
	
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}


//  ----------------------------------------------------------------
void CGridEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (m_pGrid)
		m_pGrid->OnKeyUp(nChar, nRepCnt, nFlags);
}


//  ---------------------------------------------------------------
void CGridEdit::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CEdit::OnLButtonUp(nFlags, point);
}


//  -------------------------------------------------------------
void CGridEdit::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CEdit::OnMouseMove(nFlags, point);
}


//  --------------------------------------------------------------
void CGridEdit::OnSetFocus(CWnd* pOldWnd) 
{
	CEdit::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here
	
}


//  -------
BOOL CGridEdit::EditCanModify()
{
	if (m_pGrid == NULL)
		return FALSE;

	return m_pGrid->EditCanModify(m_pGrid->GetEditCell());
}


//  --------
void CGridEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (EditCanModify())
		CEdit::OnChar(nChar, nRepCnt, nFlags);
}
