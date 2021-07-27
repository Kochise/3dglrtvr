// VirtualGridCtrl.cpp : implementation file
//


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
#include "resource.h"
#include "VirtualGridCtrl.h"
#include "MemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVirtualGridCtrl

IMPLEMENT_DYNCREATE(CVirtualGridCtrl, CWnd);

CVirtualGridCtrl::CVirtualGridCtrl()
{
	HINSTANCE hInstance;
	hInstance = AfxGetInstanceHandle();
	WNDCLASS wc;
	BOOL bSuccess;

	if (!(::GetClassInfo(hInstance, VIRTUAL_GRID_CLASS_NAME, &wc)))  {
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
		wc.hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		wc.hIcon = NULL;
		wc.hInstance = hInstance;
		wc.lpfnWndProc = ::DefWindowProc;
		wc.lpszClassName = VIRTUAL_GRID_CLASS_NAME;
		wc.lpszMenuName = NULL;
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	
		if (!AfxRegisterClass(&wc))  {
			AfxThrowResourceException();
			return;
		}
	}

	m_header.m_pGrid = this;
	m_columns.m_pGrid = this;
	m_rows.m_pGrid = this;
	m_fixed.m_pGrid = this;
	m_bShowHeader = TRUE;
	m_bGridLines = TRUE;
	m_nGridLineWidth = 1;		//  Don't change

	m_gridColor = ::GetSysColor(COLOR_WINDOW);	//  ???
	m_bEndEllipsis = TRUE;
	m_nImageLeftIndent = 2;
	m_nImageTopIndent = 1;
	m_nTextLeftIndent = 6;
	m_nTextRightIndent = 6;
	m_nTextTopIndent = 2;
	m_bShowFocusRect = TRUE;
	m_bRightClickSelect = TRUE;
	m_bAllowSelect = TRUE;

	m_nCursorKeys = gkArrows | gkMouse | gkMouseWheel;
	m_bColumnsSizing = TRUE;
	m_bColumnClick = TRUE;

	m_editCell.m_nColumn = -1;
	m_editCell.m_nRow = -1;

	m_checkStyle = cs3D;
	m_nCheckWidth = 16;
	m_nCheckHeight = 16;
	m_nCheckLeftIndent = 0;
	m_nCheckTopIndent = 0;

	m_bColResizing = FALSE;		//  !!!!!!!!!!!

//	Bitmaps, bitmaps...

	m_nSortLeftIndent = 10;
	m_nSortTopIndent = 0;

//	Sort bitmaps, sort bitmaps...
	m_bCancelOnExit = TRUE;

	//  ------------
	bSuccess = m_penBtnShadow.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_BTNSHADOW));
	ASSERT(bSuccess);

	bSuccess = m_penBtnHighlight.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_BTNHIGHLIGHT));
	ASSERT(bSuccess);

	bSuccess = m_penGridLines.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_ACTIVEBORDER));
	ASSERT(bSuccess);

	m_nUpdateLock = 0;	//  !!!!
	m_bColumnsFullDrag = FALSE;
	m_bEditing = FALSE;
	m_nGridStyles = gsHorzLine | gsVertLine;
	m_bRowSelect = FALSE;
	m_bAlwaysEdit = FALSE;

	//  Scroll bar data
	m_nPageStep = 100;
	m_nLineStep = 8;
	m_nLineSize = 1;

	m_pEdit = NULL;
	m_bCellSelected = FALSE;
	m_bImageHighlight = 0;
	m_bAllowEdit = FALSE;		//  Such a passive grid
	m_bReadOnly = FALSE;

	m_fixed.m_bGridColor = FALSE;
	m_bTitleTips = TRUE;		//  Show title tips by default

	m_bHeaderClicking = FALSE;
	m_bHeaderClickState = FALSE;
	m_pHeaderClickSection = NULL;
	m_bCheckBoxes = FALSE;

	m_bDoubleBuffered = TRUE;
}



//  ===================================================================
CVirtualGridCtrl::~CVirtualGridCtrl()
{
	BOOL bSuccess;

	bSuccess = m_penBtnShadow.DeleteObject();
	ASSERT(bSuccess);

	bSuccess = m_penBtnHighlight.DeleteObject();
	ASSERT(bSuccess);

	if (m_pEdit)
		delete m_pEdit;

	if (m_bTitleTips && ::IsWindow(m_titleTip.GetSafeHwnd()))  {
		bSuccess = m_titleTip.DestroyWindow();
		ASSERT(bSuccess);
	}

	bSuccess = m_font.DeleteObject();
	ASSERT(bSuccess);

	bSuccess = m_bitmapPattern.DeleteObject();
	ASSERT(bSuccess);
}



//  ---------------------------------------------------------------------
void CVirtualGridCtrl::PreSubclassWindow() 
{
	BOOL bSuccess;
	CRect rect;
	CWnd *pParentWnd;
	CDC *pDC;
	int nRetVal;

	if (::IsWindow(m_hWnd))  {
		bSuccess = ModifyStyleEx(0, WS_EX_CLIENTEDGE);
		ASSERT(bSuccess);
	}

	//  I like it very much
	GetWindowRect(rect);
	pParentWnd = GetParent();
	if (pParentWnd)
		pParentWnd->ScreenToClient(rect);
	rect.InflateRect(1, 1);
	MoveWindow(rect);
	rect.DeflateRect(1, 1);
	MoveWindow(rect);

	if (pParentWnd)  {
		CFont *pFont;
		LOGFONT lf;
		int nRetVal;

		pFont = pParentWnd->GetFont();
		nRetVal = pFont->GetLogFont(&lf);
		bSuccess = m_font.CreateFontIndirect(&lf);
		m_header.m_pFont = &m_font;
		m_fixed.m_pFont = &m_font;
		SetFont(&m_font);
	}

	m_titleTip.SetParentWnd(this);


	pDC = GetDC();
	bSuccess = m_bitmapPattern.CreateCompatibleBitmap(pDC, 2, 2);
	ASSERT(bSuccess);
	nRetVal = ReleaseDC(pDC);
	ASSERT(nRetVal);
}




//  --
BOOL CVirtualGridCtrl::Create(
			      const RECT& rect,
			      CWnd *pParentWnd,
			      UINT nID,
			      DWORD dwStyle
			      )
{
	return CWnd::Create(
		VIRTUAL_GRID_CLASS_NAME,
		_T(""),
		dwStyle,
		rect,
		pParentWnd,
		nID
		);
}



//  -----
void CVirtualGridCtrl::Paint(CDC *pDC)
{
	CRect rect;
	CRect fixedRect;
	int nRetVal;

	if (m_nUpdateLock > 0)
		return;		//  Painting is forbidden

	//  ------------------------  Paint the grid header  ---------------------------
	rect = GetHeaderRect();
	if (m_bShowHeader && pDC->RectVisible(rect))  {
		//  Fixed part
		PaintHeaders(pDC);
		//  Cut off fixed headers
		fixedRect = GetFixedRect();
		nRetVal = pDC->ExcludeClipRect(fixedRect.left, rect.top, fixedRect.right, rect.bottom);
	}

	//  Regular part
	PaintHeaders(pDC, FALSE);
	
	//  Cut-off the header rectangle
	nRetVal = pDC->ExcludeClipRect(rect);

	//  Fields to the right and to the bottom
	PaintFreeField(pDC);

	//  -----------------  Paint the fixed part of the Grid  ------------------------
	rect = GetFixedRect();
	if (m_fixed.GetCount() && pDC->RectVisible(rect))  {
		//  Cells...
		PaintFixed(pDC);
		//  Mesh
		if (m_bGridLines)
			PaintFixedGrid(pDC);

		//  Cut off fixed part
		nRetVal = pDC->ExcludeClipRect(rect);
	}


	//  ------------------  Paint regular cells  ------------------------------
	if (m_visSize.m_nColumn > 0 && m_visSize.m_nRow > 0)  {
		//  Cut off edit control (if present)
		if (m_bEditing)
			nRetVal = pDC->ExcludeClipRect(GetEditRect(m_editCell));

		//  Cells cells cells
		PaintCells(pDC);

		if (IsFocusAllowed())
			PaintFocus(pDC);
	}

	//  Mesh
	if (m_bGridLines)
		PaintGridLines(pDC);
}




//  --
void CVirtualGridCtrl::ApplyEdit()
{
	m_bEditing = FALSE;
}


//  --------
void CVirtualGridCtrl::CancelEdit()
{
	if (!m_bEditing)
		return;		//  Nothing to cancel

	if (m_bAlwaysEdit)
		UpdateEditContents(FALSE);
	else
		HideEdit();
}


//  ------
void CVirtualGridCtrl::DefaultDrawCell(CDC *pDC, const CGridCell &cell, LPRECT lpRect)
{
	const UINT ds[] = {ILD_NORMAL, ILD_SELECTED};
	UINT ids;
	CRect defRect(*lpRect);
	CRect rect(*lpRect);
	GridCheckKind checkKind;
	CRect cellRect;
	int nImage;
	int nRetVal;
	int nWidth, nHeight;
	CPoint point;
	BOOL bRowHighlight, bCellHighlight, bSh, bImageHighlight, bSuccess;
	LOGBRUSH lb;
	CPoint textIndent;
	CString strText;

	if (m_bGridLines)  {
		if (m_nGridStyles & gsVertLine)
			rect.right -= m_nGridLineWidth;
		if (m_nGridStyles & gsHorzLine)
			rect.bottom -= m_nGridLineWidth;
	}

	checkKind = GetCheckKind(cell);
	nImage = GetCellImage(cell);

	//  Find out the indication of image highlight
	bRowHighlight = m_bRowSelect && (cell.m_nColumn == m_fixed.m_nCount)
		&& (cell.m_nRow == m_cellFocused.m_nRow);
	bCellHighlight = !m_bRowSelect && (cell == m_cellFocused);
	
	nRetVal = pDC->GetCurrentBrush()->GetLogBrush(&lb);
	bSh = (lb.lbColor == ::GetSysColor(COLOR_BTNHIGHLIGHT)
		|| lb.lbColor == ::GetSysColor(COLOR_BTNFACE));

	bImageHighlight = (!m_bImageHighlight) && (bRowHighlight || bCellHighlight) && bSh;

	//  -------------  Draw the flag  ----------------
	if (checkKind != gsNone)  {
		CRect checkRect;

		//  Get flag's rectangle
		rect.right = min(rect.left + m_nCheckWidth + GetCheckIndent(cell).x, rect.right);

		//  Is the flag visible
		if (rect.left < defRect.right)  {
			//  Let's paint
			point.x = rect.left + GetCheckIndent(cell).x;
			point.y = rect.top + GetCheckIndent(cell).y;

			//  Flag's size
			nWidth = m_nCheckWidth;
			if (point.x + nWidth > rect.right)
				nWidth = rect.right - point.x;
			nHeight = m_nCheckHeight;
			if (point.y + nHeight > rect.bottom)
				nHeight = rect.bottom - point.y;

			if (checkKind != gcUserDefine)  {
				UINT nState;

				//  Default is check box-like
				if (checkKind == gcRadioButton)
					nState = DFCS_BUTTONRADIO;
				else
					nState = DFCS_BUTTONCHECK;

				switch (GetCheckState(cell))  {
				case 0:		//  Unchecked
					break;
				case 1:		//  Checked
					nState |= DFCS_CHECKED;
					break;
				case 2:
					nState |= DFCS_INACTIVE;
					break;
				default:
					break;
				}


				if (m_checkStyle == csFlat)
					nState |= DFCS_FLAT;

				pDC->FillSolidRect(rect, pDC->GetBkColor());

				checkRect = rect;
				checkRect.DeflateRect(1, 1);
				bSuccess = pDC->DrawFrameControl(
					checkRect,
					DFC_BUTTON,
					nState
					);
				ASSERT(bSuccess);
			}  else  {
				//  NYI
			}

			lpRect->left = rect.right;
		}
	}

	//  ---------  Draw the image (if present) ------------
	if (nImage >= 0)  {
		IMAGEINFO ii;

		bSuccess = m_pImageList->GetImageInfo(nImage, &ii);
		ASSERT(bSuccess);
		//  if (m_bImageHighlight)
		//  ?????

		rect = *lpRect;

		//  Get image rectangle
		rect.right = min(rect.left + (ii.rcImage.right - ii.rcImage.left) + GetCellImageIndent(cell).x, rect.right);

		//  Is image visible?
		if (rect.left < defRect.right)  {
			point.x = rect.left + GetCellImageIndent(cell).x;
			point.y = rect.top + GetCellImageIndent(cell).y;

			//  Image size (for narrow columns cut-off purposes)
			nWidth = ii.rcImage.right - ii.rcImage.left;
			if (point.x + nWidth > rect.right)
				nWidth = rect.right - nWidth;
			nHeight = ii.rcImage.bottom - ii.rcImage.top;
			if (point.y + nHeight > rect.bottom)
				nHeight = rect.bottom - point.y;

			//  Image's style and background colors...
			ids = ds[IsCellFocused(cell) && m_bCellSelected && m_bImageHighlight];

			//  Paint the image
			pDC->FillSolidRect(rect, pDC->GetBkColor());

			m_pImageList->Draw(pDC, nImage, point, ids);
			lpRect->left = rect.right;
		}
	}

	//  Restore background color
	GetCellColors(cell, pDC);

	//  Paint the text if edit control is not visible
	if (!(cell == m_editCell && !IsFocusAllowed()))  {
		CGridColumn *pColumn;

		//  Get text rectangle
		rect = *lpRect;

		//  Is the text visible
		if (rect.left >= defRect.right)
			return;

		//  Get text draw parameters
		textIndent = GetCellTextIndent(cell);
		pColumn = m_columns[cell.m_nColumn];
		strText = GetCellText(cell);

		pDC->FillSolidRect(rect, pDC->GetBkColor());

		PaintText(
			pDC,
			rect,
			textIndent.x,
			textIndent.y,
			pColumn->GetAlignment(),
			pColumn->m_bWantReturns,
			pColumn->m_bWordWrap,
			strText
			);
	}
}



//  -------------------------------------------------------------------------------
void CVirtualGridCtrl::DefaultDrawHeader(CDC *pDC, CGridHeaderSection *pSection, LPRECT lpRect)
{
	CRect defRect(*lpRect);
	int nIndex;
	CPoint point;
	DRAWTEXTPARAMS dtp;
	UINT nFlags = 0;
	CRect rect(*lpRect);
	GridSortDirection sd;
	CRect textRect;
	int nRetVal;
	CPen *pOldPen;
	BOOL bSuccess;

	pDC->FillSolidRect(lpRect, ::GetSysColor(COLOR_BTNFACE));	//  ???

	//  If the section was pressed then we should offset both the picture and the text
	rect.OffsetRect(1, 1);

	nIndex = GetHeaderImage(pSection);

	//  Paint the image if present
	if (nIndex >= 0)  {
		IMAGEINFO ii;
		int nWidth, nHeight;

		bSuccess = m_header.m_pImageList->GetImageInfo(nIndex, &ii);
		rect = *lpRect;
		rect.right = min(rect.left + (ii.rcImage.right - ii.rcImage.left) + 2, rect.right);

		//  If the image is visible then paint it
		if (rect.left < lpRect->right)  {
			point.x = rect.left + 2;
			point.y = rect.top + 1 + (!m_header.m_bFlat);
		}

		nWidth = ii.rcImage.right - ii.rcImage.left;
		if (point.x + nWidth > rect.right)
			nWidth = rect.right - point.x;

		nHeight = ii.rcImage.bottom - ii.rcImage.top;
		if (point.y + nHeight > rect.bottom)
			nHeight = rect.bottom - point.y;

		bSuccess = m_pImageList->Draw(pDC, nIndex, point, ILD_NORMAL);
		lpRect->left = rect.right;
	}

	//  Is the text visible?
	if (rect.Width())  {
		sd = gsNone;
		if (pSection->GetLevel() == m_header.GetMaxLevel())
			sd = GetSortDirection(pSection);

		dtp.cbSize = sizeof(DRAWTEXTPARAMS);
		dtp.iLeftMargin = 2 + 4 * (nIndex < 0);
		dtp.iRightMargin = 6;

		//
		nFlags = DT_END_ELLIPSIS | DT_NOPREFIX;
		switch (pSection->m_nAlignment)  {
		case LVCFMT_LEFT:
			nFlags |= DT_LEFT;
			break;
		case LVCFMT_RIGHT:
			nFlags |= DT_RIGHT;
			break;
		case LVCFMT_CENTER:
			nFlags |= DT_CENTER;
			break;
		default:
			break;
		}

		if (pSection->m_bWordWrap)
			nFlags |= DT_WORDBREAK;

		if (sd != gsNone)  {

			//  NYI
		}

		//  Draw the text
		if (pSection->m_strCaption.GetLength() && rect.Width())  {
			TCHAR szBuffer[1024];
			lstrcpy(szBuffer, pSection->m_strCaption);

			//  First, we should calculate text rectangle
			textRect = *lpRect;
			nRetVal = ::DrawTextEx(
				pDC->m_hDC,
				szBuffer,
				lstrlen(szBuffer),
				textRect,
				nFlags | DT_CALCRECT,
				&dtp
				);

			//  Use vertical alignment for sections without images
			if (nIndex < 0)
				textRect.OffsetRect(0, ((lpRect->bottom - lpRect->top - textRect.Height()) / 2));
			else
				textRect.OffsetRect(0, 2 + 2 * !m_header.m_bFlat);

			//  Adjust left and right
			textRect.left = lpRect->left;
			textRect.right = lpRect->right;

			//  Text out
			pDC->SetBkMode(TRANSPARENT);
			nRetVal = ::DrawTextEx(
				pDC->m_hDC,
				szBuffer,
				lstrlen(szBuffer),
				textRect,
				nFlags,
				&dtp
				);
		}
	}

	//  ----------------------  Paint divider  --------------------------
	rect = *lpRect;

	//  What to paint: a button or some lines?
	if (m_header.m_bFlat)  {
		//  If the colors of header and the table are the same then
		//  we should paint single line
		if (m_header.m_bGridColor)  {
			CPen pen;

			bSuccess = pen.CreatePen(PS_SOLID, m_nGridLineWidth, GetGridLineColor(::GetSysColor(COLOR_WINDOW)));
			pOldPen = pDC->SelectObject(&pen);

			//  Line at the bottom
			point = pDC->MoveTo(rect.left, rect.bottom - 1);
			bSuccess = pDC->LineTo(rect.right - 1, rect.bottom - 1);

			//  Line to the right
			point = pDC->MoveTo(rect.right - 1, rect.top);
			bSuccess = pDC->LineTo(rect.right - 1, rect.bottom);

			pOldPen = pDC->SelectObject(pOldPen);
			bSuccess = pen.DeleteObject();
			ASSERT(bSuccess);
		}  else  {
			//  Line at the bottom
			pOldPen = pDC->SelectObject(&m_penBtnShadow);
			point = pDC->MoveTo(rect.left, rect.bottom - 2);
			bSuccess = pDC->LineTo(rect.right - 1, rect.bottom - 2);

			pDC->SelectObject(&m_penBtnHighlight);
			point = pDC->MoveTo(rect.left, rect.bottom - 1);
			bSuccess = pDC->LineTo(rect.right - 1, rect.bottom - 1);

			//  Line to the right
			pDC->SelectObject(&m_penBtnShadow);
			point = pDC->MoveTo(rect.right - 2, rect.top);
			bSuccess = pDC->LineTo(rect.right - 2, rect.bottom - 1);

			pDC->SelectObject(&m_penBtnHighlight);
			point = pDC->MoveTo(rect.right - 1, rect.top);
			bSuccess = pDC->LineTo(rect.right - 1, rect.bottom);

			pOldPen = pDC->SelectObject(pOldPen);
		}
	}  else  {		//  The Grid's Header ain't no flat
		//  Paint the button frame
		if (IsHeaderPressed(pSection))
			bSuccess = pDC->DrawEdge(lpRect, BDR_SUNKENOUTER, BF_RECT | BF_FLAT);
		else
			Paint3DFrame(pDC, lpRect, BF_RECT);

		//  Adjust section's rectangle
		bSuccess = InflateRect(lpRect, -2, -2);
	}
}



//  --
void CVirtualGridCtrl::DrawDragRect(CDC *pDC, const CGridCell &cell)
{
	int nInterType;

	if (!IsCellVisible(cell))
		return;		//  Nothing to draw

	//  Cell's rectangle
	CRect rect(GetEditRect(cell));

	//  Colors
	GetCellColors(m_cellFocused, pDC);

	//  --------  Let's paint  ----------
	
	//  Cut off some place for the header and fixed part
	CRect gridRect;
	gridRect = GetGridRect();

	nInterType = pDC->IntersectClipRect(
		GetFixedRect().right,
		gridRect.top,
		gridRect.right,
		gridRect.bottom
		);

	//  Paint the focus
	pDC->DrawFocusRect(rect);
}



//  ---
CGridCell CVirtualGridCtrl::GetCellAt(int x, int y)
{
	CGridCell cell;
	int nColumn, nRow;

	nColumn = GetColumnAt(x, y);
	nRow = GetRowAt(x, y);

	if (nColumn >= 0 && nRow >= 0)  {
		cell.m_nColumn = nColumn;
		cell.m_nRow = nRow;
	}  else  {
		cell.m_nColumn = -1;
		cell.m_nRow = -1;
	}

	return cell;
}



//  ---
CRect CVirtualGridCtrl::GetCellRect(const CGridCell &cell)
{
	CRect rect;
	CRect rowRect;

	rect = GetColumnLeftRight(cell.m_nColumn);
	rowRect = GetRowTopBottom(cell.m_nRow);

	rect.top = rowRect.top;
	rect.bottom = rowRect.bottom;

	return rect;
}



//  ---
int CVirtualGridCtrl::GetColumnAt(int x, int y)
{
	int i;
	int nLeft, nRight;
	CRect rect;

	GetClientRect(rect);

	//  Look through fixed columns...
	nLeft = rect.left;

	for (i = 0; i < GetFixed()->m_nCount; i ++)  {
		nRight = nLeft + m_columns[i]->GetWidth();

		if (nRight != nLeft && x >= nLeft && x < nRight)
			return i;

		nLeft = nRight;
	}

	//  Look through usual columns
	nLeft += GetGridOrigin().x;
	for (; i < m_columns.GetSize(); i ++)  {
		nRight = nLeft + m_columns[i]->GetWidth();
		if (nRight != nLeft && x >= nLeft && x < nRight)
			return i;
		nLeft = nRight;
	}

	return -1;
}



//  --
CRect CVirtualGridCtrl::GetColumnLeftRight(int nColumn)
{
	CRect rect;

	//  Check the column...
	if (!m_columns.GetSize())  {
		//  There's no columns at all
		rect.left = GetGridRect().left;
		rect.right = rect.left;
	}  else if (nColumn < 0)  {
		//  The column is leftier than the leftmost
		rect = GetColumnLeftRight(0);
		rect.right = rect.left;
	}  else if (nColumn > m_columns.GetUpperBound())  {
		//  The column is rightier than the rightmost
		rect = GetColumnLeftRight(m_columns.GetUpperBound());
		rect.left = rect.right;
	}  else  {
		//  Normal column
		GetClientRect(rect);
		rect.left += GetColumnsWidth(0, nColumn - 1);
		if (nColumn >= m_fixed.GetCount())
			rect.left += GetGridOrigin().x;
		rect.right = rect.left + m_columns[nColumn]->GetWidth();
	}

	return rect;
}



//  --
int CVirtualGridCtrl::GetColumnMaxWidth(int nColumn)
{
	int nMaxWidth;
	int i, nWidth;
	CGridCell cell;
	CRect rect;

	//  Check the column...
	if (nColumn < 0 || nColumn > m_columns.GetUpperBound())
		return 0;

	//  Is there visible rows?
	if (m_visSize.m_nRow == 0)
		return m_columns[nColumn]->m_nWidth;

	nMaxWidth = 0;

	for (i = 0; i < m_visSize.m_nRow; i ++)  {
		cell.m_nColumn = nColumn;
		cell.m_nRow = m_visOrigin.m_nRow + i;

		//  Find out text rectangle
		rect = GetCellTextBounds(cell);
		nWidth = rect.Width();

		//  Place for "flag"
		if (IsCellHasCheck(cell))
			nWidth += m_nCheckWidth + GetCheckIndent(cell).x;
		//  Place for image
		if (IsCellHasImage(cell))  {
			IMAGEINFO ii;
			BOOL bSuccess;

			bSuccess = m_pImageList->GetImageInfo(0, &ii);
			ASSERT(bSuccess);

			nWidth += (ii.rcImage.right - ii.rcImage.left) + GetCellImageIndent(cell).x;
		}
		
		//  Taking into account the grid lines
		if (m_bGridLines && (gsVertLine & m_nGridStyles))
			nWidth += m_nGridLineWidth;

		if (nMaxWidth < nWidth)
			nMaxWidth = nWidth;
	}

	return nMaxWidth;
}


//  --
CRect CVirtualGridCtrl::GetColumnRect(int nColumn)
{
	CRect rect;

	rect = GetColumnLeftRight(nColumn);
	rect.top = GetRowTopBottom(0).top;
	rect.bottom = GetRowTopBottom(m_rows.GetCount() - 1).bottom;

	return rect;
}



//  --
CRect CVirtualGridCtrl::GetColumnsRect(int nFirstColumn, int nLastColumn)
{
	CRect rect;
	CRect firstRect, secondRect;
	BOOL bSuccess;

	firstRect = GetColumnRect(nFirstColumn);
	secondRect = GetColumnRect(nLastColumn);

	bSuccess = rect.UnionRect(firstRect, secondRect);
	return rect;
}



//  --
int CVirtualGridCtrl::GetColumnsWidth(int nFirstColumn, int nLastColumn)
{
	int nWidth = 0;
	int i;

	//  Check the indices
	nFirstColumn = max(nFirstColumn, 0);
	nLastColumn = min(m_columns.GetUpperBound(), nLastColumn);

	for (i = nFirstColumn; i <= nLastColumn; i ++)
		nWidth += m_columns[i]->GetWidth();

	return nWidth;
}



//  ---
CRect CVirtualGridCtrl::GetEditRect(const CGridCell &cell)
{
	CRect rect;

	rect = GetCellRect(cell);

	//  Place for check box...
	if (IsCellHasCheck(cell))
		rect.left += m_nCheckWidth + GetCheckIndent(cell).x;

	//  Place for image...
	if (IsCellHasImage(cell))  {
		IMAGEINFO ii;
		BOOL bSuccess;

		bSuccess = m_pImageList->GetImageInfo(0, &ii);
		ASSERT(bSuccess);

		rect.left += (ii.rcImage.right - ii.rcImage.left) + GetCellImageIndent(cell).x;
	}

	//  Taking into account grid lines...
	if (m_bGridLines)  {
		if (gsVertLine & m_nGridStyles)
			rect.right -= m_nGridLineWidth;
		if (gsHorzLine & m_nGridStyles)
			rect.bottom -= m_nGridLineWidth;
	}


	//  Check the right border
	if (rect.left > rect.right)
		rect.left = rect.right;

	return rect;
}


//  ---
int CVirtualGridCtrl::GetFirstImageColumn()
{
	int i;

	for (i = m_fixed.GetCount(); i < m_columns.GetSize(); i ++)  {
		if (m_columns[i]->m_bVisible)
			return i;
	}

	return -1;
}


//  ---
CRect CVirtualGridCtrl::GetFixedRect()
{
	CRect rect;

	rect = GetGridRect();
	rect.right = rect.left + GetColumnsWidth(0, m_fixed.m_nCount - 1);

	return rect;
}


//  ---
int CVirtualGridCtrl::GetFixedWidth()
{
	return GetFixedRect().Width();
}


//  ---
CRect CVirtualGridCtrl::GetFocusRect()
{
	CRect rect;
	CGridCell cell;
	int nLeft;

	if (m_bRowSelect)
		rect = GetRowRect(m_cellFocused.m_nRow);
	else
		rect = GetCellRect(m_cellFocused);

	cell.m_nColumn = m_cellFocused.m_nColumn;

	if (m_bRowSelect)
		cell.m_nColumn = m_fixed.GetCount();

	cell.m_nRow = m_cellFocused.m_nRow;

	//  If cell's picture selectable
	if (!m_bImageHighlight)  {
		//  Place for small flag
		if (IsCellHasCheck(cell))
			rect.left += m_nCheckWidth + GetCheckIndent(cell).x;
		
		//  Place for picture
		if (IsCellHasImage(cell))  {
			IMAGEINFO ii;
			BOOL bSuccess;

			bSuccess = m_pImageList->GetImageInfo(0, &ii);
			ASSERT(bSuccess);

			rect.left += (ii.rcImage.right - ii.rcImage.left) + GetCellImageIndent(cell).x;
		}
	}

	//  Check the right border
	nLeft = GetCellRect(cell).right;
	if (rect.left > nLeft)
		rect.left = nLeft;

	return rect;
}



//  --
int CVirtualGridCtrl::GetGridHeight()
{
	return GetGridRect().Height();
}


//  --
CPoint CVirtualGridCtrl::GetGridOrigin()
{
	BOOL bSuccess;
	CPoint point(0, 0);
	SCROLLINFO si;

	//  -------  Get horizontal scroll bar data (if the bar is present)  -----
	bSuccess = GetScrollInfo(SB_HORZ, &si);
	if (bSuccess)
		point.x = - si.nPos;

	//  ------  Get vertical scroll bar data (if the bar is present)  -----
	bSuccess = GetScrollInfo(SB_VERT, &si);
	if (bSuccess)
		point.y = - si.nPos * m_rows.m_nHeight;

	return point;
}



//  --
CRect CVirtualGridCtrl::GetGridRect()
{
	CRect rect;

	GetClientRect(rect);
	rect.top = GetHeaderRect().bottom;

	return rect;
}


//  ---
int CVirtualGridCtrl::GetHeaderHeight()
{
	return m_header.GetHeight();
}



//  ---
CRect CVirtualGridCtrl::GetHeaderRect()
{
	CRect rect;

	GetClientRect(rect);
	rect.bottom = rect.top;

	if (m_bShowHeader)
		rect.bottom += GetHeaderHeight();

	return rect;
}



//  ---
CGridHeaderSection * CVirtualGridCtrl::GetHeaderSection(int nColumnIndex, int nLevel)
{
	return m_header.m_sections.GetSection(nColumnIndex, nLevel);
}



//  ---
CGridHeaderSection * CVirtualGridCtrl::GetResizeSectionAt(CPoint point)
{
	CGridHeaderSection *pSection;

	if (m_header.m_sections.FindResizeSection(point, pSection))
		return pSection;

	return NULL;
}



//  ----
int CVirtualGridCtrl::GetRowAt(int x, int y)
{
	int nRow;
	int grt, goy;

	if (m_rows.m_nHeight <= 0)
		return -1;

	grt = GetGridRect().top;
	goy = GetGridOrigin().y;

	if (y - grt - goy < 0)
		return -1;	//  That is the key moment


	nRow = (y - grt - goy) / m_rows.m_nHeight;

	//  Check the cell
	if (nRow >= 0 && nRow < m_rows.m_nCount)
		return nRow;

	return -1;
}



//  --
CRect CVirtualGridCtrl::GetRowRect(int nRow)
{
	CRect rect;
	RECT rc;

	rect = GetRowTopBottom(nRow);
	GetClientRect(&rc);

	rect.left = min(rc.left, GetColumnLeftRight(m_fixed.GetCount()).left);
	rect.right = GetColumnLeftRight(m_columns.GetUpperBound()).right;

	return rect;
}


//  --
CRect CVirtualGridCtrl::GetRowsRect(int nFirstRow, int nLastRow)
{
	CRect firstRect, lastRect;
	BOOL bSuccess;

	firstRect = GetRowRect(nFirstRow);
	lastRect = GetRowRect(nLastRow);

	bSuccess = firstRect.UnionRect(firstRect, lastRect);
	return firstRect;
}



//  --
int CVirtualGridCtrl::GetRowsHeight(int nFirstRow, int nLastRow)
{
	if (nFirstRow > nLastRow)
		return 0;

	return (nLastRow - nFirstRow + 1) * m_rows.m_nHeight;
}



//  --
CRect CVirtualGridCtrl::GetRowTopBottom(int nRow)
{
	CRect rect;

	rect.top = GetGridRect().top  + GetRowsHeight(0, nRow - 1) + GetGridOrigin().y;
	rect.bottom = rect.top + m_rows.m_nHeight;

	return rect;
}


//  ---
CGridHeaderSection * CVirtualGridCtrl::GetSectionAt(CPoint point)
{
	CGridHeaderSection *pSection;

	if (m_header.m_sections.FindSection(point, pSection))
		return pSection;

	return NULL;
}



//  ---
void CVirtualGridCtrl::PaintCells(CDC *pDC)
{
	int i, j;
	int nLeft, nTop;
	CRect rect;
	int nWidth;

	//  Left and top border of visible cells
	nLeft = GetColumnLeftRight(m_visOrigin.m_nColumn).left;
	nTop = GetRowTopBottom(m_visOrigin.m_nRow).top;

	//  Initialize top border
	rect.bottom = nTop;

	//  Look through the rows
	for (j = 0; j < m_visSize.m_nRow; j ++)  {
		//  Offset rectangle in vertical direction
		rect.top = rect.bottom;
		rect.bottom += m_rows.m_nHeight;

		//  Initialize left border
		rect.right = nLeft;
		
		//  Look though columns
		for (i = 0; i < m_visSize.m_nColumn; i ++)  {
			CGridCell cell(m_visOrigin.m_nColumn + i, m_visOrigin.m_nRow + j);

			nWidth = m_columns[cell.m_nColumn]->GetWidth();

			//  Paint only visible cells
			if (nWidth > 0)  {
				//  Offset rectangle in horizontal direction
				rect.left = rect.right;
				rect.right += nWidth;

				//  Paint the cell
				if (pDC->RectVisible(rect))
					PaintCell(pDC, cell, rect);
			}
		}
	}
}



//  --
void CVirtualGridCtrl::InvalidateCell(const CGridCell &cell)
{
	if (cell.m_nRow == m_cellFocused.m_nRow || cell.m_nColumn == m_cellFocused.m_nColumn)
		HideFocus();

	InvalidateRect(GetCellRect(cell));

	if (cell.m_nRow == m_cellFocused.m_nRow || cell.m_nColumn == m_cellFocused.m_nColumn)
		ShowFocus();
}



//  ---
void CVirtualGridCtrl::InvalidateCheck(const CGridCell &cell)
{
	InvalidateRect(GetCheckRect(cell));
}


//  --
void CVirtualGridCtrl::InvalidateColumn(int nColumn)
{
	HideFocus();
	InvalidateRect(GetColumnRect(nColumn));
	ShowFocus();
}


//  ---
void CVirtualGridCtrl::InvalidateColumns(int nFirstColumn, int nLastColumn)
{
	HideFocus();
	InvalidateRect(GetColumnsRect(nFirstColumn , nLastColumn));
	ShowFocus();
}


//  ---
void CVirtualGridCtrl::InvalidateEdit()
{
	if (m_bEditing)
		m_pEdit->Invalidate();
}


//  --
void CVirtualGridCtrl::InvalidateFixed()
{
	InvalidateRect(GetFixedRect());
}


//  ---
void CVirtualGridCtrl::InvalidateFocus()
{
	CRect rect;
	BOOL bSuccess;

	rect = GetFocusRect();

	//  Adjust focus rectangle (it doesn't suit the picture)
	if (m_bRowSelect)
		bSuccess = rect.UnionRect(rect, GetCellRect(CGridCell(m_fixed.m_nCount, m_cellFocused.m_nRow)));
	else
		bSuccess = rect.UnionRect(rect, GetCellRect(m_cellFocused));

	InvalidateRect(rect);
}



//  ---
void CVirtualGridCtrl::InvalidateGrid()
{
	InvalidateEdit();
	InvalidateRect(GetGridRect());
}


//  ---
void CVirtualGridCtrl::InvalidateHeader()
{
	if (m_bShowHeader)
		InvalidateRect(GetHeaderRect());
}


//  ---
void CVirtualGridCtrl::InvalidateRow(int nRow)
{
	if (nRow == m_cellFocused.m_nRow)
		HideFocus();

	InvalidateRect(GetRowRect(nRow));

	if (nRow == m_cellFocused.m_nRow)
		ShowFocus();
}


//  --
void CVirtualGridCtrl::InvalidateRows(int nFirstRow, int nLastRow)
{
	HideFocus();
	InvalidateRect(GetRowsRect(nFirstRow, nLastRow));
	ShowFocus();
}



//  --
BOOL CVirtualGridCtrl::IsActiveControl()
{
	if (GetParent() && this == GetActiveWindow())
		return TRUE;

	if (m_pEdit && m_pEdit == GetFocus())
		return TRUE;

	if (this == GetFocus())
		return TRUE;

	return FALSE;
}


//  --
BOOL CVirtualGridCtrl::IsCellAcceptCursor(const CGridCell &cell)
{
	CWnd *pWnd;
	LRESULT lResult;

	if (!IsCellValid(cell))
		return FALSE;

	pWnd = GetParent();
	ASSERT(pWnd);
	ASSERT(::IsWindow(pWnd->GetSafeHwnd()));

	m_dispInfo.hdr.code = VGN_CELLACCEPTCURSOR;
	m_dispInfo.hdr.hwndFrom = m_hWnd;
	m_dispInfo.hdr.idFrom = GetDlgCtrlID();

	m_dispInfo.item.nColumn = cell.m_nColumn;
	m_dispInfo.item.nRow = cell.m_nRow;

	//  Default result
	m_dispInfo.item.bAcceptCursor = (cell.m_nColumn >= m_fixed.GetCount()) && (m_columns[cell.m_nColumn]->GetTabStop());

	lResult = pWnd->SendMessage(WM_NOTIFY, m_dispInfo.hdr.idFrom, (LPARAM)&m_dispInfo);

	return m_dispInfo.item.bAcceptCursor;
}



//  --
BOOL CVirtualGridCtrl::IsCellHasCheck(const CGridCell &cell)
{
	return (m_bCheckBoxes && gcNone != GetCheckKind(cell));
}


//  --
BOOL CVirtualGridCtrl::IsCellHasImage(const CGridCell &cell)
{
	return (m_pImageList && -1 != GetCellImage(cell));
}


//  --
BOOL CVirtualGridCtrl::IsCellFocused(const CGridCell &cell)
{
	return ((cell.m_nColumn == m_cellFocused.m_nColumn) || m_bRowSelect)
		&& (cell.m_nRow == m_cellFocused.m_nRow)
		&& (cell.m_nColumn >= m_fixed.m_nCount);
}



//  --
BOOL CVirtualGridCtrl::IsCellReadOnly(const CGridCell &cell)
{
	CWnd *pWnd;
	LRESULT lResult;

	if (!IsCellValid(cell))
		return TRUE;

	pWnd = GetParent();
	ASSERT(pWnd);

	ASSERT(::IsWindow(pWnd->GetSafeHwnd()));

	m_dispInfo.hdr.code = VGN_GETALLOWEDIT;
	m_dispInfo.hdr.hwndFrom = m_hWnd;
	m_dispInfo.hdr.idFrom = GetDlgCtrlID();

	m_dispInfo.item.nColumn = cell.m_nColumn;
	m_dispInfo.item.nRow = cell.m_nRow;

	//  We are allowed to edit the cell if the grid is not read only AND cell's column
	//  is not fixed AND the column itself is not read only
	m_dispInfo.item.bReadOnly = m_bReadOnly || cell.m_nColumn < m_fixed.GetCount() || m_columns[cell.m_nColumn]->m_bReadOnly;

	lResult = pWnd->SendMessage(WM_NOTIFY, m_dispInfo.hdr.idFrom, (LPARAM)&m_dispInfo);
	return m_dispInfo.item.bReadOnly;
}




//  --
BOOL CVirtualGridCtrl::IsCellValid(
				     const CGridCell &cell,
				     BOOL bCheckPosition,	//  = TRUE
				     BOOL bCheckVisible		//  = TRUE
				     )
{
	BOOL bCorrectColumn;
	BOOL bCorrectRow;
	BOOL bVisible = FALSE;

	bCorrectColumn = cell.m_nColumn >= 0 && cell.m_nColumn < m_columns.GetSize();
	bCorrectRow = cell.m_nRow >= 0 && cell.m_nRow < m_rows.m_nCount;

	if (bCorrectColumn)
		bVisible = m_columns[cell.m_nColumn]->GetVisible()
			&& m_columns[cell.m_nColumn]->m_nWidth > 0;


	return (!bCheckPosition || (bCorrectColumn && bCorrectRow)
		&& !bCheckVisible || bVisible);
}


//  --
BOOL CVirtualGridCtrl::IsCellVisible(const CGridCell &cell, BOOL bPartialOK)
{
	BOOL bVisible;
	CRect cellRect;
	CRect gridRect;
	CRect rect;

	cellRect = GetCellRect(cell);
	gridRect = GetGridRect();

	if (m_fixed.GetCount() && cell.m_nColumn >= m_fixed.GetCount())
		gridRect.left = GetFixedRect().right;

	//  Intersection
	bVisible = rect.IntersectRect(cellRect, gridRect);

	if (!bPartialOK)
		bVisible = (rect == cellRect);

	return bVisible;
}



//  --
BOOL CVirtualGridCtrl::IsColumnVisible(int nColumn)
{
	CRect columnRect;
	CRect gridRect;

	columnRect = GetColumnRect(nColumn);
	gridRect = GetGridRect();
	return columnRect.IntersectRect(columnRect, gridRect);
}



//  --
BOOL CVirtualGridCtrl::IsFocusAllowed()
{
	return (m_bRowSelect || (!(m_bEditing || m_bAlwaysEdit))) && m_bAllowSelect;
}


//  --
BOOL CVirtualGridCtrl::IsHeaderHasImage(CGridHeaderSection *pSection)
{
	if (m_pImageList == NULL)
		return FALSE;

	if (GetHeaderImage(pSection) < 0)
		return FALSE;

	return TRUE;
}


//  --
BOOL CVirtualGridCtrl::IsHeaderPressed(CGridHeaderSection *pSection)
{
	return ((pSection == NULL) || (pSection == m_pHeaderClickSection)) && m_bHeaderClickState;
}


//  --
BOOL CVirtualGridCtrl::IsRowVisible(int nRow)
{
	CRect rowRect;
	CRect gridRect;

	rowRect = GetRowRect(nRow);
	gridRect = GetGridRect();

	return rowRect.IntersectRect(rowRect, gridRect);
}



//  -------
void CVirtualGridCtrl::MakeCellVisible(const CGridCell &cell, BOOL bPartialOK)
{
	int dx, dy, x, y;
	CRect gridRect, rect;

	LRESULT lResult;

	if (IsCellVisible(cell, bPartialOK))
		return;		//  Already done

	dx = 0;
	dy = 0;
	gridRect = GetGridRect();

	//  -----------------  Horizontal scroll  --------------------
	if (!m_bRowSelect)  {
		rect = GetColumnRect(cell.m_nColumn);
		x = gridRect.left + GetFixedWidth();

		if (rect.right > gridRect.right)
			dx = gridRect.right - rect.right;
		if (rect.left < x)
			dx = x - rect.left;
		if (rect.Width() > gridRect.right - x)
			dx = x - rect.left;
	}

	if (dx < 0)  {
		while (dx)  {
			lResult = SendMessage(WM_HSCROLL, SB_LINERIGHT, 0);
			dx ++;
		}
	}  else if (dx > 0)  {
		while (dx)  {
			lResult = SendMessage(WM_HSCROLL, SB_LINELEFT, 0);
			dx --;
		}
	}

	//  --------------  Vertical scroll  ---------------
	if (m_rows.m_nHeight > 0)  {
		rect = GetRowRect(cell.m_nRow);
		if (rect.bottom > gridRect.bottom)
			dy = gridRect.bottom - rect.bottom;
		if (rect.top < gridRect.top)
			dy = gridRect.top - rect.top;
		if (rect.Height() > gridRect.Height())
			dy = gridRect.top - rect.top;
		y = dy / m_rows.m_nHeight;

		if (m_visSize.m_nRow > 1 && fmod(dy, m_rows.m_nHeight) != 0.0)
			y --;
		dy = y;
	}

	if (dy < 0)  {
		while (dy)  {
			lResult = SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);
			dy ++;
		}
	}  else if (dy > 0)  {
		while (dy)  {
			lResult = SendMessage(WM_VSCROLL, SB_LINEUP, 0);
			dy --;
		}
	}
}



//  ---
void CVirtualGridCtrl::SetCursor(CGridCell &cell, BOOL bSelected, BOOL bVisible)
{
	//  Check selection...
	UpdateSelection(cell, bSelected);

	//  Is there something changed?
	if (m_cellFocused != cell || m_bCellSelected != bSelected)  {
		//  The cell is changing...
		Changing(cell, bSelected);

		//  Set active cell
		if (m_cellFocused != cell)  {
			//  Application.CancelHint

			SetEditing(FALSE);

			//  Change cell
			HideCursor();

			m_cellFocused = cell;
			m_bCellSelected = bSelected;
			if (bVisible)
				MakeCellVisible(cell, FALSE);
			ShowCursor();
		}  else if (m_bCellSelected != bSelected)  {
			if (m_bEditing)
				ShowEdit();

			if (!m_bEditing)  {
				HideCursor();
				m_bCellSelected = bSelected;
				if (bVisible)
					MakeCellVisible(cell, FALSE);
				ShowCursor();
			}
		}

		Change(m_cellFocused, m_bCellSelected);
	}  else  {
		if (bVisible)
			MakeCellVisible(cell, FALSE);
	}
}


//  --
void CVirtualGridCtrl::UndoEdit()
{
	//  NYI
}



//  --
void CVirtualGridCtrl::UpdateCursor()
{
	CGridCell cell;
	BOOL bIsValidCell, bDummy;

	cell = m_cellFocused;

	bIsValidCell = IsCellValid(cell) && IsCellAcceptCursor(cell);

	//  If current cell is not acceptable then look for accessible cell around.
	if (!bIsValidCell)  {
		UpdateSelection(cell, bDummy);
		if (cell == m_cellFocused)
			cell = GetCursorCell(cell, goFirst);
	}

	//  Adjust cell selection
	SetCursor(cell, m_bCellSelected, !bIsValidCell);
}


//  --
void CVirtualGridCtrl::UpdateColors()
{
	CDC *pDC = GetDC();
	int nRetVal;

	m_header.GridColorChanged(pDC->GetBkColor());
	m_fixed.GridColorChanged(pDC->GetBkColor());

	nRetVal = ReleaseDC(pDC);
	ASSERT(nRetVal);
}


//  ---
void CVirtualGridCtrl::UpdateEdit(BOOL bActivate)
{
	if (EditCanShow(m_cellFocused))  {
		if (m_pEdit == NULL)  {
			ValidateEdit();
			DoUpdateEdit();
		}  else if (!(m_editCell == m_cellFocused && m_bEditing))  {
			bActivate = bActivate || m_bEditing || m_bAlwaysEdit;
			HideEdit();
			ValidateEdit();
			DoUpdateEdit();
		}

		if (bActivate)
			m_pEdit->Show();
	}  else  {
		HideEdit();
	}
}


//  ----
void CVirtualGridCtrl::UpdateEditContents(BOOL bSaveText)
{
	CString strEditText;

	if (!m_bEditing)
		return;		//  Nothing to update

	m_pEdit->GetWindowText(strEditText);

	//  To completely update the edit control, we should switch it off
	m_pEdit->ShowWindow(SW_HIDE);

	//  Update and show again
	UpdateEdit();

	//  Recover the text
	if (bSaveText)
		m_pEdit->SetWindowText(strEditText);
}


//  ---
void CVirtualGridCtrl::UpdateEditText()
{
	BOOL bEditFocused;
	CString strEditText;
	CWnd *pWnd;

	if (m_bReadOnly || m_pEdit == NULL || IsCellReadOnly(m_editCell))
		return;		//  Nothing to update

	bEditFocused = GetEditing();

	try
	{
		m_pEdit->GetWindowText(strEditText);
		SetEditText(m_editCell, strEditText);

		m_pEdit->SetWindowText(strEditText);
	}
	catch (...)
	{
		//  Don't let the scroller move
		MakeCellVisible(m_cellFocused, FALSE);

		//  If the edit line is visible then we should set the focus on it
		//  otherwise it would be switched off after error dialog box appearing
		if (bEditFocused)
			pWnd = m_pEdit->SetFocus();
	}
}


//  ---
void CVirtualGridCtrl::UpdateFixed()
{
	m_fixed.SetCount(m_fixed.m_nCount);
}



//  ---
void CVirtualGridCtrl::UpdateFocus()
{
	CWnd *pWnd;

	//  ????????
	//  If the grid is already active then set the focus once more to
	//  avoid problems with MDI forms
	if (IsActiveControl())  {
		pWnd = SetFocus();
	}  else  {
		//  Can we set the focus at all?
		if (IsWindowVisible())  {	//   && m_bTabStop  ?????
			pWnd = SetFocus();
			if (m_bAlwaysEdit && m_pEdit)
				UpdateEdit();
		}
	}
}


//  --
void CVirtualGridCtrl::UpdateFonts()
{
	m_header.GridFontChanged(&m_font);
	m_fixed.GridFontChanged(&m_font);
}


//  ---
void CVirtualGridCtrl::UpdateHeader()
{
	if (m_header.m_bAutoSynchronize || m_header.m_bSynchronized)
		m_header.SynchronizeSections();
	else
		m_header.UpdateSections();
}


//  --
void CVirtualGridCtrl::UpdateRows()
{
	m_rows.SetHeight(m_rows.m_nHeight);
}



//  ---
void CVirtualGridCtrl::UpdateScrollBars()
{
	CRect gridRect;
	SCROLLINFO si;
	BOOL bSuccess;

	gridRect = GetGridRect();

	//  ------------  Vertical scrolling parameters...  ---------------
	if (m_rows.m_nCount > 0 && m_rows.m_nHeight > 0)  {
		si.nMax = m_rows.m_nCount - 1;
		si.nPage = gridRect.Height() / m_rows.m_nHeight;
	}  else  {
		si.nMax = 0;
		si.nPage = 0;
	}

	EnableScrollBarCtrl(SB_VERT, !((UINT)si.nMax < si.nPage));

	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_PAGE | SIF_RANGE | SIF_DISABLENOSCROLL;
	si.nMin = 0;

	bSuccess = SetScrollInfo(SB_VERT, &si);
	ASSERT(bSuccess);

	//  --------  Horizontal scrolling parameters...  -------
	if (m_columns.GetSize())  {
		si.nMax = GetColumnsWidth(0, m_columns.GetUpperBound()) - GetFixedWidth();
		si.nPage = gridRect.Width() - GetFixedWidth();
	}  else  {
		si.nMax = 0;
		si.nPage = 0;
	}

	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_RANGE | SIF_PAGE;
	si.nMin = 0;

	bSuccess = SetScrollInfo(SB_HORZ, &si);
	ASSERT(bSuccess);
}



//  ---
void CVirtualGridCtrl::UpdateScrollPos()
{
	SCROLLINFO si;
	BOOL bSuccess;

	//  ------  Update vertical scroll bar  ------
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_POS;
	si.nPos = m_visOrigin.m_nRow;
	bSuccess = SetScrollInfo(SB_VERT, &si);

	//  ------  Update horizontal scroll bar  -----
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_POS;
	si.nPos = GetColumnsWidth(m_fixed.GetCount(), m_visOrigin.m_nColumn - 1);
	bSuccess = SetScrollInfo(SB_HORZ, &si);
}


//  ---
void CVirtualGridCtrl::UpdateSelection(CGridCell &cell, BOOL &bSelected)
{
	//  Check the selection flag
	bSelected = bSelected || m_bAlwaysSelected;
	bSelected = bSelected && m_rows.m_nCount > 0 && m_columns.GetSize();

	//  Check cell borders
	if (cell.m_nColumn < m_fixed.GetCount())
		cell.m_nColumn = m_fixed.GetCount();
	if (cell.m_nColumn < 0)
		cell.m_nColumn = 0;
	if (cell.m_nColumn > m_columns.GetUpperBound())
		cell.m_nColumn = m_columns.GetUpperBound();

	//  Check the focus...
	cell = GetCursorCell(cell, goSelect);
}


//  ---
void CVirtualGridCtrl::UpdateText()
{
	UpdateEditText();
}


//  ----
void CVirtualGridCtrl::UpdateVisOriginSize()
{
	CRect clientRect;
	CRect rect;
	SCROLLINFO si;
	BOOL bSuccess;
	int i;
	int x, nHeight;

	GetClientRect(clientRect);

	//  -------------------  Columns part  ---------------------------
	if (m_columns.GetSize())  {
		bSuccess = GetScrollInfo(SB_HORZ, &si);
		if (!bSuccess)
			si.nPos = 0;

		//  Look for the first non-fixed column
		x = clientRect.left + GetFixedWidth() - si.nPos;
		rect = GetFixedRect();
		i = m_fixed.GetCount();

		while (i < m_columns.GetUpperBound())  {
			x += m_columns[i]->GetWidth();

			if (x >= rect.right)
				break;

			i ++;
		}

		m_visOrigin.m_nColumn = i;

		//  Calculate amount of visible columns
		rect = GetGridRect();

		while (i < m_columns.GetUpperBound())  {
			if (x >= rect.right)
				break;
			i ++;
			x += m_columns[i]->GetWidth();
		}

		m_visSize.m_nColumn = i - m_visOrigin.m_nColumn + 1;
	}  else  {
		m_visOrigin.m_nColumn = 0;
		m_visSize.m_nColumn = 0;
	}

	//  -----------------------  Rows part  ---------------------------
	if (m_rows.GetCount() && m_rows.m_nHeight > 0)  {
		div_t myDiv;
		bSuccess = GetScrollInfo(SB_VERT, &si);
		if (!bSuccess)
			si.nPos = 0;

		m_visOrigin.m_nRow = si.nPos;

		//  Calculate amount of visible (even partially) rows
		nHeight = GetGridHeight();
		myDiv = div(nHeight, m_rows.m_nHeight);

		m_visSize.m_nRow = myDiv.quot + (myDiv.rem > 0);

		if (m_visSize.m_nRow + m_visOrigin.m_nRow > m_rows.GetCount())
			m_visSize.m_nRow = m_rows.m_nCount - m_visOrigin.m_nRow;
	}  else  {
		m_visOrigin.m_nRow = 0;
		m_visSize.m_nRow = 0;
	}
}


//  ---
CString CVirtualGridCtrl::GetCell(int nColumn, int nRow)
{
	CGridCell cell(nColumn, nRow);
	return GetCellText(cell);
}


//  ---
BOOL CVirtualGridCtrl::GetChecked(int nColumn, int nRow)
{
	int nState;

	nState = GetCheckState(CGridCell(nColumn, nRow));

	return (nState == 1 || nState == 2);	//  ????
}


//  ---
int CVirtualGridCtrl::GetCol()
{
	return m_cellFocused.m_nColumn;
}


//  ---
CGridFixed * CVirtualGridCtrl::GetFixed()
{
	return &m_fixed;
}


//  --
CGridEdit * CVirtualGridCtrl::GetEdit()
{
	return m_pEdit;
}


//  --
CGridColumn * CVirtualGridCtrl::GetEditColumn()
{
	if (m_editCell.m_nColumn < 0 || m_editCell.m_nColumn > m_columns.GetUpperBound())
		return NULL;

	return m_columns[m_editCell.m_nColumn];
}


//  --
BOOL CVirtualGridCtrl::GetEditDropDown()
{
	//  NYI
	return FALSE;
}


//  --
BOOL CVirtualGridCtrl::GetEditing()
{
	return m_bEditing && m_pEdit;
}


//  --
CGridHeader * CVirtualGridCtrl::GetHeader()
{
	return &m_header;
}


//  --
int CVirtualGridCtrl::GetLeftCol()
{
	return m_visOrigin.m_nColumn;
}


//  --
int CVirtualGridCtrl::GetRow()
{
	return m_cellFocused.m_nRow;
}


//  --
CGridRows* CVirtualGridCtrl::GetRows()
{
	return &m_rows;
}


//  --
int CVirtualGridCtrl::GetTopRow()
{
	return m_visOrigin.m_nRow;
}


//  --
int CVirtualGridCtrl::GetVisibleColCount()
{
	return m_visSize.m_nColumn;
}


//  --
int CVirtualGridCtrl::GetVisibleRowCount()
{
	return m_visSize.m_nRow;
}


//  --
void CVirtualGridCtrl::ColumnsChange()
{
	UpdateFixed();
	UpdateHeader();

	if (!m_header.m_bAutoSynchronize)
		m_header.SetSynchronized(FALSE);

	UpdateScrollBars();
	UpdateVisOriginSize();
	UpdateCursor();
	UpdateEdit(m_bEditing);
	Invalidate();
	ChangeColumns();
}


//  --
void CVirtualGridCtrl::FixedChange()
{
	UpdateRows();
	UpdateScrollBars();
	UpdateVisOriginSize();
	UpdateCursor();
	UpdateEdit(m_bEditing);
	Invalidate();
	ChangeFixed();
}


//  --
void CVirtualGridCtrl::HeaderChange()
{
	UpdateScrollBars();
	UpdateVisOriginSize();
	UpdateEdit(m_bEditing);
	Invalidate();
}


//  --
void CVirtualGridCtrl::HorzScrollChange()
{
	UpdateVisOriginSize();
	UpdateEdit(m_bEditing);
}


//  --
void CVirtualGridCtrl::ImagesChange()
{
	InvalidateGrid();
	UpdateRows();
}


//  --
void CVirtualGridCtrl::RowsChange()
{
	UpdateScrollBars();
	UpdateVisOriginSize();
	UpdateCursor();
	UpdateEdit(m_bEditing);
	Invalidate();
	ChangeRows();
}



//  --
void CVirtualGridCtrl::SetAllowEdit(BOOL bAllowEdit)
{
	if (m_bAllowEdit == bAllowEdit)
		return;		//  Nothing to change

	m_bAllowEdit = bAllowEdit;
	if (m_bAllowEdit)  {
		SetRowSelect(FALSE);
	}  else  {
		m_bAlwaysEdit = FALSE;
		HideEdit();
	}

	//  Event
	ChangeEditMode();
}


//  ---
void CVirtualGridCtrl::SetAllowSelect(BOOL bAllowSelect)
{
	if (m_bAllowSelect == bAllowSelect)
		return;		//  Nothing to change

	m_bAllowSelect = bAllowSelect;
	SetRowSelect(m_bRowSelect || !m_bAllowSelect);
	InvalidateFocus();
}


//  ---
void CVirtualGridCtrl::SetAlwaysEdit(BOOL bAlwaysEdit)
{
	if (m_bAlwaysEdit == bAlwaysEdit)
		return;		//  Nothing to change
	
	m_bAlwaysEdit = bAlwaysEdit;
	if (m_bAlwaysEdit)  {
		SetAllowEdit();
		SetEditing();
	}  else  {
		HideEdit();
	}
}


//  ---
void CVirtualGridCtrl::SetAlwaysSelected(BOOL bAlwaysSelected)
{
	if (m_bAlwaysSelected == bAlwaysSelected)
		return;		//  Nothing to change
	
	m_bAlwaysSelected = bAlwaysSelected;
	m_bCellSelected = m_bAlwaysSelected || m_bAlwaysSelected;
	InvalidateFocus();
}


//  --
void CVirtualGridCtrl::SetCell(int nColumn, int nRow, CString& strText)
{
	SetEditText(CGridCell(nColumn, nRow), strText);
}


//  --
void CVirtualGridCtrl::SetCellFocused(CGridCell &cell)
{
	SetCursor(cell, m_bCellSelected, TRUE);
}


//  --
void CVirtualGridCtrl::SetCellSelected(BOOL bCellSelected)
{
	SetCursor(m_cellFocused, bCellSelected, TRUE);
}


//  --
void CVirtualGridCtrl::SetCheckBoxes(BOOL bCheckBoxes)
{
	if (m_bCheckBoxes == bCheckBoxes)
		return;		//  Nothing to change

	m_bCheckBoxes = bCheckBoxes;
	UpdateRows();
	UpdateEdit(m_bEditing);
	InvalidateGrid();
}


//  --
void CVirtualGridCtrl::SetCheckLeftIndent(int nLeftIndent)
{

}


//  --
void CVirtualGridCtrl::SetCheckStyle(GridCheckStyle checkStyle)
{
	if (m_checkStyle == checkStyle)
		return;		//  Nothing to change

	m_checkStyle = checkStyle;

	if (m_bCheckBoxes)
		InvalidateGrid();
}


//  --
void CVirtualGridCtrl::SetCheckTopIndent(int nTopIndent)
{

}


//  --
void CVirtualGridCtrl::SetCol(int nCol)
{
	SetCellFocused(CGridCell(nCol, m_cellFocused.m_nRow));
}


//  ---
void CVirtualGridCtrl::SetColumns(const CGridColumns& columns)
{
	int i;
	CGridColumn *pColumn;

	for (i = 0; i < m_columns.m_columns.GetSize(); i ++)  {
		delete m_columns[i];
	}

	m_columns.m_columns.RemoveAll();

	for (i = 0; i < columns.m_columns.GetSize(); i ++)  {
		pColumn = new CGridColumn(*columns.m_columns[i]);
		pColumn->m_pColumns = &m_columns;
		m_columns.m_columns.Add(pColumn);
	}

	ColumnsChange();
}


//  ---
void CVirtualGridCtrl::SetCursorKeys(UINT nCursorKeys)
{
	//  Check incompatible flags
	if (gkMouseMove & nCursorKeys)
		nCursorKeys = nCursorKeys | gkMouse;
	if (!(gkMouse & nCursorKeys))
		nCursorKeys = nCursorKeys & ~gkMouseMove;

	m_nCursorKeys = nCursorKeys;
}


//  ---
void CVirtualGridCtrl::SetEditDropDown(BOOL bEditDropDown)
{
	//  Set cell into editing mode
	SetEditing();

	//  Show drop down list
	if (m_pEdit)
		m_pEdit->SetDropListVisible(TRUE);
}


//  ---
void CVirtualGridCtrl::SetEditing(BOOL bEditing)
{
	m_titleTip.Hide();

	if (bEditing && m_bAllowEdit)  {
		if (AcquireFocus())
			ShowEdit();
	}  else if (!bEditing && m_bEditing)  {
		UpdateEditText();
		if (!m_bAlwaysEdit)
			HideEdit();
	}

	//  An event...
	ChangeEditing();
}



//  ---
void CVirtualGridCtrl::SetEndEllipsis(BOOL bEndEllipsis)
{
	if (m_bEndEllipsis == bEndEllipsis)
		return;		//  Nothing to change

	m_bEndEllipsis = bEndEllipsis;
	InvalidateGrid();
}

void CVirtualGridCtrl::SetFlatScrollBars(BOOL bFlatScrollBars)
{

}

void CVirtualGridCtrl::SetFixed(CGridFixed *pFixed)
{

}

void CVirtualGridCtrl::SetGridColor(COLORREF gridColor)
{

}


//  -----
void CVirtualGridCtrl::SetGridLines(BOOL bGridLines)
{
	if (m_bGridLines == bGridLines)
		return;		//  Nothing to set

	m_bGridLines = bGridLines;
	UpdateRows();
	UpdateEdit(m_bEditing);
	Invalidate();
}



//  --------
void CVirtualGridCtrl::SetGridStyle(UINT nGridStyles)
{
	if (m_nGridStyles == nGridStyles)
		return;		//  Nothing to set

	m_nGridStyles = nGridStyles;
	UpdateRows();
	UpdateEdit(m_bEditing);
	Invalidate();
}


//  ---
void CVirtualGridCtrl::SetHeader(CGridHeader *pHeader)
{

}

void CVirtualGridCtrl::SetHideSelection(BOOL bHideSelection)
{

}

void CVirtualGridCtrl::SetImageIndexDef(int nIndex)
{

}

void CVirtualGridCtrl::SetImageHighlight(BOOL bImageHighlight)
{

}


void CVirtualGridCtrl::SetImageLeftIndent(int nLeftIndent)
{

}


//  ---
void CVirtualGridCtrl::SetImageList(CImageList *pImageList)
{
	m_pImageList = pImageList;
}


//  ---
void CVirtualGridCtrl::SetImageTopIndent(int nTopIndent)
{

}


//  ----
void CVirtualGridCtrl::SetLeftCol(int nLeftCol)
{
	SetVisOrigin(CGridCell(nLeftCol, m_visOrigin.m_nRow));
}


//  ----
void CVirtualGridCtrl::SetReadOnly(BOOL bReadOnly)
{
	if (m_bReadOnly == bReadOnly)
		return;		//  Nothing to change

	m_bReadOnly = bReadOnly;
	UpdateEditContents();
}



void CVirtualGridCtrl::SetRow(int nRow)
{

}

void CVirtualGridCtrl::SetRows(CGridRows *pRows)
{

}


//  ---
void CVirtualGridCtrl::SetRowSelect(BOOL bRowSelect)
{
	if (m_bRowSelect == bRowSelect)
		return;		//  Nothing to change

	m_bRowSelect = bRowSelect;
	SetAllowEdit(!bRowSelect);

	SetAllowSelect(GetAllowSelect() || !bRowSelect);
	InvalidateGrid();
}

void CVirtualGridCtrl::SetShowCellTips(BOOL bShowCellTips)
{

}

void CVirtualGridCtrl::SetShowFocusRect(BOOL bShowFocusRect)
{

}

void CVirtualGridCtrl::SetShowHeader(BOOL bShowHeader)
{

}

void CVirtualGridCtrl::SetSortLeftIndent(int nLeftIndent)
{

}


void CVirtualGridCtrl::SetSortTopIndent(int nTopIndent)
{

}

void CVirtualGridCtrl::SetTextLeftIndent(int nLeftIndent)
{

}

void CVirtualGridCtrl::SetTextRightIndent(int nRightIndent)
{

}

void CVirtualGridCtrl::SetTextTopIndent(int nTopIndent)
{

}


//  ----
void CVirtualGridCtrl::SetTopRow(int nTopRow)
{
	SetVisOrigin(CGridCell(m_visOrigin.m_nColumn, nTopRow));
}



//  ---
void CVirtualGridCtrl::SetVisOrigin(const CGridCell &cell)
{
	if (m_visOrigin.m_nColumn != cell.m_nColumn || m_visOrigin.m_nRow != cell.m_nRow)  {
		m_visOrigin = cell;

		//  Update scroll bars...
		UpdateScrollPos();
		UpdateVisOriginSize();

		//  Repaint...
		Invalidate();
	}
}


//  --------
void CVirtualGridCtrl::VertScrollChange()
{
	UpdateVisOriginSize();
	UpdateEdit(m_bEditing);
}


//  -------------------------------------------------------------------------------
BOOL CVirtualGridCtrl::AcquireFocus()
{
	UpdateFocus();

	return IsActiveControl();
}



//  ----
void CVirtualGridCtrl::CellClick(const CGridCell &cell, UINT nFlags, int x, int y)
{
	//  NYI
}


//  ---
void CVirtualGridCtrl::CellTips(const CGridCell &cell, BOOL &bAllowTips)
{
	//  NYI
}


//  ----
void CVirtualGridCtrl::Change(CGridCell &cell, BOOL &bSelected)
{
	//  NYI
}


//  ----
void CVirtualGridCtrl::ChangeColumns()
{
	//  NYI
}


//  ---
void CVirtualGridCtrl::ChangeEditing()
{
	//  NYI
}


//  --
void CVirtualGridCtrl::ChangeEditMode()
{
	//  NYI
}


//  ---
void CVirtualGridCtrl::ChangeFixed()
{
	//  NYI
}


//  ---
void CVirtualGridCtrl::ChangeRows()
{
	//  NYI
}


//  ---
void CVirtualGridCtrl::Changing(CGridCell &cell, BOOL &bSelected)
{
	//  NYI
}


//  ---
void CVirtualGridCtrl::CheckClick(const CGridCell &cell)
{
	CWnd *pWnd;
	LRESULT lResult;

	pWnd = GetParent();
	ASSERT(pWnd);
	ASSERT(::IsWindow(pWnd->GetSafeHwnd()));

	m_dispInfo.hdr.code = VGN_CHECKCLICK;
	m_dispInfo.hdr.hwndFrom = m_hWnd;
	m_dispInfo.hdr.idFrom = GetDlgCtrlID();

	m_dispInfo.item.nColumn = cell.m_nColumn;
	m_dispInfo.item.nRow = cell.m_nRow;

	lResult = pWnd->SendMessage(WM_NOTIFY, m_dispInfo.hdr.idFrom, (LPARAM)&m_dispInfo);
}


//  ----
void CVirtualGridCtrl::ColumnAutoSize(int nColumn, int &nWidth)
{
	//  NYI
}


//  ---
void CVirtualGridCtrl::ColumnResize(int nColumn, int &nWidth)
{
	//  NYI
}


//  ---
void CVirtualGridCtrl::ColumnResizing(int nColumn, int &nWidth)
{
	//  NYI
}




//  -------------------------------------------------------------------
CGridEdit * CVirtualGridCtrl::CreateEdit(CRuntimeClass *pEditClass)
{
	CGridEdit *pEdit;

	if (pEditClass == NULL)
		pEditClass = RUNTIME_CLASS(CGridEdit);
	pEdit = (CGridEdit *)pEditClass->CreateObject();

	return pEdit;
}



//  ----
void CVirtualGridCtrl::EditButtonPress(const CGridCell &cell)
{
	CWnd *pWnd;
	LRESULT lResult;

	pWnd = GetParent();
	ASSERT(pWnd);
	ASSERT(::IsWindow(pWnd->GetSafeHwnd()));

	m_dispInfo.hdr.code = VGN_EDITBUTTONPRESS;
	m_dispInfo.hdr.hwndFrom = m_hWnd;
	m_dispInfo.hdr.idFrom = GetDlgCtrlID();

	m_dispInfo.item.nColumn = cell.m_nColumn;
	m_dispInfo.item.nRow = cell.m_nRow;
	lResult = pWnd->SendMessage(WM_NOTIFY, m_dispInfo.hdr.idFrom, (LPARAM)&m_dispInfo);
}


//  --
BOOL CVirtualGridCtrl::EditCanAcceptKey(const CGridCell &cell, char key)
{
	CWnd *pWnd;
	LRESULT lResult;

	pWnd = GetParent();
	ASSERT(pWnd);
	ASSERT(::IsWindow(pWnd->GetSafeHwnd()));

	m_dispInfo.hdr.code = VGN_EDITACCEPTKEY;
	m_dispInfo.hdr.hwndFrom = m_hWnd;
	m_dispInfo.hdr.idFrom = GetDlgCtrlID();

	m_dispInfo.item.bEditAcceptKey = IsCellValid(cell);
	m_dispInfo.item.nColumn = cell.m_nColumn;
	m_dispInfo.item.nRow = cell.m_nRow;

	lResult = pWnd->SendMessage(WM_NOTIFY, m_dispInfo.hdr.idFrom, (LPARAM)&m_dispInfo);

	return m_dispInfo.item.bEditAcceptKey;
}


//  --
void CVirtualGridCtrl::EditCanceled(const CGridCell &cell)
{
	CWnd *pWnd;
	LRESULT lResult;

	pWnd = GetParent();
	ASSERT(pWnd);
	ASSERT(::IsWindow(pWnd->GetSafeHwnd()));

	m_dispInfo.hdr.code = VGN_EDITCANCELED;
	m_dispInfo.hdr.hwndFrom = m_hWnd;
	m_dispInfo.hdr.idFrom = GetDlgCtrlID();

	m_dispInfo.item.nColumn = cell.m_nColumn;
	m_dispInfo.item.nRow = cell.m_nRow;

	lResult = pWnd->SendMessage(WM_NOTIFY, m_dispInfo.hdr.idFrom, (LPARAM)&m_dispInfo);
}



//  --
BOOL CVirtualGridCtrl::EditCanModify(const CGridCell &cell)
{
	BOOL bCanModify = !IsCellReadOnly(cell);
	//  NYI
	return bCanModify;
}


//  --
BOOL CVirtualGridCtrl::EditCanShow(const CGridCell &cell)
{
	//  Is there any cells?
	if ((0 == m_columns.GetSize() - m_fixed.m_nCount) || !m_rows.GetCount())
		return FALSE;

	return m_bAllowEdit && (m_bAlwaysEdit || IsActiveControl());
}


//  --
BOOL CVirtualGridCtrl::EditCanUndo(const CGridCell &cell)
{
	return EditCanModify(cell);
}


//  ----
void CVirtualGridCtrl::EditChange(const CGridCell &cell)
{
	//  NYI
}


//  ---
void CVirtualGridCtrl::EditCloseUp(const CGridCell &cell, int nItemIndex, BOOL &bAccept)
{
	//  NYI
}


//  ---
void CVirtualGridCtrl::EditSelectNext(const CGridCell &cell, CString &strText)
{
	//  NYI
}



//  ----
void CVirtualGridCtrl::GetCellColors(const CGridCell &cell, CDC *pDC)
{
	CWnd *pWnd;
	LRESULT lResult;

	//  --------  Fixed part  -----------
	if (cell.m_nColumn < m_fixed.GetCount())  {
		pDC->SetBkColor(m_fixed.m_color);
		pDC->SelectObject(m_fixed.m_pFont);
	}  else  {
		//  ------  Regular cell  ----------
		pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));
		pDC->SelectObject(&m_font);

		if (!IsWindowEnabled())
			pDC->SetTextColor(::GetSysColor(COLOR_GRAYTEXT));

		//  Selected cell
		if (IsWindowEnabled() && IsFocusAllowed() && m_bCellSelected && IsCellFocused(cell))  {
			//  Is the grid focused?
			if (this == GetFocus())  {
				pDC->SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
				pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
			}  else if (!m_bHideSelection)  {
				pDC->SetBkColor(::GetSysColor(COLOR_BTNFACE));
			}
		}
	}

	if (IsCellFocused(cell))
		return;

	pWnd = GetParent();
	ASSERT(pWnd);
	ASSERT(::IsWindow(pWnd->m_hWnd));

	m_dispInfo.hdr.code = VGN_GETDISPINFO;
	m_dispInfo.hdr.hwndFrom = m_hWnd;
	m_dispInfo.hdr.idFrom = GetDlgCtrlID();

	m_dispInfo.item.mask = LVIF_COLOR;
	m_dispInfo.item.nColumn = cell.m_nColumn;
	m_dispInfo.item.nRow = cell.m_nRow;
	m_dispInfo.item.pDC = pDC;

	lResult = pWnd->SendMessage(WM_NOTIFY, m_dispInfo.hdr.idFrom, (LPARAM)&m_dispInfo);
}



//  ----
int CVirtualGridCtrl::GetCellImage(const CGridCell &cell)
{
	CWnd *pWnd;
	LRESULT lResult;

	pWnd = GetParent();
	ASSERT(pWnd);
	ASSERT(::IsWindow(pWnd->m_hWnd));

	m_dispInfo.hdr.code = VGN_GETDISPINFO;
	m_dispInfo.hdr.hwndFrom = m_hWnd;
	m_dispInfo.hdr.idFrom = GetDlgCtrlID();

	m_dispInfo.item.mask = LVIF_IMAGE;
	m_dispInfo.item.nColumn = cell.m_nColumn;
	m_dispInfo.item.nRow = cell.m_nRow;
	m_dispInfo.item.nImage = -1;	//  Default: NO IMAGE
	lResult = pWnd->SendMessage(WM_NOTIFY, m_dispInfo.hdr.idFrom, (LPARAM)&m_dispInfo);

	return m_dispInfo.item.nImage;
}



//  ----
CPoint CVirtualGridCtrl::GetCellImageIndent(const CGridCell &cell)
{
	CPoint point(m_nImageLeftIndent, m_nImageTopIndent);
	CWnd *pWnd;
	LRESULT lResult;

	//  Take into account 3D effect...
	if (m_fixed.GetCount() > 0 && !m_fixed.GetFlat())
		point.y ++;

	pWnd = GetParent();
	ASSERT(pWnd);
	ASSERT(::IsWindow(pWnd->m_hWnd));

	m_dispInfo.hdr.code = VGN_GETCELLIMAGEINDENT;
	m_dispInfo.hdr.hwndFrom = m_hWnd;
	m_dispInfo.hdr.idFrom = GetDlgCtrlID();

	m_dispInfo.item.nColumn = cell.m_nColumn;
	m_dispInfo.item.nRow = cell.m_nRow;
	m_dispInfo.item.indent = point;

	lResult = pWnd->SendMessage(WM_NOTIFY, m_dispInfo.hdr.idFrom, (LPARAM)&m_dispInfo);

	return point;
}


//  -----
CRect CVirtualGridCtrl::GetCellImageRect(const CGridCell &cell)
{
	CRect rect(0, 0, 0, 0);
	IMAGEINFO ii;
	BOOL bSuccess;

	if (m_pImageList == NULL)
		return rect;

	bSuccess = m_pImageList->GetImageInfo(0, &ii);
	ASSERT(bSuccess);

	//  Is there any pictures at all?
	if (!IsCellHasImage(cell))
		return rect;

	rect = GetCellRect(cell);

	//  Take into account the check
	if (IsCellHasCheck(cell))
		rect.left += (m_nCheckWidth + GetCheckIndent(cell).x);

	//  Image rectangle...
	rect.left += GetCellImageIndent(cell).x;
	rect.top += GetCellImageIndent(cell).y;
	rect.right = min(rect.left + (ii.rcImage.right - ii.rcImage.left), rect.right);
	rect.bottom = rect.top + (ii.rcImage.bottom - ii.rcImage.top);

	return rect;
}



//  -----
CRect CVirtualGridCtrl::GetCellHintRect(const CGridCell &cell)
{
	CRect rect;

	rect = GetEditRect(cell);

	//  NYI
	return rect;
}



//  ------
CString CVirtualGridCtrl::GetCellText(const CGridCell &cell)
{
	CWnd *pWnd;
	LRESULT lResult;

	pWnd = GetParent();
	ASSERT(pWnd);
	ASSERT(::IsWindow(pWnd->m_hWnd));

	m_dispInfo.hdr.code = VGN_GETDISPINFO;
	m_dispInfo.hdr.hwndFrom = m_hWnd;
	m_dispInfo.hdr.idFrom = GetDlgCtrlID();

	m_dispInfo.item.mask = LVIF_TEXT;
	m_dispInfo.item.nColumn = cell.m_nColumn;
	m_dispInfo.item.nRow = cell.m_nRow;
	m_dispInfo.item.strText.Empty();

	lResult = pWnd->SendMessage(WM_NOTIFY, m_dispInfo.hdr.idFrom, (LPARAM)&m_dispInfo);

	return m_dispInfo.item.strText;
}


// ------
CRect CVirtualGridCtrl::GetCellTextBounds(const CGridCell &cell)
{
	CDC *pDC;
	CPoint point;
	CString strText;
	CGridColumn *pColumn;
	int nRetVal;

	//  Check cell's column
	if (cell.m_nColumn < 0 || cell.m_nColumn > m_columns.GetUpperBound())
		return CRect(0, 0, 0, 0);

	pDC = GetDC();
	//  Determine the colors...
	if (cell.m_nRow >= 0 && cell.m_nRow < m_rows.GetCount())  {
		GetCellColors(cell, pDC);
		point = GetCellTextIndent(cell);
		strText = GetCellText(cell);
	}

	//  Paint parameters
	CRect rect(0, 0, 0, 0);

	pColumn = m_columns[cell.m_nColumn];

	if (pColumn->GetWordWrap())  {
		rect = GetEditRect(cell);
		rect.OffsetRect(-rect.left, -rect.top);
		rect.bottom = rect.top;
	}

	LPTSTR lpBuffer = new TCHAR[strText.GetLength() + 1];

	lstrcpy(lpBuffer, strText);

	//  Calculate text rectangle
	rect = GetTextRect(
		pDC,
		rect,
		point.x,
		point.y,
		pColumn->GetAlignment(),
		pColumn->m_bWantReturns,
		pColumn->m_bWordWrap,
		lpBuffer
		);

	delete lpBuffer;


	//  Set the upper-left corner to 0, 0
	rect.OffsetRect(-rect.left, -rect.top);

	nRetVal = ReleaseDC(pDC);
	ASSERT(nRetVal);

	return rect;
}


//  -----
CPoint CVirtualGridCtrl::GetCellTextIndent(const CGridCell &cell)
{
	CWnd *pWnd;
	LRESULT lResult;

	pWnd = GetParent();
	ASSERT(pWnd);
	ASSERT(::IsWindow(pWnd->m_hWnd));

	m_dispInfo.hdr.code = VGN_GETCELLTEXTINDENT;
	m_dispInfo.hdr.hwndFrom = m_hWnd;
	m_dispInfo.hdr.idFrom = GetDlgCtrlID();

	m_dispInfo.item.nColumn = cell.m_nColumn;
	m_dispInfo.item.nRow = cell.m_nRow;

	//  Default values
	m_dispInfo.item.indent.x = m_nTextLeftIndent;
	m_dispInfo.item.indent.y = m_nTextTopIndent;

	//  Taking into account images and 3D-effect
	if (IsCellHasCheck(cell) || IsCellHasImage(cell))  {
		m_dispInfo.item.indent.x = 2;

		if (m_fixed.GetCount() > 0 && !m_fixed.m_bFlat)
			m_dispInfo.item.indent.y ++;
	}

	lResult = pWnd->SendMessage(WM_NOTIFY, m_dispInfo.hdr.idFrom, (LPARAM)&m_dispInfo);

	//  NYI
	return m_dispInfo.item.indent;
}


//  --
int CVirtualGridCtrl::GetCheckAlignment(const CGridCell &cell)
{
	//  NYI
	return LVCFMT_LEFT;
}



//  --
void CVirtualGridCtrl::GetCheckImage(const CGridCell &cell, CBitmap *pCheckImage)
{
	//  NYI
}


//  --
CPoint CVirtualGridCtrl::GetCheckIndent(const CGridCell &cell)
{
	//  NYI
	return CPoint(0, 0);
}


//  --
GridCheckKind CVirtualGridCtrl::GetCheckKind(const CGridCell &cell)
{
	CWnd *pWnd;
	LRESULT lResult;

	if (!m_bCheckBoxes || (cell.m_nColumn < 0) || (cell.m_nColumn > m_columns.GetUpperBound()))
		return gcNone;

	pWnd = GetParent();
	ASSERT(pWnd);
	ASSERT(::IsWindow(pWnd->GetSafeHwnd()));

	m_dispInfo.hdr.code = VGN_GETCHECKKIND;
	m_dispInfo.hdr.hwndFrom = m_hWnd;
	m_dispInfo.hdr.idFrom = GetDlgCtrlID();

	m_dispInfo.item.nColumn = cell.m_nColumn;
	m_dispInfo.item.nRow = cell.m_nRow;
	m_dispInfo.item.checkKind = m_columns[cell.m_nColumn]->m_checkKind;

	lResult = pWnd->SendMessage(WM_NOTIFY, m_dispInfo.hdr.idFrom, (LPARAM)&m_dispInfo);

	return m_dispInfo.item.checkKind;
}



//  ---
CRect CVirtualGridCtrl::GetCheckRect(const CGridCell &cell)
{
	CRect rect;

	//  Is there check box?
	if (!IsCellHasCheck(cell))
		return CRect(0, 0, 0, 0);

	rect = GetCellRect(cell);

	//  Check box rectangle...
	rect.left += GetCheckIndent(cell).x;
	rect.right = min(rect.left + m_nCheckWidth, rect.right);
	rect.top += GetCheckIndent(cell).y;
	rect.bottom = rect.top + m_nCheckHeight;

	return rect;
}


//  --
int CVirtualGridCtrl::GetCheckState(const CGridCell &cell)
{
	LRESULT lResult;
	CWnd *pWnd;

	pWnd = GetParent();
	ASSERT(pWnd);
	ASSERT(::IsWindow(pWnd->GetSafeHwnd()));

	m_dispInfo.hdr.code = VGN_GETCHECKSTATE;
	m_dispInfo.hdr.hwndFrom = m_hWnd;
	m_dispInfo.hdr.idFrom = GetDlgCtrlID();

	m_dispInfo.item.nCheckState = 0;	//  Unchecked is default
	m_dispInfo.item.nColumn = cell.m_nColumn;
	m_dispInfo.item.nRow = cell.m_nRow;

	lResult = pWnd->SendMessage(WM_NOTIFY, m_dispInfo.hdr.idFrom, (LPARAM)&m_dispInfo);

	return m_dispInfo.item.nCheckState;
}


//  --
CPoint CVirtualGridCtrl::GetClientOrigin()
{
	CRect rect;

	GetClientRect(rect);
	return rect.TopLeft();
}



//  --
CRuntimeClass * CVirtualGridCtrl::GetColumnClass()
{
	return RUNTIME_CLASS(CGridColumn);
}


//  --
CGridCell CVirtualGridCtrl::GetCursorCell(CGridCell &cell, GridCursorOffset offset)
{
	switch (offset)  {
	case goLeft:
		return MoveLeft(cell, 1);
	case goRight:
		return MoveRight(cell, 1);
	case goUp:
		return MoveUp(cell, 1);
	case goDown:
		return MoveDown(cell, 1);
	case goPageUp:
		return MoveUp(cell, m_visSize.m_nRow - 1);
	case goPageDown:
		return MoveDown(cell, m_visSize.m_nRow - 1);
	case goHome:
		return MoveHome(cell);
	case goEnd:
		return MoveEnd(cell);
	case goGridHome:
		return MoveGridHome(cell);
	case goGridEnd:
		return MoveGridEnd(cell);
	case goSelect:
		return Select(cell);
	case goFirst:
		return First();
	case goNext:
		return Next(cell);
	case goPrev:
		return Prev(cell);
	default:
		break;
	}

	return cell;
}



//  --
CRuntimeClass * CVirtualGridCtrl::GetEditClass(const CGridCell &cell)
{
	return RUNTIME_CLASS(CGridEdit);
}



//  --
void CVirtualGridCtrl::GetEditList(const CGridCell &cell, CStringArray &items)
{
	LRESULT lResult;
	CWnd *pWnd;
	CGridColumn *pColumn;

	pColumn = m_columns[cell.m_nColumn];

	pWnd = GetParent();
	ASSERT(pWnd);
	ASSERT(::IsWindow(pWnd->m_hWnd));

	m_dispInfo.hdr.code = VGN_GETEDITLIST;
	m_dispInfo.hdr.hwndFrom = m_hWnd;
	m_dispInfo.hdr.idFrom = GetDlgCtrlID();

	m_dispInfo.item.nColumn = cell.m_nColumn;
	m_dispInfo.item.nRow = cell.m_nRow;
	m_dispInfo.item.listStrings.RemoveAll();	//  !!!
	m_dispInfo.item.listStrings.Append(pColumn->m_pickList);

	lResult = pWnd->SendMessage(WM_NOTIFY, m_dispInfo.hdr.idFrom, (LPARAM)&m_dispInfo);

	items.RemoveAll();	//  Just reinsurance...

	items.Append(m_dispInfo.item.listStrings);
}


//  --
void CVirtualGridCtrl::GetEditListBounds(const CGridCell &cell, LPRECT lpRect)
{
	//  NYI
}


//  --
CString CVirtualGridCtrl::GetEditMask(const CGridCell &cell)
{
	CString strText;

	//  NYI
	return strText;
}


//  --
GridEditStyle CVirtualGridCtrl::GetEditStyle(const CGridCell &cell)
{
	CWnd *pWnd;
	LRESULT lResult;

	if (cell.m_nColumn < 0 || cell.m_nColumn > m_columns.GetUpperBound())
		return geSimple;

	pWnd = GetParent();
	ASSERT(pWnd);
	ASSERT(::IsWindow(pWnd->m_hWnd));

	m_dispInfo.hdr.code = VGN_GETEDITSTYLE;
	m_dispInfo.hdr.hwndFrom = m_hWnd;
	m_dispInfo.hdr.idFrom = GetDlgCtrlID();

	m_dispInfo.item.nColumn = cell.m_nColumn;
	m_dispInfo.item.nRow = cell.m_nRow;
	m_dispInfo.item.style = m_columns[cell.m_nColumn]->GetEditStyle();

	lResult = pWnd->SendMessage(WM_NOTIFY, m_dispInfo.hdr.idFrom, (LPARAM)&m_dispInfo);

	return m_dispInfo.item.style;
}


//  --
CString CVirtualGridCtrl::GetEditText(const CGridCell &cell)
{
	CString strText;

	strText = GetCellText(cell);

	//  NYI
	return strText;
}


//  --
COLORREF CVirtualGridCtrl::GetGridLineColor(COLORREF bkColor)
{
	return (::GetSysColor(COLOR_ACTIVEBORDER));
}


//  --
int CVirtualGridCtrl::GetHeaderImage(CGridHeaderSection *pSection)
{
	LRESULT lResult;
	CWnd *pWnd;

	if (m_header.m_pImageList == NULL)
		return -1;

	if (!m_header.m_pImageList->GetImageCount())
		return -1;

	pWnd = GetParent();
	ASSERT(pWnd);
	ASSERT(::IsWindow(pWnd->GetSafeHwnd()));

	m_dispInfo.hdr.code = VGN_GETHEADERIMAGE;
	m_dispInfo.hdr.hwndFrom = m_hWnd;
	m_dispInfo.hdr.idFrom = GetDlgCtrlID();

	m_dispInfo.item.pSection = pSection;
	//  Default header image is its column index by default
	m_dispInfo.item.nImage = pSection->GetColumnIndex();

	lResult = pWnd->SendMessage(WM_NOTIFY, m_dispInfo.hdr.idFrom, (LPARAM)&m_dispInfo);

	//  Just reinsurance...
	if (m_dispInfo.item.nImage >= m_header.m_pImageList->GetImageCount())
		m_dispInfo.item.nImage = -1;

	return m_dispInfo.item.nImage;
}


//  --
void CVirtualGridCtrl::GetHeaderColors(CGridHeaderSection *pSection, CDC *pDC)
{
	//  NYI
}


//  --
GridSortDirection CVirtualGridCtrl::GetSortDirection(CGridHeaderSection *pSection)
{
	//  NYI
	return gsNone;
}


//  ----
void CVirtualGridCtrl::GetSortImage(CGridHeaderSection *pSection, CBitmap *pSortImage)
{
	//  NYI
}



//  ---
CRect CVirtualGridCtrl::GetTextRect(
				    CDC *pDC,
				    LPRECT lpRect,
				    int nLeftIndent,
				    int nTopIndent,
				    int nAlignment,
				    BOOL bWantReturns,
				    BOOL bWordWrap,
				    LPTSTR lpText
				    )
{
	CRect rect(*lpRect);
	DRAWTEXTPARAMS dtp;
	UINT nFlags;
	int nRetVal;
	int nWidth, nHeight;

	//  Let's check how to paint text: using DrawTextEx or TextOut
	if (bWantReturns || bWordWrap || m_bEndEllipsis)  {
		dtp.cbSize = sizeof(DRAWTEXTPARAMS);
		dtp.iLeftMargin = nLeftIndent;
		dtp.iRightMargin = m_nTextRightIndent;

		//  Text attributes
		nFlags = DT_NOPREFIX;

		//  Horizontal alignment...
		switch (nAlignment)  {
		case LVCFMT_LEFT:
			nFlags |= DT_LEFT;
			break;
		case LVCFMT_RIGHT:
			nFlags |= DT_RIGHT;
			break;
		case LVCFMT_CENTER:
			nFlags |= DT_CENTER;
			break;
		default:
			break;
		}

		//  Vertical alignment...
		if (!(bWantReturns || bWordWrap))
			//  Automatic alignment
			nFlags |= (DT_SINGLELINE | DT_VCENTER);

		//  Word wrap...
		if (bWordWrap)
			nFlags |= DT_WORDBREAK;

		//  Text rectangle
		rect = *lpRect;

		//  Calculate text rectangle
		nRetVal = DrawTextEx(pDC->m_hDC, lpText, lstrlen(lpText), rect, nFlags | DT_CALCRECT, &dtp);

		nWidth = max(rect.Width(), lpRect->right - lpRect->left);
		nHeight = max(rect.Height(), lpRect->bottom - lpRect->top);
	}  else  {
		CSize mySize;

		mySize = pDC->GetTextExtent(lpText);
		nWidth = max(rect.Width(), nLeftIndent + mySize.cx + m_nTextRightIndent);
		nHeight = max(rect.Height(), mySize.cy);
	}

	//  Mould the rectangle
	switch (nAlignment)  {
	case LVCFMT_CENTER:
		rect.left = lpRect->left - (nWidth - (lpRect->right - lpRect->left)) / 2;
		rect.right = rect.left + nWidth;
		break;
	case LVCFMT_RIGHT:
		rect.right = lpRect->right;
		rect.left = rect.right - nWidth;
		break;
	default:
		rect.left = lpRect->left;
		rect.right = rect.left + nWidth;
		break;
	}

	rect.top = lpRect->top;
	rect.bottom = rect.top + nHeight;

	return rect;
}



//  ---------  Header click handler  ---------
void CVirtualGridCtrl::HeaderClick(CGridHeaderSection *pSection)
{
	CWnd *pWnd;
	LRESULT lResult;

	pWnd = GetParent();
	ASSERT(pWnd);
	ASSERT(::IsWindow(pWnd->GetSafeHwnd()));

	m_dispInfo.hdr.code = VGN_HEADERCLICK;
	m_dispInfo.hdr.hwndFrom = m_hWnd;
	m_dispInfo.hdr.idFrom = GetDlgCtrlID();

	m_dispInfo.item.pSection = pSection;
	
	lResult = pWnd->SendMessage(WM_NOTIFY, m_dispInfo.hdr.idFrom, (LPARAM)&m_dispInfo);
}


//  ---
void CVirtualGridCtrl::HeaderClicking(CGridHeaderSection *pSection, BOOL &bAllowClick)
{
	CWnd *pWnd;
	LRESULT lResult;

	//  By default only bottom-level sections is allowed to click
	bAllowClick = m_bColumnClick && pSection->GetAllowClick()
		&& !pSection->GetSections()->GetSize();

	pWnd = GetParent();
	ASSERT(pWnd);
	ASSERT(::IsWindow(pWnd->GetSafeHwnd()));

	m_dispInfo.hdr.code = VGN_HEADERCLICKING;
	m_dispInfo.hdr.hwndFrom = m_hWnd;
	m_dispInfo.hdr.idFrom = GetDlgCtrlID();

	m_dispInfo.item.pSection = pSection;
	m_dispInfo.item.bAcceptCursor = bAllowClick;	//  !!!!

	lResult = pWnd->SendMessage(WM_NOTIFY, m_dispInfo.hdr.idFrom, (LPARAM)&m_dispInfo);
	bAllowClick = m_dispInfo.item.bAcceptCursor;
}


//  ---
void CVirtualGridCtrl::HideCursor()
{
	if (IsFocusAllowed())
		InvalidateFocus();
	else
		HideEdit();
}


//  --
void CVirtualGridCtrl::HideFocus()
{
	CDC *pDC;
	int nRetVal;

	pDC = GetDC();

	if (IsFocusAllowed())
		PaintFocus(pDC);

	nRetVal = ReleaseDC(pDC);
	ASSERT(nRetVal);
}



//  ---
void CVirtualGridCtrl::PaintCell(CDC *pDC, const CGridCell &cell, LPRECT lpRect)
{
	CWnd *pWnd;
	LRESULT lResult;

	GetCellColors(cell, pDC);

	pWnd = GetParent();
	ASSERT(pWnd);
	ASSERT(::IsWindow(pWnd->m_hWnd));

	m_dispInfo.hdr.code = VGN_DRAWCELL;
	m_dispInfo.hdr.hwndFrom = m_hWnd;
	m_dispInfo.hdr.idFrom = GetDlgCtrlID();

	m_dispInfo.item.bDefDraw = TRUE;	//  !!!!  DEFAULT  !!!!!
	m_dispInfo.item.nColumn = cell.m_nColumn;
	m_dispInfo.item.nRow = cell.m_nRow;

	m_dispInfo.item.rc.left = lpRect->left;
	m_dispInfo.item.rc.top = lpRect->top;
	m_dispInfo.item.rc.right = lpRect->right;
	m_dispInfo.item.rc.bottom = lpRect->bottom;

	lResult = pWnd->SendMessage(WM_NOTIFY, m_dispInfo.hdr.idFrom, (LPARAM)&m_dispInfo);

	if (m_dispInfo.item.bDefDraw)
		DefaultDrawCell(pDC, cell, lpRect);

	//  !!!!!!!!  Restore  !!!!!!!!!!!
	pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));
	pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
}


//  ---
void CVirtualGridCtrl::PaintDotGridLines(CDC *pDC, LPPOINT lpPoints, int nCount)
{
	int i = 0;
	CRect rect;

	PreparePatternBitmap(pDC, GetGridLineColor(pDC->GetBkColor()), FALSE);

	__try
	{
		//  -----------  Paint the lines  -----------
		while (i < nCount)  {
			//  Line position
			rect.left = lpPoints[i].x;
			rect.top = lpPoints[i].y;
			i ++;
			rect.right = lpPoints[i].x;
			rect.bottom = lpPoints[i].y;
			i ++;

			//  Check the rectangle
			if ((rect.left == rect.right) && (rect.top != rect.bottom))
				rect.right ++;
			else if ((rect.left != rect.right) && (rect.top == rect.bottom))
				rect.bottom ++;

			pDC->FillRect(rect, pDC->GetCurrentBrush());
		}
	}
	__finally
	{
		PreparePatternBitmap(pDC, GetGridLineColor(pDC->GetBkColor()), TRUE);
	}
}


//  -----
void CVirtualGridCtrl::PaintFixed(CDC *pDC)
{
	int i, j;
	int nWidth;
	CRect rect;
	CGridCell cell;

	//  Top border of rows...
	rect.bottom = GetRowTopBottom(m_visOrigin.m_nRow).top;
	
	//  Look through the lines...
	for (j = 0; j < m_visSize.m_nRow; j ++)  {
		rect.top = rect.bottom;
		rect.bottom += m_rows.m_nHeight;
		rect.right = GetGridRect().left;

		//  Look through the columns...
		for (i = 0; i < m_fixed.GetCount(); i ++)  {
			cell.m_nColumn = i;
			cell.m_nRow = m_visOrigin.m_nRow + j;
			nWidth = m_columns[cell.m_nColumn]->GetWidth();

			if (nWidth > 0)  {
				rect.left = rect.right;
				rect.right += nWidth;

				if (pDC->RectVisible(rect))
					PaintCell(pDC, cell, rect);
			}
		}
	}

	//  --------------  Line to the right  -----------------
	if (m_fixed.m_bFlat)  {
		rect = GetFixedRect();

		//  If the colors of fixed part and the table are the same then we will
		//  paint the single line
		if (m_fixed.m_bGridColor)  {
			if (gsDotLines & m_nGridStyles)  {
				rect.left = rect.right - 1;
				PaintDotGridLines(pDC, (LPPOINT)&rect, 2);
			}  else  {
				CPen *pOldPen;
				CPoint point;
				BOOL bSuccess;

				pOldPen = pDC->SelectObject(&m_penBtnShadow);

				point = pDC->MoveTo(rect.right - 1, rect.bottom - 1);
				bSuccess = pDC->LineTo(rect.right - 1, rect.top - 1);
				ASSERT(bSuccess);

				pOldPen = pDC->SelectObject(&m_penBtnShadow);
			}
		}  else  {
			//  ----------  Otherwise we will paint double line  -------------
			CPen *pOldPen;
			CPoint point;
			BOOL bSuccess;

			pOldPen = pDC->SelectObject(&m_penBtnShadow);
			point = pDC->MoveTo(rect.right - 2, rect.top - 1);
			bSuccess = pDC->LineTo(rect.right - 2, rect.bottom);
			ASSERT(bSuccess);

			pDC->SelectObject(&m_penBtnHighlight);
			point = pDC->MoveTo(rect.right - 1, rect.bottom - 1);
			bSuccess = pDC->LineTo(rect.right - 1, rect.top - 1);
			ASSERT(bSuccess);
		}
	}
}



//  ---
void CVirtualGridCtrl::PaintFixedGrid(CDC *pDC)
{
	int nStrokeCount;
	int nPointCount;
	int nLeft, nTop, nRight, nBottom;
	int x, y;
	int i, nIndex, c;
	LPPOINT lpPoints;
	LPDWORD lpPolyPoints;
	CRect rect;
	BOOL bSuccess;

	if (m_fixed.m_bFlat)  {
		nStrokeCount = 0;

		if (gsHorzLine & m_nGridStyles)  {
			nStrokeCount = m_visSize.m_nRow;
			if (gsListViewLike & m_nGridStyles)
				nStrokeCount = GetGridHeight() / m_rows.m_nHeight;
		}

		if (gsVertLine & m_nGridStyles)
			nStrokeCount += m_fixed.GetCount();

		//  Is there a mesh?
		if (nStrokeCount > 0)  {
			//  Calculate amount of mesh points
			nPointCount = nStrokeCount * 2;

			//  Get the memory for lines...
			lpPoints = new POINT[nPointCount];
			lpPolyPoints = new DWORD[nStrokeCount];

			for (i = 0; i < nStrokeCount; i ++)
				lpPolyPoints[i] = 2;

			rect = GetFixedRect();

			//  --------  Vertical line points...  ----
			if (gsVertLine & m_nGridStyles)  {
				nTop = rect.top;
				nBottom = rect.bottom;

				if (!(gsFullVertLine & m_nGridStyles) && !(gsListViewLike & m_nGridStyles))
					nBottom = GetRowTopBottom(m_visOrigin.m_nRow + m_visSize.m_nRow).top;

				x = rect.left;

				for (i = 0; i < m_fixed.GetCount(); i ++)  {
					x += m_columns[i]->GetWidth();
					nIndex = i * 2;
					lpPoints[nIndex].x = x - 2;
					lpPoints[nIndex].y = nTop;
					lpPoints[nIndex + 1].x = x - 2;
					lpPoints[nIndex + 1].y = nBottom;
				}
			}

			//  ----------  Horizontal line points...  -----------
			if (gsHorzLine & m_nGridStyles)  {
				nLeft = rect.left;
				nRight = rect.right;
				y = GetRowTopBottom(m_visOrigin.m_nRow).top;
				c = m_visSize.m_nRow;

				if (gsListViewLike & m_nGridStyles)
					c = GetGridHeight() / m_rows.m_nHeight;

				for (i = 0; i < c; i ++)  {
					y += m_rows.m_nHeight;
					nIndex = m_fixed.GetCount() * (0 < (gsVertLine & m_nGridStyles)) * 2 + i * 2;
					lpPoints[nIndex].x = nLeft;
					lpPoints[nIndex].y = y - 2;
					lpPoints[nIndex + 1].x = nRight;
					lpPoints[nIndex + 1].y = y - 2;
				}
			}

			//  Double or single lines?
			if (m_fixed.m_bGridColor)  {
				//  Single line...
				//  We must offset lines (they actually has been calculated for first double line)
				ShiftGridPoints(nPointCount, lpPoints, 1, 1);

				if (gsDotLines & m_nGridStyles)  {
					PaintDotGridLines(pDC, lpPoints, nPointCount);
				}  else  {
					CPen *pOldPen;

					pOldPen = pDC->SelectObject(&m_penBtnShadow);
					bSuccess = pDC->PolyPolyline(lpPoints, lpPolyPoints, nStrokeCount);
					ASSERT(bSuccess);

					pOldPen = pDC->SelectObject(pOldPen);
				}

			}  else  {
				//  Double line...
				CPen *pOldPen;

				//  First, paint dark lines...
				pOldPen = pDC->SelectObject(&m_penBtnShadow);
				bSuccess = pDC->PolyPolyline(lpPoints, lpPolyPoints, nStrokeCount);
				ASSERT(bSuccess);

				//  Offset the lines...
				ShiftGridPoints(nPointCount, lpPoints, 1, 1);

				//  Second, paint light lines...
				pDC->SelectObject(&m_penBtnHighlight);
				bSuccess = pDC->PolyPolyline(lpPoints, lpPolyPoints, nStrokeCount);
				ASSERT(bSuccess);

				pOldPen = pDC->SelectObject(pOldPen);

			}

			delete[] lpPoints;
			delete[] lpPolyPoints;
		}
	}  else if ((gsHorzLine & m_nGridStyles) && (gsVertLine & m_nGridStyles))  {
		//  Paint ALL 3D fixed cells
		int nWidth;

		rect = GetFixedRect();

		if (!(gsListViewLike & m_nGridStyles))
			rect.bottom = rect.top + m_visSize.m_nRow * m_rows.m_nHeight;



		//  Paint 3D cells
		CRect cellRect(rect);

		cellRect.bottom = cellRect.top;
		//  Rows...
		while (cellRect.bottom < rect.bottom)  {
			cellRect.top = cellRect.bottom;
			cellRect.bottom += m_rows.m_nHeight;
			cellRect.right = GetFixedRect().left;

			//  Columns...
			for (i = 0; i < m_fixed.GetCount(); i ++)  {
				nWidth = m_columns[i]->GetWidth();
				if (nWidth > 0)  {
					cellRect.left = cellRect.right;
					cellRect.right += nWidth;
					if (pDC->RectVisible(cellRect))
						Paint3DFrame(pDC, cellRect, BF_RECT);
				}
			}
		}

		//  Paint remainder...
		if (!(gsListViewLike & m_nGridStyles))  {
			rect.top = rect.bottom;
			rect.bottom = GetFixedRect().bottom;
			if (gsFullVertLine & m_nGridStyles)	//  Vertical lines only
				PaintVert3DLines(pDC, rect, FALSE);
			else
				PaintBottom3DMargin(pDC, rect);		//  Remainder to the bottom
		}
	}  else if ((gsHorzLine & m_nGridStyles) && !(gsVertLine & m_nGridStyles))  {
		//  We need to paint only 3D horizontal lines
		rect = GetFixedRect();
		if (!(gsListViewLike & m_nGridStyles))
			rect.bottom = rect.top + m_visSize.m_nRow * m_rows.m_nHeight;

		//  Horizontal 3D lines
		PaintHorz3DLines(pDC, rect);

		//  Remainder...
		if (!(gsListViewLike & m_nGridStyles))  {
			rect.top = rect.bottom;
			rect.bottom = GetFixedRect().bottom;
			PaintBottom3DMargin(pDC, rect);
		}
	}  else if (!(gsHorzLine & m_nGridStyles) && (gsVertLine & m_nGridStyles))  {
		//  We need to paint only vertical 3D lines
		rect = GetFixedRect();
		PaintVert3DLines(pDC, rect, FALSE);
	}  else  {
		//  Just 3d frame around fixed part
		rect = GetFixedRect();
		PaintBottom3DMargin(pDC, rect);
	}
}



//  ---
void CVirtualGridCtrl::PaintFreeField(CDC *pDC)
{
	int x, y;
	CRect rect;

	//  Free field to the right...
	x = GetColumnRect(m_visOrigin.m_nColumn + m_visSize.m_nColumn).left;
	rect = GetGridRect();

	if (x < rect.right)  {
		rect.left = x;
		pDC->FillSolidRect(rect, ::GetSysColor(COLOR_WINDOW));
	}

	//  Free field at the bottom...
	y = GetRowRect(m_visOrigin.m_nRow + m_visSize.m_nRow).top;
	rect = GetGridRect();

	if (y < rect.bottom)  {
		rect.left = GetFixedRect().right;
		rect.top = y;

		pDC->FillSolidRect(rect, ::GetSysColor(COLOR_WINDOW));

		//  Free field under the fixed part
		rect.right = rect.left;
		rect.left = GetFixedRect().left;

		if (rect.Width() && rect.Height())
			pDC->FillSolidRect(rect, m_fixed.m_color);
	}

	//  ...Resize line
	if (m_bColResizing && m_nColResizeCount > 0 && !m_bColumnsFullDrag)
		PaintResizeLine();
}


//  ----------------------------------------------------------------------------
void CVirtualGridCtrl::PaintFocus(CDC *pDC)
{
	//  Is focus visible
	if (m_bShowFocusRect && /* m_bFocused */ m_visSize.m_nRow > 0)  {
		CRect rect;
		CRect gridRect;
		int nRetVal;

		//  Focus rectangle
		rect = GetFocusRect();

		//  Taking into account the grid lines...
		if (m_bGridLines)  {
			if (gsVertLine & m_nGridStyles)
				rect.right -= m_nGridLineWidth;
			if (gsHorzLine & m_nGridStyles)
				rect.bottom -= m_nGridLineWidth;
		}

		//  Let's paint...
		pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		pDC->SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
		pDC->SetBkMode(OPAQUE);
		pDC->SetROP2(R2_COPYPEN);

		//  Cut off a place for header and fixed part
		gridRect = GetGridRect();
		gridRect.left = GetFixedRect().right;
		nRetVal = pDC->IntersectClipRect(gridRect);
		ASSERT(nRetVal != ERROR);

		pDC->DrawFocusRect(rect);
	}
}


//  -----------------------------------------------------------------------------
void CVirtualGridCtrl::PaintGridLines(CDC *pDC)
{
	int nStrokeCount;
	int nPointCount;
	int i;
	int nLeft, nTop, nRight, nBottom, x, y, c;
	int nIndex;
	CRect rect;
	LPPOINT lpPoints;
	LPDWORD lpPolyPoints;

	nStrokeCount = 0;

	if (gsHorzLine & m_nGridStyles)  {
		nStrokeCount = m_visSize.m_nRow;

		if (gsListViewLike & m_nGridStyles)
			nStrokeCount = GetGridHeight() / m_rows.m_nHeight;
	}

	if (gsVertLine & m_nGridStyles)
		nStrokeCount += m_visSize.m_nColumn;

	//  Is there a mesh?
	if (nStrokeCount)  {
		//  Let's calculate amount of mesh points
		nPointCount = nStrokeCount * 2;

		//  Allocate memory...
		lpPoints = new POINT[nPointCount];
		lpPolyPoints = new DWORD[nStrokeCount];

		for (i = 0; i < nStrokeCount; i ++)
			lpPolyPoints[i] = 2;

		rect = GetGridRect();

		//  Vertical line points...
		if (gsVertLine & m_nGridStyles)  {
			nTop = rect.top;
			nBottom = rect.bottom;

			if (!(gsFullVertLine & m_nGridStyles) && !(gsListViewLike & m_nGridStyles))
				nBottom = GetRowTopBottom(m_visOrigin.m_nRow + m_visSize.m_nRow).top;

			x = GetColumnLeftRight(m_visOrigin.m_nColumn).left;

			for (i = 0; i < m_visSize.m_nColumn; i ++)  {
				x += m_columns[m_visOrigin.m_nColumn + i]->GetWidth();
				nIndex = i * 2;

				lpPoints[nIndex].x = x - 1;
				lpPoints[nIndex].y = nTop;
				lpPoints[nIndex + 1].x = x - 1;
				lpPoints[nIndex + 1].y = nBottom;
			}
		}

		//  Horizontal line points...
		if (gsHorzLine & m_nGridStyles)  {
			nLeft = rect.left + GetFixedWidth();
			nRight = rect.right;

			if (!(gsFullHorzLine & m_nGridStyles) && !(gsListViewLike & m_nGridStyles))
				nRight = GetColumnLeftRight(m_visOrigin.m_nColumn + m_visSize.m_nColumn).left;

			y = GetRowTopBottom(m_visOrigin.m_nRow).top;

			c = m_visSize.m_nRow;

			if (gsListViewLike & m_nGridStyles)
				c = GetGridHeight() / m_rows.m_nHeight;

			for (i = 0; i < c; i ++)  {
				y += m_rows.m_nHeight;
				nIndex = m_visSize.m_nColumn * ((gsVertLine & m_nGridStyles) > 0) * 2 + i * 2;

				lpPoints[nIndex].x = nLeft;
				lpPoints[nIndex].y = y - 1;
				lpPoints[nIndex + 1].x = nRight;
				lpPoints[nIndex + 1].y = y - 1;
			}
		}

		//  ----------  Let's paint  ------------
		if (!(gsDotLines & m_nGridStyles))  {
			BOOL bSuccess;
			CPen *pOldPen;

			pOldPen = pDC->SelectObject(&m_penGridLines);

			bSuccess = pDC->PolyPolyline(lpPoints, lpPolyPoints, nStrokeCount);
			ASSERT(bSuccess);

			pOldPen = pDC->SelectObject(pOldPen);
		}  else  {
			PaintDotGridLines(pDC, lpPoints, nPointCount);
		}

		delete[] lpPoints;
		delete lpPolyPoints;
	}
}



//  ---
void CVirtualGridCtrl::PaintHeader(CDC *pDC, CGridHeaderSection *pSection, LPRECT lpRect)
{
	GetHeaderColors(pSection, pDC);

	//  NYI

	DefaultDrawHeader(pDC, pSection, lpRect);
}




//  ------------------  Paint ALL of the Grid's Header Sectinons  --------------------
void CVirtualGridCtrl::PaintHeaders(CDC *pDC, BOOL bDrawFixed)
{
	CFont *pOldFont;
	CRect clientRect;
	CRect rect;
	CPen *pOldPen;
	BOOL bSuccess;
	CPoint point;

	pOldFont = pDC->SelectObject(m_header.m_pFont);

	GetClientRect(clientRect);

	//  --------  Subheaders  ----------
	PaintHeaderSections(pDC, &m_header.m_sections, bDrawFixed);

	//  Remaining place to the right...
	rect = GetHeaderRect();

	rect.left = clientRect.left + m_header.GetWidth() + GetGridOrigin().x;
	if (rect.left < rect.right)  {
		pDC->FillSolidRect(rect, m_header.m_color);

		if (!m_header.m_bFlat)
			Paint3DFrame(pDC, rect, BF_LEFT | BF_TOP | BF_BOTTOM);
	}

	//  -----------  Grey line below...  -----------
	if (m_header.m_bFlat)  {
		//  Adjust rectangle borders
		if (bDrawFixed)  {
			rect.left = clientRect.left;
			rect.right = GetFixedRect().right;
		}  else  {
			rect.left = GetFixedRect().left;
			rect.right = clientRect.right;
		}

		//  ------------------  Let's paint  --------------------
		if (m_header.m_bGridColor)  {
			CPen pen;

			bSuccess = pen.CreatePen(PS_SOLID, m_nGridLineWidth, GetGridLineColor(::GetSysColor(COLOR_WINDOW)));
			pOldPen = pDC->SelectObject(&pen);

			point = pDC->MoveTo(rect.left, rect.bottom - 1);
			bSuccess = pDC->LineTo(rect.right, rect.bottom - 1);
			bSuccess = pen.DeleteObject();
			ASSERT(bSuccess);
		}  else  {	//  Otherwise paint double line
			pOldPen = pDC->SelectObject(&m_penBtnShadow);
			point = pDC->MoveTo(rect.left, rect.bottom - 2);
			bSuccess = pDC->LineTo(rect.right, rect.bottom - 2);

			pDC->SelectObject(&m_penBtnHighlight);
			point = pDC->MoveTo(rect.left, rect.bottom - 1);
			bSuccess = pDC->LineTo(rect.right, rect.bottom - 1);
		}

	}

	if (m_bColResizing && m_nColResizeCount > 0 && !m_bColumnsFullDrag)
		PaintResizeLine();

	pOldFont = pDC->SelectObject(pOldFont);
}




//  ---
void CVirtualGridCtrl::PaintHeaderSections(CDC *pDC, CGridHeaderSections *pSections, BOOL bAllowFixed)
{
	int i;
	CGridHeaderSection *pSection;
	CRect rect;
	CRect sectionRect;

	for (i = 0; i < pSections->GetSize(); i ++)  {
		pSection = pSections->GetSection(i);

		if (bAllowFixed == pSection->GetFixedColumn())  {
			rect = pSection->GetBoundsRect();

			if (rect.Width())  {
				sectionRect = rect;

				if (pSections->m_sections.GetSize())
					sectionRect.bottom = GetHeaderRect().bottom;

				if (pDC->RectVisible(sectionRect))  {
					PaintHeader(pDC, pSection, rect);
					PaintHeaderSections(pDC, &pSection->m_sections, bAllowFixed);
				}
			}
		}  else  {
			PaintHeaderSections(pDC, &pSection->m_sections, bAllowFixed);
		}
	}
}



//  -----------------------------------------------------------------------------
void CVirtualGridCtrl::PaintResizeLine()
{
	CDC *pDC;
	CPen *pOldPen;
	BOOL bSuccess;
	int nOldROP2;
	CPoint point;
	CPen pen;

	pDC = GetDC();

	bSuccess = pen.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	ASSERT(bSuccess);

	pOldPen = pDC->SelectObject(&pen);
	nOldROP2 = pDC->SetROP2(R2_XORPEN);

	point = pDC->MoveTo(m_nColResizePos, m_colResizeRect.top);
	bSuccess = pDC->LineTo(m_nColResizePos, m_colResizeRect.bottom);
	ASSERT(bSuccess);

	nOldROP2 = pDC->SetROP2(nOldROP2);
	pOldPen = pDC->SelectObject(pOldPen);

	bSuccess = pen.DeleteObject();
	ASSERT(bSuccess);

	bSuccess = ReleaseDC(pDC);
	ASSERT(bSuccess);
}




//  ----
void CVirtualGridCtrl::PaintText(
				 CDC *pDC,
				 LPRECT lpRect,
				 int nLeftIndent,
				 int nTopIndent,
				 int nAlignment,
				 BOOL bWantReturns,
				 BOOL bWordWrap,
				 LPCTSTR lpText
				 )
{
	DRAWTEXTPARAMS dtp;
	UINT nFlags;
	int nRetVal;
	TCHAR szText[512];

	lstrcpy(szText, lpText);

	//  -----------  Text out  ---------------
	if (bWantReturns || bWordWrap || m_bEndEllipsis)  {
		//  Fill draw text parameters
		dtp.cbSize = sizeof(DRAWTEXTPARAMS);
		dtp.iLeftMargin = nLeftIndent;
		dtp.iRightMargin = m_nTextRightIndent;
		dtp.iTabLength = 0;
		dtp.uiLengthDrawn = 0;

		//  Text attributes
		nFlags = DT_NOPREFIX;

		//  Horizontal alignment
		switch (nAlignment)  {
		case LVCFMT_LEFT:
			nFlags |= DT_LEFT;
			break;
		case LVCFMT_RIGHT:
			nFlags |= DT_RIGHT;
			break;
		case LVCFMT_CENTER:
			nFlags |= DT_CENTER;
			break;
		default:
			break;
		}

		//  Vertical alignment
		if (!(bWantReturns || bWordWrap))  {
			//  Auto alignment
			nFlags |= DT_SINGLELINE;

			if (nAlignment == LVCFMT_LEFT)
				nFlags |= DT_END_ELLIPSIS;
		}

		//  Word wrap
		if (bWordWrap)
			nFlags |= DT_WORDBREAK;

		//  Top indent
		lpRect->top += nTopIndent;

		//  Draw the text
		nRetVal = pDC->SetBkMode(TRANSPARENT);

		nRetVal = DrawTextEx(
			pDC->m_hDC,
			szText,
			lstrlen(szText),
			lpRect,
			nFlags,
			&dtp
			);
	
	}  else  {
		int dx;
		UINT nAl;
		BOOL bSuccess;

		//  Horizontal alignment
		switch (nAlignment)  {
		case LVCFMT_RIGHT:
			dx = lpRect->right - lpRect->left - m_nTextRightIndent;
			nAl = TA_RIGHT;
			break;
		case LVCFMT_CENTER:
			dx = nLeftIndent + (lpRect->right - lpRect->left) / 2;
			nAl = TA_CENTER;
			break;
		default:
			dx = nLeftIndent;
			nAl = TA_LEFT;
			break;
		}

		//  Standard text out
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextAlign(nAl);

		bSuccess = pDC->ExtTextOut(
			lpRect->left + dx,
			lpRect->top + nTopIndent,
			ETO_CLIPPED,
			lpRect,
			szText,
			lstrlen(szText),
			NULL
			);
		ASSERT(bSuccess);
	}
}


//  ---
void CVirtualGridCtrl::PreparePatternBitmap(CDC *pDC, COLORREF fillColor, BOOL bRemove)
{
	SCROLLINFO si;
	BOOL bSuccess;
	COLORREF pattern[4];
	CBrush *pOldBrush;
	DWORD dwBytes;

	if (bRemove)  {
		//  ???
	}

	bSuccess = GetScrollInfo(SB_HORZ, &si, SIF_POS);

	if (div(si.nPos, 2).rem == 0)  {
		pattern[0] = pDC->GetBkColor();
		pattern[1] = fillColor;
		pattern[2] = fillColor;
		pattern[3] = pDC->GetBkColor();
	}  else  {
		pattern[0] = fillColor;
		pattern[1] = pDC->GetBkColor();
		pattern[2] = fillColor;
		pattern[3] = pDC->GetBkColor();
	}

	dwBytes = m_bitmapPattern.SetBitmapBits(4 * sizeof(COLORREF), pattern);

	CBrush brushDotLine;
	bSuccess = brushDotLine.CreatePatternBrush(&m_bitmapPattern);
	ASSERT(bSuccess);

	pOldBrush = pDC->SelectObject(&brushDotLine);
	if (pOldBrush->GetSafeHandle())  {
		bSuccess = pOldBrush->DeleteObject();
	}
}


//  ---
void CVirtualGridCtrl::ResetClickPos()
{
	m_clickPos.m_nColumn = -1;
	m_clickPos.m_nRow = -1;
}


//  ----
void CVirtualGridCtrl::SetEditText(const CGridCell &cell, CString& strText)
{
	CWnd *pWnd;
	LRESULT lResult;

	pWnd = GetParent();
	ASSERT(pWnd);
	ASSERT(::IsWindow(pWnd->m_hWnd));

	m_dispInfo.hdr.code = VGN_SETDISPINFO;
	m_dispInfo.hdr.hwndFrom = m_hWnd;
	m_dispInfo.hdr.idFrom = GetDlgCtrlID();

	m_dispInfo.item.nColumn = cell.m_nColumn;
	m_dispInfo.item.nRow = cell.m_nRow;
	m_dispInfo.item.strText = strText;

	lResult = pWnd->SendMessage(WM_NOTIFY, m_dispInfo.hdr.idFrom, (LPARAM)&m_dispInfo);
	strText = m_dispInfo.item.strText;
}


//  ---
void CVirtualGridCtrl::ShowCursor()
{
	if (IsFocusAllowed())
		InvalidateFocus();
	else
		ShowEdit();
}


//  --
void CVirtualGridCtrl::ShowEdit()
{
	UpdateEdit();
}


//  ---
void CVirtualGridCtrl::ShowEditChar(UINT nChar)
{
	SetEditing();

	if (m_pEdit && m_bEditing)
		m_pEdit->PostMessage(WM_CHAR, nChar, 0);
}


//  ---
void CVirtualGridCtrl::ShowFocus()
{
	CDC *pDC;
	int nRetVal;

	pDC = GetDC();

	if (IsFocusAllowed())
		PaintFocus(pDC);

	nRetVal = ReleaseDC(pDC);
	ASSERT(nRetVal);
}



//  -----------------------------------------------------------------------------------
void CVirtualGridCtrl::StartColResize(CGridHeaderSection *pSection, int x, int y)
{
	CRect clientRect;
	CWnd *pWnd;

	GetClientRect(clientRect);
	m_pColResizeSection = pSection;
	m_nColResizeIndex = pSection->GetResizeColumnIndex();

	//  Horizontal borders...
	if (m_nColResizeIndex <= m_columns.GetUpperBound())  {
		m_colResizeRect = GetColumnRect(m_nColResizeIndex);
		m_colResizeRect.bottom = clientRect.bottom;	//  ???
		m_nColResizeMinWidth = m_columns[m_nColResizeIndex]->GetMinWidth();
		m_nColResizeMaxWidth = m_columns[m_nColResizeIndex]->GetMaxWidth();
	}  else  {
		m_colResizeRect = pSection->GetBoundsRect();
		m_colResizeRect.bottom = clientRect.bottom;
		m_nColResizeMinWidth = 0;
		m_nColResizeMaxWidth = 10000;
	}

	//  Vertical borders...
	m_colResizeRect.top = pSection->GetLevel() * m_header.m_nSectionHeight;
	m_colResizeRect.bottom = clientRect.Height();

	//  Resize line...
	m_nColResizePos = m_colResizeRect.right;
	m_nColResizeOffset = m_nColResizePos - x;

	//  Now we can change columns size
	m_nColResizeCount = 0;
	m_bColResizing = TRUE;

	//  Capture mouse
	pWnd = SetCapture();
}


//  ---
void CVirtualGridCtrl::StartHeaderClick(CGridHeaderSection *pSection, CPoint point)
{
	BOOL bAllowClick;
	CWnd *pWnd;

	bAllowClick = TRUE;
	HeaderClicking(pSection, bAllowClick);

	if (!bAllowClick)
		return;

	//  Memorize parameters...
	m_pHeaderClickSection = pSection;
	m_headerClickRect = pSection->GetBoundsRect();
	m_bHeaderClickState = FALSE;
	m_bHeaderClicking = TRUE;

	pWnd = SetCapture();

	//  Press the 'button'
	StepHeaderClick(point);
}



//  ---
void CVirtualGridCtrl::StepHeaderClick(CPoint point)
{
	BOOL bHit;

	if (!m_bHeaderClicking)
		return;		//  Nothing to stop

	bHit = m_headerClickRect.PtInRect(point);

	//  Is something changed there?
	if (m_bHeaderClickState != bHit)  {
		m_bHeaderClickState = bHit;
		InvalidateRect(m_headerClickRect);
	}
}



//  ------
void CVirtualGridCtrl::StopHeaderClick(BOOL bAbort)
{
	BOOL bHit;
	BOOL bSuccess;

	if (!m_bHeaderClicking)
		return;

	bHit = m_bHeaderClickState;

	//  Button up
	StepHeaderClick(CPoint(-1, -1));

	m_bHeaderClicking = FALSE;
	bSuccess = ReleaseCapture();
	ASSERT(bSuccess);

	if (!bAbort && bHit)
		HeaderClick(m_pHeaderClickSection);
}


//  -----------------------------------------------------------------------
void CVirtualGridCtrl::StepColResize(int x, int y)
{
	int nWidth;
	CRect rect;

	//  No resizing at all...
	if (!m_bColResizing)
		return;

	//  Current resize line position
	x += m_nColResizeOffset;

	//  Current width
	nWidth = x - m_colResizeRect.left;

	//  Adjust width according to borders
	if (nWidth < m_nColResizeMinWidth)
		nWidth = m_nColResizeMinWidth;

	if (nWidth > m_nColResizeMaxWidth)
		nWidth = m_nColResizeMaxWidth;

	ColumnResizing(m_nColResizeIndex, nWidth);

	//  Adjust the width again...
	if (nWidth < m_nColResizeMinWidth)
		nWidth = m_nColResizeMinWidth;

	if (nWidth > m_nColResizeMaxWidth)
		nWidth = m_nColResizeMaxWidth;

	//  New line position
	x = m_colResizeRect.left + nWidth;

	// ---  Paint the line  -----
	if (m_nColResizePos != x)  {
		//  Paint over old line
		if (m_nColResizeCount > 0 && !m_bColumnsFullDrag)
			PaintResizeLine();

		m_nColResizeCount ++;

		//  A new line postion...
		m_nColResizePos = x;

		//  Set the width
		if (m_bColumnsFullDrag && m_nColResizeIndex < m_columns.GetSize())  {
			CRect rect;
			CGridHeaderSection *pSection;

			//  Before changing column width we should calculate and
			//  update changing cells
			GetClientRect(rect);
			rect.left = GetColumnLeftRight(m_nColResizeIndex).left;

			//  If non-fixed column partially overlayed by fixed part
			//  then it's not necessary to repaint it
			if (m_nColResizeIndex >= m_fixed.GetCount())
				rect.left = max(rect.left, GetFixedRect().right);

			if (nWidth < m_columns[m_nColResizeIndex]->GetWidth())  {
				SCROLLINFO si;
				BOOL bSuccess;

				bSuccess = GetScrollInfo(SB_HORZ, &si);
				ASSERT(bSuccess);

				//  if (((si.nMax - si.nMin) > nPageStep) && (si.nPos ==  //  NYI
			}

			InvalidateRect(rect);

			//  If the column has multi-level header then we should additionally
			//  update topmost section
			pSection = GetHeaderSection(m_nColResizeIndex, 0);
			if (pSection)  {
				rect = pSection->GetBoundsRect();
				rect.bottom = GetHeaderRect().bottom;
				InvalidateRect(rect);
			}

			m_columns[m_nColResizeIndex]->SetWidth(nWidth);

			//  And now we should repaint...
			UpdateWindow();
		}  else  {
			//  Paint the new line
			PaintResizeLine();
		}
	}  else  {
		//  Paint resize line at the first time
		if (m_nColResizeCount == 0 && !m_bColumnsFullDrag)
			PaintResizeLine();
		m_nColResizeCount ++;
	}
}



//  --------
void CVirtualGridCtrl::StopColResize(BOOL bAbort)
{
	int nWidth;
	BOOL bSuccess;

	if (!m_bColResizing)
		return;		//  Nothing to stop

	__try
	{
		//  Free our poor rodent
		bSuccess = ReleaseCapture();
		ASSERT(bSuccess);

		//  Is there at least one resize action
		if (m_nColResizeCount > 0)  {
			//  Paint over the line
			if (!m_bColumnsFullDrag)
				PaintResizeLine();

			if (bAbort)
				return;

			//  --------  Set column size  ------------
			//  New width...
			nWidth = m_nColResizePos - m_colResizeRect.left;

			//  Adjust the width according to borders
			if (nWidth < m_nColResizeMinWidth)
				nWidth = m_nColResizeMinWidth;
			if (nWidth > m_nColResizeMaxWidth)
				nWidth = m_nColResizeMaxWidth;

			//  User's event
			ColumnResize(m_nColResizeIndex, nWidth);

			//  Adjust the width again
			if (nWidth < m_nColResizeMinWidth)
				nWidth = m_nColResizeMinWidth;
			if (nWidth > m_nColResizeMaxWidth)
				nWidth = m_nColResizeMaxWidth;

			//  Set the width
			if (m_nColResizeIndex < m_columns.GetSize())
				m_columns[m_nColResizeIndex]->SetWidth(nWidth);

			m_pColResizeSection->SetWidth(nWidth);
		}
	}
	__finally
	{
		m_bColResizing = FALSE;
	}

	HeaderChange();
}



//  -----------------------------------------------------------------------
void CVirtualGridCtrl::Paint3DFrame(CDC *pDC, LPRECT lpRect, UINT nSideFlags)
{
	CRect rect(*lpRect);	//  !!!!!!!!
	BOOL bSuccess;

	bSuccess = pDC->DrawEdge(rect, BDR_RAISEDOUTER, nSideFlags & (~BF_TOPLEFT));
	ASSERT(bSuccess);

	if (nSideFlags & BF_BOTTOM)
		rect.bottom --;
	if (nSideFlags & BF_RIGHT)
		rect.right --;

	bSuccess = pDC->DrawEdge(rect, BDR_RAISEDINNER, nSideFlags & (~BF_BOTTOMRIGHT));
	ASSERT(bSuccess);

	if (nSideFlags & BF_TOP)
		rect.top ++;
	if (nSideFlags & BF_LEFT)
		rect.left ++;

	bSuccess = pDC->DrawEdge(rect, BDR_RAISEDINNER, nSideFlags & (~BF_TOPLEFT));
	ASSERT(bSuccess);

	if (nSideFlags & BF_BOTTOM)
		rect.bottom --;
	if (nSideFlags & BF_RIGHT)
		rect.right --;

	bSuccess = pDC->DrawEdge(rect, BDR_RAISEDOUTER, nSideFlags & (~BF_BOTTOMRIGHT));
	ASSERT(bSuccess);
}



//  ----------------------------------------------------------------------
void CVirtualGridCtrl::HideEdit()
{
	if (m_pEdit == NULL || !(::IsWindow(m_pEdit->m_hWnd)))
		return;		//  Nothing to hide

	m_editCell.m_nColumn = -1;
	m_editCell.m_nRow = -1;
	m_pEdit->Hide();
}



//  -----
int CVirtualGridCtrl::GetTextTopIndent()
{
	return m_nTextTopIndent;
}


//  ---
CGridHeaderSections * CVirtualGridCtrl::GetHeaderSections()
{
	return m_header.GetSections();
}


//  ---
CGridColumn * CVirtualGridCtrl::GetColumn(int nIndex)
{
	if (!m_columns.GetSize())
		return NULL;

	if (nIndex < 0 || nIndex > m_columns.GetUpperBound())
		return NULL;

	return m_columns[nIndex];
}



//  ---
COLORREF CVirtualGridCtrl::GetGridColor()
{
	return m_gridColor;
}


//  ---
BOOL CVirtualGridCtrl::GetCheckBoxes()
{
	return m_bCheckBoxes;
}


//  ---
int CVirtualGridCtrl::GetCheckHeight()
{
	return m_nCheckHeight;
}


//  ---
int CVirtualGridCtrl::GetCheckTopIndent()
{
	return m_nCheckTopIndent;
}


//  ---
BOOL CVirtualGridCtrl::GetGridLines()
{
	return m_bGridLines;
}



//  ---
UINT CVirtualGridCtrl::GetGridStyle()
{
	return m_nGridStyles;
}


//  ---
CImageList * CVirtualGridCtrl::GetImageList()
{
	return m_pImageList;
}



//  ---
int CVirtualGridCtrl::GetImageTopIndent()
{
	return m_nImageTopIndent;
}


//  ----
int CVirtualGridCtrl::GetGridLineWidth()
{
	return m_nGridLineWidth;
}




//  -----------------------------------------------------------------------------

//  ---------------  Some navigation methods  -----------------

CGridCell CVirtualGridCtrl::MoveLeft(const CGridCell& cell, int nOffset)
{
	CGridCell newCell;
	int i;

	i = max(cell.m_nColumn - nOffset, m_fixed.GetCount());

	while (i >= m_fixed.GetCount())  {
		newCell.m_nColumn = i;
		newCell.m_nRow = cell.m_nRow;

		//  Trying to set the cursor...
		if (IsCellAcceptCursor(newCell))
			return newCell;

		//  Previous column
		i --;
	}

	return cell;
}


//  ---
CGridCell CVirtualGridCtrl::MoveRight(const CGridCell& cell, int nOffset)
{
	CGridCell newCell;
	int i;

	//  New active column
	i = min(cell.m_nColumn + nOffset, m_columns.GetUpperBound());

	while (i <= m_columns.GetUpperBound())  {
		newCell.m_nColumn = i;
		newCell.m_nRow = cell.m_nRow;

		//  Trying to set the cursor...
		if (IsCellAcceptCursor(newCell))
			return newCell;

		//  Next column
		i ++;
	}

	return cell;
}



//  ---
CGridCell CVirtualGridCtrl::MoveUp(const CGridCell& cell, int nOffset)
{
	CGridCell newCell;
	int i;

	//  New active row
	i = max(cell.m_nRow - nOffset, 0);

	while (i >= 0)  {
		newCell.m_nColumn = cell.m_nColumn;
		newCell.m_nRow = i;

		//  Trying to set the cursor...
		if (IsCellAcceptCursor(newCell))
			return newCell;

		//  Previous row
		i --;
	}

	return cell;
}


//  ---
CGridCell CVirtualGridCtrl::MoveDown(const CGridCell& cell, int nOffset)
{
	CGridCell newCell;
	int j;

	//  New active row
	j = min(cell.m_nRow + nOffset, m_rows.GetCount() - 1);

	while (j <= m_rows.GetCount() - 1)  {
		newCell.m_nColumn = cell.m_nColumn;
		newCell.m_nRow = j;

		//  Trying to set the cursor...
		if (IsCellAcceptCursor(newCell))
			return newCell;

		j ++;
	}

	return cell;
}


//  ---
CGridCell CVirtualGridCtrl::MoveHome(CGridCell& cell)
{
	CGridCell newCell(cell);

	newCell.m_nColumn = m_fixed.m_nCount;
	newCell = MoveRight(newCell, 0);

	return newCell;
}


//  ---
CGridCell CVirtualGridCtrl::MoveEnd(CGridCell& cell)
{
	CGridCell newCell(cell);

	newCell.m_nColumn = m_columns.GetUpperBound();
	newCell = MoveLeft(newCell, 0);

	return newCell;
}


//  ---
CGridCell CVirtualGridCtrl::MoveGridHome(const CGridCell& cell)
{
	int i, j;
	CGridCell newCell;

	//  New active column
	i = m_fixed.GetCount();

	while (i <= cell.m_nColumn)  {
		//  New active row...
		j = 0;

		// --
		while (j <= cell.m_nRow)  {
			newCell.m_nColumn = i;
			newCell.m_nRow = j;

			//  Trying to set the cursor...
			if (IsCellAcceptCursor(newCell))
				return newCell;

			//  Next row
			j ++;
		}

		//  New column
		i ++;
	}

	return cell;
}


//  ---
CGridCell CVirtualGridCtrl::MoveGridEnd(const CGridCell& cell)
{
	int i, j;
	CGridCell newCell;

	//  New active cell
	i = m_columns.GetUpperBound();

	while (i >= cell.m_nColumn)  {
		j = m_rows.GetCount() - 1;

		//  ---
		while (j >= cell.m_nRow)  {
			newCell.m_nColumn = i;
			newCell.m_nRow = j;

			//  Trying to set the cursor
			if (IsCellAcceptCursor(newCell))
				return newCell;

			//  Previous row...
			j --;
		}

		//  Previous column
		i --;
	}

	return cell;
}



//  ---
CGridCell CVirtualGridCtrl::Select(const CGridCell& cell)
{
	CGridCell newCell;

	//  Is given cell accessible?
	if (IsCellAcceptCursor(cell))
		return cell;

	//  If selection is on the left then let's seek on the left
	if (cell.m_nColumn < m_cellFocused.m_nColumn)  {
		int i = max(cell.m_nColumn, m_fixed.GetCount());

		while (i <= m_cellFocused.m_nColumn)  {
			newCell.m_nColumn = i;
			newCell.m_nRow = cell.m_nRow;

			//  Trying to set the cursor
			if (IsCellAcceptCursor(newCell))
				break;
			//  The next column...
			i ++;
		}

		//  The cell is not found
		newCell = cell;

		if (IsCellAcceptCursor(newCell))
			return newCell;
	}

	//  If selection is on the right then let's seek on the right
	if (cell.m_nColumn > m_cellFocused.m_nColumn)  {
		int i = min(cell.m_nColumn, m_columns.GetUpperBound());

		while (i >= m_cellFocused.m_nColumn)  {
			newCell.m_nColumn = i;
			newCell.m_nRow = cell.m_nRow;

			//  Try to set the cursor...
			if (IsCellAcceptCursor(newCell))
				break;
			//  Previous column...
			i --;
		}
		//  The cell is not found
		newCell = cell;

		if (IsCellAcceptCursor(newCell))
			return newCell;
	}

	//  If selection is above then let's seek on the top
	if (cell.m_nRow < m_cellFocused.m_nRow)  {
		int j;
		CGridCell newCell;

		j = max(cell.m_nRow, 0);

		while (j <= m_cellFocused.m_nRow)  {
			newCell.m_nColumn = cell.m_nColumn;
			newCell.m_nRow = j;

			//  Try to set the cursor
			if (IsCellAcceptCursor(newCell))
				break;
			//  Next row
			j ++;
		}
		//  The cell is not found
		newCell = cell;

		if (IsCellAcceptCursor(newCell))
			return newCell;
	}

	//  If selection is below then let's seek at the bottom
	if (cell.m_nRow > m_cellFocused.m_nRow)  {
		int j;
		CGridCell newCell;

		j = min(cell.m_nRow, m_rows.GetCount() - 1);

		while (j >= m_cellFocused.m_nRow)  {
			newCell.m_nColumn = cell.m_nColumn;
			newCell.m_nRow = cell.m_nRow;

			//  Try to set the cursor
			if (IsCellAcceptCursor(newCell))
				break;
			//  Previous row
			j --;
		}
		//  The cell is not found
		newCell = cell;

		if (IsCellAcceptCursor(newCell))
			return newCell;
	}

	//  Nothing changes...
	return m_cellFocused;
}


//  ---
CGridCell CVirtualGridCtrl::First()
{
	CGridCell cell;
	int i, j;

	j = 0;

	while (j <= m_rows.GetCount() - 1)  {
		i = m_fixed.GetCount();

		while (i <= m_columns.GetUpperBound())  {
			cell.m_nColumn = i;
			cell.m_nRow = j;

			//  Trying to set the cursor...
			if (IsCellAcceptCursor(cell))
				return cell;

			//  Next column
			i ++;
		}

		//  Next row
		j ++;
	}

	//  Default result...
	return m_cellFocused;
}


//  ---
CGridCell CVirtualGridCtrl::Next(const CGridCell& cell)
{
	CGridCell newCell;
	int i, j;

	i = cell.m_nColumn + 1;
	j = cell.m_nRow;

	//  ---
	while (j <= m_rows.GetCount() - 1)  {
		while (i <= m_columns.GetUpperBound())  {
			newCell.m_nColumn = i;
			newCell.m_nRow = j;
			//  Trying to set the cursor taking into account line selection
			if (IsCellAcceptCursor(newCell) && (!m_bRowSelect || newCell.m_nRow != cell.m_nRow))
				return newCell;

			//  Next column...
			i ++;
		}

		//  Next row from the first column...
		i = m_fixed.GetCount();
		j ++;
	}

	//  Default result
	return m_cellFocused;
}


//  ---
CGridCell CVirtualGridCtrl::Prev(const CGridCell& cell)
{
	CGridCell newCell;
	int i, j;

	i = cell.m_nColumn - 1;
	j = cell.m_nRow;

	//  ---
	while (j >= 0)  {
		while (i >= m_fixed.GetCount())  {
			newCell.m_nColumn = i;
			newCell.m_nRow = j;

			//  Trying to set the cursor taking into account line selection
			if (IsCellAcceptCursor(newCell) && (!m_bRowSelect || newCell.m_nRow != cell.m_nRow))
				return newCell;

			//  Previous column...
			i --;
		}

		//  Previous row of the last column
		i = m_columns.GetUpperBound();
		j --;
	}

	//  Default result
	return cell;
}


//  ---
CGridCell CVirtualGridCtrl::GetEditCell()
{
	return m_editCell;
}


//  ---
int CVirtualGridCtrl::GetTextRightIndent()
{
	return m_nTextRightIndent;
}



//  ----
BOOL CVirtualGridCtrl::GetReadOnly()
{
	return m_bReadOnly;
}


//  -----------------------------------------------------------------
void CVirtualGridCtrl::SetRowsHeight(int nHeight)
{
	m_rows.SetHeight(nHeight);
}


//  ----------------------------------------------------------------
void CVirtualGridCtrl::SetRowCount(int nCount)
{
	m_rows.SetCount(nCount);
}


//  -----
void CVirtualGridCtrl::ValidateEdit()
{
	CRuntimeClass *pEditClass;

	pEditClass = GetEditClass(m_cellFocused);

	//  Create or change an edit control...
	if (m_pEdit == NULL || pEditClass != m_pEdit->GetRuntimeClass())  {
		if (m_pEdit)
			delete m_pEdit;
		m_pEdit = CreateEdit(pEditClass);
		m_pEdit->m_pGrid = this;
	}
}


//  ---
void CVirtualGridCtrl::DoUpdateEdit()
{
	BOOL bSuccess;

	if (!(::IsWindow(m_pEdit->m_hWnd)))  {
		bSuccess = m_pEdit->Create(
			WS_CHILD | ES_AUTOHSCROLL | ES_NOHIDESEL,
			GetCellRect(m_cellFocused),
			this,
			0
			);
		ASSERT(bSuccess);
		m_pEdit->SetFont(&m_font);
	}

	m_editCell = m_cellFocused;
	m_pEdit->UpdateContents();
	m_pEdit->UpdateStyle();
}



//  ---------------------------------------------------------------------------------------
int CVirtualGridCtrl::AddColumn(
				LPCTSTR lpCaption,
				int nWidth,
				int nAlignment
				)
{
	CGridColumn *pColumn;

	pColumn = m_columns.Add();

	pColumn->SetCaption(lpCaption);
	pColumn->SetWidth(nWidth);
	pColumn->SetAlignment(nAlignment);

	return m_columns.GetUpperBound();
}


//  -----------------------------------------------------------------------------------
CGridCell CVirtualGridCtrl::GetCellFocused()
{
	return m_cellFocused;
}


//  --------------------
CGridColumns * CVirtualGridCtrl::GetGridColumns()
{
	return &m_columns;
}




//  ----
void CVirtualGridCtrl::ShiftGridPoints(int nPoints, LPPOINT lpPoints, int dx, int dy)
{
	int i;

	//  First we should offset vertical lines in X direction...
	i = 0;
	while (i < m_fixed.GetCount() * (0 < (gsVertLine & m_nGridStyles)) * 2)  {
		lpPoints[i].x += dx;
		i ++;
	}

	//  Second we should offset horizontal lines in Y direction...
	while (i < nPoints)  {
		lpPoints[i].y += dy;
		i ++;
	}
}



//  ------------------------------
int CVirtualGridCtrl::GetFixedCount()
{
	return m_fixed.GetCount();
}


//  ----
void CVirtualGridCtrl::SetFixedCount(int nCount)
{
	m_fixed.SetCount(nCount);
}


//  ---
BOOL CVirtualGridCtrl::GetAllowSelect()
{
	return m_bAllowSelect;
}


//  -----
BOOL CVirtualGridCtrl::GetRowSelect()
{
	return m_bRowSelect;
}



//  -----  Used inside the PaintFixedGrid method  ----
void CVirtualGridCtrl::PaintVert3DLines(CDC *pDC, LPRECT lpRect, BOOL bDrawBottomLine)
{
	const UINT nFlags[] = {BF_TOPLEFT | BF_RIGHT, BF_RECT};
	int i, nWidth;
	CRect rect(*lpRect);

	rect.right = rect.left;

	//  Columns...
	for (i = 0; i < m_fixed.GetCount(); i ++)  {
		nWidth = m_columns[i]->GetWidth();
		if (nWidth > 0)  {
			rect.left = rect.right;
			rect.right += nWidth;

			if (pDC->RectVisible(rect))
				Paint3DFrame(pDC, rect, nFlags[bDrawBottomLine]);
		}
	}
}



//  ------  Used inside the PaintFixedGrid method  ------
void CVirtualGridCtrl::PaintBottom3DMargin(CDC *pDC, LPRECT lpRect)
{
	if (pDC->RectVisible(lpRect))
		Paint3DFrame(pDC, lpRect, BF_LEFT | BF_TOP | BF_RIGHT);
}


//  ------  Used inside the PaintFixedGrid method  ------
void CVirtualGridCtrl::PaintHorz3DLines(CDC *pDC, LPRECT lpRect)
{
	CRect rect(*lpRect);

	rect.bottom = rect.top;
	do  {
		rect.top = rect.bottom;
		rect.bottom += m_rows.m_nHeight;

		if (pDC->RectVisible(rect))
			Paint3DFrame(pDC, rect, BF_RECT);
	}  while (rect.bottom < lpRect->bottom);
}



//  ----
void CVirtualGridCtrl::SetRowHeight(int nHeight)
{
	m_rows.SetHeight(nHeight);
}



//  ----
int CVirtualGridCtrl::GetRowCount()
{
	return m_rows.GetCount();
}



//  ----
BOOL CVirtualGridCtrl::GetCancelOnExit()
{
	return m_bCancelOnExit;
}



//  ---
void CVirtualGridCtrl::SetCancelOnExit(BOOL bCancel)
{
	m_bCancelOnExit = bCancel;
}


//  ----
BOOL CVirtualGridCtrl::GetAllowEdit()
{
	return m_bAllowEdit;
}


//  ----
BOOL CVirtualGridCtrl::GetAlwaysEdit()
{
	return m_bAlwaysEdit;
}


//  ---
BOOL CVirtualGridCtrl::GetAlwaysSelected()
{
	return m_bAlwaysSelected;
}


//  ----
GridCheckStyle CVirtualGridCtrl::GetCheckStyle()
{
	return m_checkStyle;
}



//  ---
int CVirtualGridCtrl::GetFontHeight(CFont *pFont)
{
	CWnd *pWnd;
	int nHeight, nRetVal;

	pWnd = GetParent();
	ASSERT(pWnd);
	ASSERT(::IsWindow(pWnd->GetSafeHwnd()));

	CDC *pDC = pWnd->GetDC();
	nHeight = pDC->GetTextExtent("^j").cy;

	nRetVal = pWnd->ReleaseDC(pDC);
	ASSERT(nRetVal);
	
	return nHeight;
}



//  -----
BOOL CVirtualGridCtrl::GetDoubleBuffered()
{
	return m_bDoubleBuffered;
}


//  ----
void CVirtualGridCtrl::SetDoubleBuffered(BOOL bDoubleBuffered)
{
	if (m_bDoubleBuffered == bDoubleBuffered)
		return;		//  Nothing to change

	m_bDoubleBuffered = bDoubleBuffered;
	Invalidate();
}



//  =======================================================================================
//  =======================================================================================
BEGIN_MESSAGE_MAP(CVirtualGridCtrl, CWnd)
	//{{AFX_MSG_MAP(CVirtualGridCtrl)
	ON_WM_PAINT()
	ON_WM_GETDLGCODE()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_CHAR()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_NCHITTEST()
	ON_WM_SETCURSOR()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SYSKEYDOWN()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_MESSAGE(VGM_EXIT, OnExit)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CVirtualGridCtrl message handlers



//  --------------------------------------------------------------------------------------
void CVirtualGridCtrl::OnPaint()
{
	CPaintDC dc(this);	// device context for painting

	if (m_bDoubleBuffered)  {
		//  Use a memory DC to remove flicker.
		CMemDC memDC(&dc);
		Paint(memDC);
	}  else  {
		//  "Classical" painting
		Paint(&dc);
	}
}




//  ------------------------------------------------------------------------------
BOOL CVirtualGridCtrl::OnEraseBkgnd(CDC* pDC) 
{
	//  Erase!
	return TRUE;
}


//  ------------------------------------------------------------------------------
UINT CVirtualGridCtrl::OnGetDlgCode() 
{
	UINT nCode = DLGC_WANTARROWS;

	if (!m_bRowSelect)  {
		if (m_bAllowEdit)
			nCode |= DLGC_WANTCHARS;
	}

	return nCode;
}


//  --------------------------------------------------------------------
void CVirtualGridCtrl::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd::OnKillFocus(pNewWnd);

	if (m_rows.GetCount())  {
		InvalidateFocus();
		if (m_pEdit && pNewWnd != m_pEdit)
			HideCursor();
	}
}


//  --------------------------------------------------------------------
void CVirtualGridCtrl::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);

	if (m_rows.m_nCount > 0)  {
		InvalidateFocus();
		if (m_pEdit == NULL)	//  NYI
			ShowCursor();
	}
}


//  ----------------------------------------------------------------------
void CVirtualGridCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	BOOL bSuccess;
	CGridHeaderSection *pSection;
	CGridCell cell;

	m_titleTip.Hide();

	if (!AcquireFocus())  {
		bSuccess = ReleaseCapture();	//  ??????
		return;
	}

	//  If the header is visible and we hit it...
	if (m_bShowHeader && GetHeaderRect().PtInRect(point))  {
		//  Did we hit the resize area?
		pSection = GetResizeSectionAt(point);

		if (pSection)  {
			StartColResize(pSection, point.x, point.y);
		}  else  {
			pSection = GetSectionAt(point);

			if (pSection)  {
				//  Check 3D-effect
				if (m_header.m_bFlat)
					HeaderClick(pSection);
				else
					StartHeaderClick(pSection, point);
			}
		}

		return;
	}

	//  Check newly selected cell
	if ((gkMouse & m_nCursorKeys) && GetGridRect().PtInRect(point))  {
		cell = GetCellAt(point.x, point.y);

		m_clickPos.m_nColumn = -1;
		m_clickPos.m_nRow = -1;

		if (cell.IsEmpty())  {
			SetEditing(FALSE);
			SetCursor(m_cellFocused, FALSE, FALSE);
		}  else  {
			SetCursor(cell, TRUE, TRUE);
			CellClick(cell, nFlags, point.x, point.y);
		}

		if (GetCheckRect(cell).PtInRect(point))  {
			CheckClick(cell);
			return;
		}

		m_clickPos = cell;
	}

	CWnd::OnLButtonDown(nFlags, point);
}



//  -----------------------------------------------------------------------
void CVirtualGridCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	//  If column resizing take place...
	if (m_bColResizing)  {
		StopColResize(FALSE);
		return;
	}

	//  If header clicking take place...
	if (m_bHeaderClicking)  {
		StopHeaderClick(FALSE);
		return;
	}
	
	CWnd::OnLButtonUp(nFlags, point);
}



//  -----------------------------------------------------------------------
void CVirtualGridCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	BOOL bSuccess;
	CGridHeaderSection *pSection;

	//  Set the focus on itself
	if (!AcquireFocus())  {
		bSuccess = ReleaseCapture();
		return;
	}


	//  If the grid's header is visible and we hit it...
	if (m_bShowHeader && GetHeaderRect().PtInRect(point))  {
		//  If we hit header section border
		pSection = GetResizeSectionAt(point);
		if (pSection)  {
			int nIndex;
			int nWidth;

			//  Let's set the column width according to max text width
			nIndex = pSection->GetResizeColumnIndex();

			if (nIndex < m_columns.GetSize())  {
				nWidth = min(m_columns[nIndex]->GetMaxWidth(), GetColumnMaxWidth(nIndex));
				ColumnAutoSize(nIndex, nWidth);
				ColumnResize(nIndex, nWidth);
				m_bColResizing = TRUE;
				__try
				{
					m_columns[nIndex]->SetWidth(nWidth);
				}
				__finally
				{
					m_bColResizing = FALSE;
				}
			}
		}

		//  End header click handling
		return;
	}

	//  Check newly selected cell...
	if ((gkMouse & m_nCursorKeys) && (GetGridRect().PtInRect(point)))  {
		CGridCell cell, prevCell;

		cell = GetCellAt(point.x, point.y);
		prevCell = m_clickPos;
		m_clickPos.m_nColumn = -1;
		m_clickPos.m_nRow = -1;

		if (cell.IsEmpty())  {
			SetEditing(FALSE);
			SetCursor(m_cellFocused, FALSE, FALSE);
		}  else  {
			//  We hit the cell
			SetCursor(cell, TRUE, TRUE);
			CellClick(cell, nFlags, point.x, point.y);
		}

		//  Check flag hitting...
		if (GetCheckRect(cell).PtInRect(point))  {
			CheckClick(cell);
			return;
		}

		//  Check beginning of editing
		if (cell == m_cellFocused && m_bAllowEdit)  {
			SetEditing();
			if (GetEditing())
				return;
		}

		//  Don't forget last click's position
		m_clickPos = cell;
	}

	CWnd::OnLButtonDblClk(nFlags, point);
}


//  ----------------------------------------------------------------------
void CVirtualGridCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//  Show edit control if possible
	if (m_bAllowEdit && nChar >= 32 && nChar <= 255)  {
		ShowEditChar(nChar);
		return;
	}
	
	CWnd::OnChar(nChar, nRepCnt, nFlags);
}


//  ----------------------------------------------------------------------
void CVirtualGridCtrl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}


//  ----------------------------------------------------------------------------
void CVirtualGridCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	SCROLLINFO si;
	CRect rect;
	BOOL bSuccess;

	m_titleTip.Hide();

	GetClientRect(rect);
	rect.left = GetFixedRect().right;

	bSuccess = GetScrollInfo(SB_HORZ, &si);
	ASSERT(bSuccess);

	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_POS;


	switch (nSBCode)  {
	case SB_LINELEFT:
		si.nPos -= m_nLineStep;
		if (si.nPos < 0)
			si.nPos = 0;

		bSuccess = SetScrollInfo(SB_HORZ, &si);
		if (!bSuccess)
			break;
		InvalidateRect(rect);
		break;
	case SB_LINERIGHT:
		si.nPos += m_nLineStep;
		bSuccess = SetScrollInfo(SB_HORZ, &si);
		if (!bSuccess)
			break;
		InvalidateRect(rect);
		break;
	case SB_PAGELEFT:
		si.nPos -= m_nPageStep;
		if (si.nPos < 0)
			si.nPos = 0;
		bSuccess = SetScrollInfo(SB_HORZ, &si);
		if (!bSuccess)
			break;
		Invalidate();
		break;
	case SB_PAGERIGHT:
		si.nPos += m_nPageStep;
		bSuccess = SetScrollInfo(SB_HORZ, &si);
		if (!bSuccess)
			break;
		Invalidate();
		break;
	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		bSuccess = GetScrollInfo(SB_HORZ, &si, SIF_TRACKPOS);
		ASSERT(bSuccess);
		si.fMask = SIF_POS;
		si.nPos = si.nTrackPos;
		bSuccess = SetScrollInfo(SB_HORZ, &si);
		if (!bSuccess)
			break;
		Invalidate();
		break;
	default:
		break;
	}

	HorzScrollChange();

	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}



//  ------------------------------------------------------------------------------
void CVirtualGridCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	SCROLLINFO si;
	BOOL bSuccess;

	m_titleTip.Hide();

	bSuccess = GetScrollInfo(SB_VERT, &si);
	ASSERT(bSuccess);

	m_nLineStep = 1;	// m_rows.m_nHeight;

	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_POS;

	switch (nSBCode)  {
	case SB_LINEDOWN:
		si.nPos += m_nLineStep;
		bSuccess = SetScrollInfo(SB_VERT, &si);
		if (!bSuccess)
			break;

		Invalidate();
		break;
	case SB_LINEUP:
		if (si.nPos > 0)  {
			si.nPos -= m_nLineStep;
			bSuccess = SetScrollInfo(SB_VERT, &si);
			if (!bSuccess)
				break;

			Invalidate();
		}
		break;
	case SB_PAGEDOWN:
		si.nPos += m_nPageStep;
		bSuccess = SetScrollInfo(SB_VERT, &si);
		Invalidate();
		break;
	case SB_PAGEUP:
		if (si.nPos > 0)  {
			si.nPos -= m_nPageStep;
			bSuccess = SetScrollInfo(SB_VERT, &si);
			if (!bSuccess)
				break;
			Invalidate();
		}
		break;
	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		bSuccess = GetScrollInfo(SB_VERT, &si, SIF_TRACKPOS);
		si.fMask = SIF_POS;
		si.nPos = si.nTrackPos;
		bSuccess = SetScrollInfo(SB_VERT, &si);
		Invalidate();
		break;
	default:
		break;
	}

	VertScrollChange();

	m_nLineStep = 8;	//  !!!!
	
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}


//  ------------------------------------------------------------------------------
UINT CVirtualGridCtrl::OnNcHitTest(CPoint point) 
{
	m_hitTest = point;
	ScreenToClient(&m_hitTest);

	return CWnd::OnNcHitTest(point);
}



//  -----------------------------------------------------------------------------
BOOL CVirtualGridCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	HCURSOR hCursor;

	if (m_bColResizing)  {
		hCursor = ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
		return TRUE;
	}

	//  --
	if (nHitTest == HTCLIENT && m_bShowHeader)  {
		CRect rect(GetHeaderRect());

		if (rect.PtInRect(m_hitTest) && GetResizeSectionAt(m_hitTest))  {
			hCursor = ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
			return TRUE;
		}
	}

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}



//  ----------------------------------------------------------------------------
void CVirtualGridCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	//  Is there columns resizing happening?
	if (m_bColResizing)  {
		//  Just go on
		StepColResize(point.x, point.y);
		CWnd::OnMouseMove(nFlags, point);

		//  ...And nothing more
		return;
	}

	//  Is someone click at the header?
	if (m_bHeaderClicking)  {
		//  Go on
		StepHeaderClick(point);
		CWnd::OnMouseMove(nFlags, point);

		//  ...That's enough
		return;
	}

	//  Title Tips handling
	if (m_bTitleTips)  {
		CRect cellRect;
		CString strCellText;
		CGridHeaderSection *pSection;
		CGridCell cell;
		int nRetVal;

		pSection = GetSectionAt(point);
		cell = GetCellAt(point.x, point.y);

		//  The mouse is above header?
		if (pSection)  {
			strCellText = pSection->GetCaption();
			if (strCellText.GetLength())
				cellRect = pSection->GetBoundsRect();
		}  else if (!cell.IsEmpty())  {
			strCellText = GetCellText(cell);
			if (strCellText.GetLength())
				cellRect = GetCellRect(cell);
		}

		if (strCellText.GetLength())  {
			LOGFONT lf;

			nRetVal = m_font.GetLogFont(&lf);
			ASSERT(nRetVal);

			CRect hoverRect(cellRect);
			hoverRect.right += 2;

			cellRect.right -= m_nTextLeftIndent;

			TRACE0("Showing title tip\n");
			m_titleTip.Show(cellRect, strCellText, 0, hoverRect, &lf);
		}

		ASSERT(nRetVal);
	}

	CWnd::OnMouseMove(nFlags, point);
}



//  ------------------------------------------------------------------------------
void CVirtualGridCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	SCROLLINFO si;
	BOOL bSuccess;
	CGridCell cell;
	GridCursorOffset homeOffsets[] = {goHome, goGridHome};
	GridCursorOffset endOffsets[] = {goEnd, goGridEnd};
	GridCursorOffset tabOffsets[] = {goNext, goPrev};

	if (gkArrows & m_nCursorKeys)  {
		switch (nChar)  {
		case VK_LEFT:
			SetCursor(GetCursorCell(m_cellFocused, goLeft), TRUE, TRUE);
			bSuccess = GetScrollInfo(SB_HORZ, &si);
			if (m_bRowSelect && bSuccess)  {
				si.fMask = SIF_POS;
				si.nPos -= m_nLineStep;
				bSuccess = SetScrollInfo(SB_HORZ, &si);
			}
			break;
		case VK_RIGHT:
			SetCursor(GetCursorCell(m_cellFocused, goRight), TRUE, TRUE);
			bSuccess = GetScrollInfo(SB_HORZ, &si);
			if (m_bRowSelect && bSuccess)  {
				si.fMask = SIF_POS;
				si.nPos += m_nLineStep;
				bSuccess = SetScrollInfo(SB_HORZ, &si);
			}
			break;
		case VK_UP:
			if (!m_bAllowSelect)
				cell = m_visOrigin;
			else
				cell = m_cellFocused;
			//  Change selection
			SetCursor(GetCursorCell(cell, goUp), TRUE, TRUE);
			break;
		case VK_DOWN:
			//  If there's no focus then let's shift all the table...
			if (!m_bAllowSelect)  {
				cell.m_nColumn = m_visOrigin.m_nColumn;
				cell.m_nRow = m_visOrigin.m_nRow + m_visSize.m_nRow - 1;
				if (!IsCellVisible(cell, FALSE))
					cell.m_nRow --;
			}  else  {
				cell = m_cellFocused;
			}

			//  Change selection
			SetCursor(GetCursorCell(cell, goDown), TRUE, TRUE);
			break;
		case VK_PRIOR:
			SetCursor(GetCursorCell(m_cellFocused, goPageUp), TRUE, TRUE);
			break;
		case VK_NEXT:
			SetCursor(GetCursorCell(m_cellFocused, goPageDown), TRUE, TRUE);
			break;
		case VK_HOME:
			cell = GetCursorCell(m_cellFocused, homeOffsets[IS_CTRL_PRESSED() || m_bRowSelect]);
			SetCursor(cell, TRUE, TRUE);
			break;
		case VK_END:
			cell = GetCursorCell(m_cellFocused, endOffsets[IS_CTRL_PRESSED() || m_bRowSelect]);
			SetCursor(cell, TRUE, TRUE);
			break;
		case VK_TAB:
			if (gkTabs & m_nCursorKeys)  {
				SetCursor(GetCursorCell(m_cellFocused, goNext), TRUE, TRUE);	//  NYI
			}
			break;
		default:
			break;
		}
	}

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}



//  ------------------------------------------------------------------------
void CVirtualGridCtrl::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	m_titleTip.Hide();
	CWnd::OnSysKeyDown(nChar, nRepCnt, nFlags);
}


//  ------------------------------------------------------------------------
void CVirtualGridCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	m_titleTip.Hide();
	
	CWnd::OnRButtonDown(nFlags, point);
}



//  -------------------------  VGM_EXIT handler  -------------------------
LRESULT CVirtualGridCtrl::OnExit(WPARAM wParam, LPARAM lParam)
{
	ResetClickPos();

	//  Set the text and turn off the edit control
	if (m_bCancelOnExit)  {
		SetEditing(FALSE);
		InvalidateGrid();
	}

	return 0;
}



///////////////////////////////////////////////////////////////////////////////////
/////////////////////////////  CGridHeaderSection  ////////////////////////////////

//  =============================================================================
CGridHeaderSection::CGridHeaderSection(CGridHeaderSections *pParentSections)
{
	m_pParentSections = pParentSections;
	m_nWidth = 64;
	m_nAlignment = LVCFMT_LEFT;
	m_bWordWrap = FALSE;
	m_sections.m_pHeader = m_pParentSections->m_pHeader;
	m_sections.m_pOwnerSection = this;
}


//  ==============  CGridHeaderSection Copy Constructor  =================
CGridHeaderSection::CGridHeaderSection(const CGridHeaderSection &otherSection)
{
	m_boundsRect = otherSection.m_boundsRect;
	m_bWordWrap = otherSection.m_bWordWrap;
	m_nAlignment = otherSection.m_nAlignment;
	m_nColumnIndex = otherSection.m_nColumnIndex;
	m_nWidth = otherSection.m_nWidth;
	m_pParentSections = otherSection.m_pParentSections;
	m_sections = otherSection.m_sections;
	m_sections.m_pHeader = m_pParentSections->m_pHeader;
	m_strCaption = otherSection.m_strCaption;
}


//  ======================================================================
CGridHeaderSection::~CGridHeaderSection()
{
	int i;

	for (i = 0; i < m_sections.m_sections.GetSize(); i ++)
		delete m_sections.m_sections[i];

	m_sections.m_sections.RemoveAll();
}



//  ---------------------------------------------------------------------
CGridHeaderSection& CGridHeaderSection::operator =(CGridHeaderSection& otherSection)
{
	m_sections = otherSection.m_sections;
	m_strCaption = otherSection.m_strCaption;
	m_nWidth = otherSection.m_nWidth;
	m_nAlignment = otherSection.m_nAlignment;
	m_bWordWrap = otherSection.m_bWordWrap;

	return *this;
}


//  ----
BOOL CGridHeaderSection::IsSectionsStored()
{
	return m_sections.GetSize();
}


//  --
BOOL CGridHeaderSection::IsWidthStored()
{
	return (0 == m_sections.GetSize() && m_nWidth != 64);
}


//  --
BOOL CGridHeaderSection::GetAllowClick()
{
	int nIndex;

	//  Can we click at the column?
	if (GetHeader() && GetHeader()->GetGrid())  {
		nIndex = m_nColumnIndex;

		if (nIndex < GetHeader()->GetGrid()->GetColumns()->GetSize())
			return GetHeader()->GetGrid()->GetColumns()->GetAt(nIndex)->m_bAllowClick;
	}

	return FALSE;
}


//  --
CRect CGridHeaderSection::GetBoundsRect()
{
	CRect rect(0, 0, 0, 0);

	//  No header - no dimensions...
	if (NULL == GetHeader() || NULL == GetHeader()->GetGrid())
		return rect;

	//  Absolute borders
	rect = m_boundsRect;

	//  If the header is not FIXED then let's offset it...
	if (!GetFixedColumn())
		rect.OffsetRect(GetHeader()->GetGrid()->GetGridOrigin().x, 0);

	return rect;
}



//  --
int CGridHeaderSection::GetFirstColumnIndex()
{
	if (m_sections.GetSize())
		return m_sections.GetSection(0)->GetFirstColumnIndex();

	return m_nColumnIndex;
}



//  --
BOOL CGridHeaderSection::GetFixedColumn()
{
	if (m_sections.GetSize())
		return m_sections.GetSection(0)->GetFixedColumn();

	if (NULL == GetHeader() || 0 == GetHeader()->GetGrid())
		return FALSE;

	return m_nColumnIndex < GetHeader()->GetGrid()->GetFixed()->GetCount();
}



//  --
CGridHeader * CGridHeaderSection::GetHeader()
{
	if (GetParentSections())
		return GetParentSections()->m_pHeader;

	return NULL;
}



//  --
int CGridHeaderSection::GetLevel()
{
	if (GetParent())
		return GetParent()->GetLevel() + 1;

	return 0;
}


//  --
CGridHeaderSection * CGridHeaderSection::GetParent()
{
	if (GetParentSections())
		return GetParentSections()->m_pOwnerSection;

	return NULL;
}



//  --
CGridHeaderSections * CGridHeaderSection::GetParentSections()
{
	return m_pParentSections;
}


//  --
int CGridHeaderSection::GetResizeColumnIndex()
{
	int i;

	//  If there's subheaders return the column of last of them
	for (i = m_sections.GetUpperBound(); i >= 0; i --)  {
		if (m_sections[i]->GetVisible())
			return m_sections[i]->GetResizeColumnIndex();

	}

	return m_nColumnIndex;
}




//  --
CGridHeaderSections * CGridHeaderSection::GetSections()
{
	return &m_sections;
}



//  --
BOOL CGridHeaderSection::GetVisible()
{
	int i;

	//  If there's subheaders, let's see their visibility
	if (m_sections.GetSize())  {
		for (i = 0; i < m_sections.GetSize(); i ++)  {
			if (m_sections[i]->GetVisible())
				return TRUE;
		}
	}

	//  Otherwise let's see column's visibility
	if (GetHeader() && GetHeader()->GetGrid())  {
		if (m_nColumnIndex < GetHeader()->GetGrid()->GetColumns()->GetSize())
			return GetHeader()->GetGrid()->GetColumns()->GetColumn(m_nColumnIndex)->m_bVisible;
	}

	//  No column - the section is visible
	return TRUE;
}



//  --
int CGridHeaderSection::GetWidth()
{
	int nWidth;
	int i;
	CVirtualGridCtrl *pGrid;

	//  If there's subheaders then the width equals to the sum of
	//  the subheaders widths
	if (m_sections.GetSize())  {
		nWidth = 0;

		for (i = 0; i < m_sections.GetSize(); i ++)
			nWidth += m_sections[i]->GetWidth();

		return nWidth;
	}

	//  Otherwise return the width of appropriate column
	if (GetHeader())
		pGrid = GetHeader()->GetGrid();

	if (pGrid)  {
		if (m_nColumnIndex < pGrid->GetColumns()->GetSize())
			return pGrid->GetColumns()->GetColumn(m_nColumnIndex)->GetWidth();
	}

	//  No column - its own width
	return m_nWidth;
}


//  ----
void CGridHeaderSection::SetAlignment(int nAlignment)
{
	if (m_nAlignment == nAlignment)
		return;		//  Nothing to change

	m_nAlignment = nAlignment;
	Changed(FALSE);
}


//  ---
void CGridHeaderSection::SetCaption(LPCTSTR lpCaption)
{
	if (!m_strCaption.Compare(lpCaption))
		return;		//  Nothing to change

	m_strCaption = lpCaption;
	Changed(FALSE);
}


//  ---
void CGridHeaderSection::SetSections(const CGridHeaderSections& sections)
{
	m_sections = sections;
}


//  ---
void CGridHeaderSection::SetWidth(int nWidth)
{
	CVirtualGridCtrl *pGrid = NULL;

	if (nWidth < 0 || m_nWidth == nWidth)
		return;		//  Nothing to change

	if (GetHeader())
		pGrid = GetHeader()->GetGrid();

	if (pGrid)  {
		if (m_nColumnIndex > pGrid->GetColumns()->GetUpperBound())  {
			if (m_sections.GetSize())  {
				CGridHeaderSection *pSection;

				pSection = m_sections[m_sections.GetUpperBound()];
				pSection->SetWidth(pSection->m_nWidth + (nWidth - m_nWidth));

				return;
			}
		}
	}

	m_nWidth = nWidth;
	Changed(FALSE);
}



//  -------------------------------------------------------------------------
void CGridHeaderSection::SetWordWrap(BOOL bWordWrap)
{
	if (m_bWordWrap == bWordWrap)
		return;		//  Nothing to change
	
	m_bWordWrap = bWordWrap;
	Changed(FALSE);
}



//  -------------------------------------------------------------------------
int CGridHeaderSection::GetAlignment()
{
	return m_nAlignment;
}


//  ------------------------------------------------------------------------
BOOL CGridHeaderSection::GetWordWrap()
{
	return m_bWordWrap;
}



//  ----
int CGridHeaderSection::GetColumnIndex()
{
	return m_nColumnIndex;
}


/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////  CGridHeaderSections  ///////////////////////////////


//  ==================================================================================
CGridHeaderSections::CGridHeaderSections()
{
	m_pHeader = NULL;
	m_pOwnerSection = NULL;
}


//  ==================================================================================
CGridHeaderSections::~CGridHeaderSections()
{
	//  Thank you, mr. bluescreen
	int i = m_sections.GetSize();
	for (; i; i--)
		delete m_sections[i - 1];
	m_sections.RemoveAll();
}


//  --
CGridHeaderSection * CGridHeaderSections::Add()
{
	int nIndex;

	nIndex = m_sections.Add(new CGridHeaderSection(this));

	if (m_pHeader && m_pHeader->GetGrid())
		Update(m_sections[nIndex]);

	return m_sections[nIndex];
}


//  --
int CGridHeaderSections::GetMaxColumn()
{
	if (m_sections.GetSize())
		return m_sections[m_sections.GetUpperBound()]->m_nColumnIndex;

	return 0;
}



//  --
int CGridHeaderSections::GetMaxLevel()
{
	int nMaxLevel = 0;
	int i;
	CGridHeaderSection *pSection;

	for (i = 0; i < m_sections.GetSize(); i ++)  {
		pSection = m_sections.GetAt(i);
		if (nMaxLevel < pSection->GetLevel())
			nMaxLevel = pSection->GetLevel();

		nMaxLevel = max(nMaxLevel, pSection->m_sections.GetMaxLevel());
	}

	return nMaxLevel;
}


//  --
CGridHeaderSection * CGridHeaderSections::GetSection(int nIndex)
{
	return m_sections.GetAt(nIndex);
}



//  --
CGridHeaderSection * CGridHeaderSections::GetSection(int nColumnIndex, int nLevel)
{
	int i, l;
	CGridHeaderSection *pSection;

	for (i = 0; i < m_sections.GetSize(); i ++)  {
		pSection = m_sections[i];

		l = pSection->GetLevel();
		
		//  Compare the column and the level
		if ((pSection->m_nColumnIndex >= nColumnIndex)
			&& ((nLevel == -1) && (0 == pSection->m_sections.GetSize())
			|| (l == nLevel)))

			//  We have found it!!!
			return pSection;

		//  Recurse...
		pSection = pSection->m_sections.GetSection(nColumnIndex, nLevel);
		if (pSection)
			return pSection;
	}

	//  No such a section
	return NULL;
}



//  ---
void CGridHeaderSections::SetSection(int nIndex, CGridHeaderSection *pSection)
{
	CVirtualGridCtrl *pGrid;

	pGrid = m_pHeader->GetGrid();

	
	delete m_sections[nIndex];
	m_sections.RemoveAt(nIndex);

	m_sections[nIndex] = pSection;
}


//  --
CGridHeader * CGridHeaderSections::GetOwner()
{
	return m_pHeader;
}


//  ---
void CGridHeaderSections::Update(CGridHeaderSection *pSection)
{
	if (m_pHeader)
		m_pHeader->Change();
}





//  ----
BOOL CGridHeaderSections::FindResizeSection(CPoint point, CGridHeaderSection *& pRetrievedSection)
{
	int i, nColumn;
	int dl, dr;
	CRect rect;
	CGridHeaderSection *pSection;
	CGridColumns *pColumns;

	pColumns = m_pHeader->GetGrid()->GetColumns();

	for (i = m_sections.GetUpperBound(); i >= 0; i --)  {
		//  Get the cell and its column
		pSection = m_sections[i];

		//  We should search only for visible columns
		if (!pSection->GetVisible())
			continue;

		nColumn = pSection->GetResizeColumnIndex();

		//  Get the rectangle of size changing area  ???
		rect = pSection->GetBoundsRect();

		//  Find out hit errors
		dl = 7;
		if (rect.Width() < 20)
			dl = 3;
		if (rect.Width() < 10)
			dl = 1;

		dr = 5;

		if (nColumn < m_pHeader->GetGrid()->GetColumns()->GetUpperBound())  {
			if (pColumns->GetColumn(nColumn + 1)->m_nWidth < 20)
				dr = 3;
			if (pColumns->GetColumn(nColumn + 1)->m_nWidth < 10)
				dr = 1;
		}

		//  Adjust target rect
		if (rect.Width() > 0)
			rect.left = rect.right - dl;
		rect.right += dr;

		//  We have got in the target?
		if (rect.PtInRect(point))  {
			//  Check the column on the fixed size
			if (nColumn < pColumns->GetSize() && (pColumns->GetColumn(nColumn)->m_bFixedSize
				|| !m_pHeader->GetGrid()->ColumnsSizing()))  {

				pRetrievedSection = NULL;
				return FALSE;
			}  else  {
				pRetrievedSection = pSection;
				return TRUE;
			}
		}

		//  Search for the section in subheadings
		if (pSection->GetSections()->FindResizeSection(point, pRetrievedSection))
			return TRUE;
	}

	//  Nothing have found
	return FALSE;
}




//  
BOOL CGridHeaderSections::FindSection(CPoint point, CGridHeaderSection *&pRetrievedSection)
{
	int i;
	CGridHeaderSection *pSection;
	CRect rect;

	for (i = 0; i < m_sections.GetSize(); i ++)  {
		pSection = m_sections[i];

		if (pSection->GetVisible())  {
			if (pSection->GetBoundsRect().PtInRect(point))  {
				pRetrievedSection = pSection;
				return TRUE;
			}
		}

		if (pSection->m_sections.FindSection(point, pRetrievedSection))
			return TRUE;
	}

	pRetrievedSection = NULL;
	return FALSE;
}



//  ---
void CGridHeaderSections::UpdateColumnIndex(int &nColumn)
{
	int i;
	CGridHeaderSection *pSection;

	for (i = 0; i < m_sections.GetSize(); i ++)  {
		pSection = m_sections[i];

		//  Is there subheaders?
		if (0 == pSection->m_sections.GetSize())  {
			//  This is the lowermost section
			pSection->m_nColumnIndex = nColumn;
			nColumn ++;
		}  else  {
			//  Recurse...
			pSection->m_sections.UpdateColumnIndex(nColumn);
			//  The index is the last's index
			pSection->m_nColumnIndex = pSection->m_sections.GetSection(pSection->m_sections.GetUpperBound())->m_nColumnIndex;
		}
	}
}



//  ---
void CGridHeaderSections::UpdateSectionsBounds(LPRECT lpRect)
{
	int i;
	CGridHeaderSection *pSection;
	CRect rect(*lpRect);
	CRect sectionRect;

	rect.right = rect.left;

	//  Let's sort subheaders
	for (i = 0; i < m_sections.GetSize(); i ++)  {
		pSection = m_sections[i];

		rect.left = rect.right;
		rect.right = rect.left + pSection->GetWidth();

		//  Rectangle
		sectionRect = rect;

		if (pSection->m_sections.GetSize())
			sectionRect.bottom = rect.top + m_pHeader->m_nSectionHeight;
		//  Store...
		pSection->m_boundsRect = sectionRect;

		//  Subheaders
		if (pSection->m_sections.GetSize())  {
			//  Substact upper-level line
			sectionRect.top = sectionRect.bottom;
			sectionRect.bottom = rect.bottom;
			
			//  Bottom-level subheaders
			pSection->m_sections.UpdateSectionsBounds(sectionRect);
		}
	}
}




/////////////////////////////////////////////////////////////////////////////////
////////////////////////////////  CGridHeader  //////////////////////////////////


//  =============================================================================
CGridHeader::CGridHeader(CVirtualGridCtrl *pGrid)
{
	m_pGrid = pGrid;
	m_color = ::GetSysColor(COLOR_BTNFACE);
	m_sections.m_pHeader = this;
	m_sections.m_pOwnerSection = NULL;
	m_nSectionHeight = 17;
	m_bSynchronized = TRUE;
	m_bAutoSynchronize = TRUE;
	m_bFullSynchronizing = FALSE;
	m_bGridFont = TRUE;
	m_bFlat = TRUE;
	m_bGridColor = FALSE;
	m_bAutoHeight = FALSE;
	m_pImageList = NULL;
}


//  =============================================================================
CGridHeader::~CGridHeader()
{
}



//  ---
void CGridHeader::SynchronizeSections()
{
	int nColumn;

	//  We should refresh sections's inner parameters
	UpdateSections();

	//  Let's synchronize...
	if (m_pGrid == NULL)
		return;		//  Nothing to synchronize


	if (0 == m_sections.GetSize())  {
		CRect rect;
		CGridHeaderSection *pSection;
		CGridColumn *pColumn;

		//  Absolute header borders...
		m_pGrid->GetClientRect(rect);
		rect.left = m_pGrid->GetColumnRect(0).left;
		rect.right = rect.left;
		rect.bottom = rect.top + GetHeight();


		//  Let's add
		nColumn = 0;
		while (nColumn < m_pGrid->GetColumns()->GetSize())  {
			pColumn = m_pGrid->GetColumn(nColumn);

			//  Section rectangle
			rect.left = rect.right;
			rect.right = rect.left + pColumn->GetWidth();

			//  Add the section
			pSection = m_sections.Add();
			
			pSection->m_nColumnIndex = nColumn;
			pSection->m_boundsRect = rect;
			pSection->m_nWidth = pColumn->GetWidth();
			pSection->m_strCaption = pColumn->GetCaption();
			pSection->m_nAlignment = pColumn->GetAlignment();

			//  The next column
			nColumn ++;
		}

		return;
	}

	nColumn = m_sections.GetSection(m_sections.GetUpperBound())->m_nColumnIndex;

	if (nColumn < m_pGrid->GetColumns()->GetUpperBound())  {
		nColumn ++;	//  !!!!
		CRect rect;
		CGridHeaderSection *pSection;
		CGridColumn *pColumn;

		//  Absolute header borders...
		m_pGrid->GetClientRect(rect);
		rect.left = m_pGrid->GetColumnRect(nColumn).left;
		rect.right = rect.left;
		rect.bottom = rect.top + GetHeight();

		//  Let's add
		while (nColumn < m_pGrid->GetColumns()->GetSize())  {
			pColumn = m_pGrid->GetColumns()->GetColumn(nColumn);

			//  Section rectangle
			rect.left = rect.right;
			rect.right = rect.left + pColumn->GetWidth();

			//  Add the section
			pSection = m_sections.Add();
			
			pSection->m_nColumnIndex = nColumn;
			pSection->m_boundsRect = rect;
			pSection->m_nWidth = pColumn->GetWidth();

			//  The next column
			nColumn ++;
		}

		nColumn --;
	}  else if (nColumn > m_pGrid->GetColumns()->GetUpperBound())  {
		m_sections.DeleteSections();
	}

	//  Lowermost section must be synchronized by headline, justification and width
	m_sections.Synchronize(nColumn);
}




//  --
void CGridHeader::UpdateSections()
{
	CRect rect;
	int nColumn = 0;

	if (m_pGrid == NULL)
		return;		//  Nothing to update

	//  Calculate column indices
	m_sections.UpdateColumnIndex(nColumn);

	//  Header's absolute borders
	m_pGrid->GetClientRect(rect);
	rect.right = rect.left + m_pGrid->GetColumnsWidth(0, m_pGrid->GetColumns()->GetUpperBound());
	rect.bottom = rect.top + GetHeight();

	//  Calculate section's borders
	m_sections.UpdateSectionsBounds(rect);
}




//  --
BOOL CGridHeader::IsColorStored()
{
	return !m_bGridColor;
}


//  --
BOOL CGridHeader::IsFontStored()
{
	return !m_bGridFont;
}


//  --
BOOL CGridHeader::IsSectionsStored()
{
	return !(0 == GetMaxLevel() && m_bFullSynchronizing && m_bSynchronized);
}



//  --
int CGridHeader::GetHeight()
{
	return (GetMaxLevel() + 1) * m_nSectionHeight;
}


//  --
int CGridHeader::GetMaxColumn()
{
	return m_sections.GetMaxColumn();
}


//  --
int CGridHeader::GetMaxLevel()
{
	return m_sections.GetMaxLevel();
}


//  --
int CGridHeader::GetWidth()
{
	int nWidth = 0;
	int i;

	for (i = 0; i < m_sections.GetSize(); i ++)
		nWidth += m_sections.GetSection(i)->GetWidth();

	return nWidth;
}


//  ---
void CGridHeader::SetAutoHeight(BOOL bAutoHeight)
{
	if (m_bAutoHeight == bAutoHeight)
		return;		//  Nothing to change

	m_bAutoHeight = bAutoHeight;
	if (bAutoHeight)
		SetSectionHeight(m_nSectionHeight);
}


//  ---
void CGridHeader::SetAutoSynchronize(BOOL bAutoSynchronize)
{
	if (m_bAutoSynchronize == bAutoSynchronize)
		return;		//  Nothing to change

	m_bAutoSynchronize = bAutoSynchronize;
	if (bAutoSynchronize)
		SetSynchronized();
}


//  ---
void CGridHeader::SetColor(COLORREF color)
{
	if (m_color == color)
		return;		//  Nothing to change

	m_color = color;
	m_bGridColor = FALSE;
	Change();
}


//  ---
void CGridHeader::SetFlat(BOOL bFlat)
{
	if (m_bFlat == bFlat)
		return;		//  Nothing to change

	m_bFlat = bFlat;
	if (bFlat && m_pGrid)
		m_pGrid->GetFixed()->SetFlat();

	SetSectionHeight(m_nSectionHeight);
	Change();
}


//  --
void CGridHeader::SetFont(CFont *pFont)
{
	m_pFont = pFont;
	Change();	//  ???
}


//  ---
void CGridHeader::SetFullSynchronizing(BOOL bFullSynchronizing)
{
	if (m_bFullSynchronizing == bFullSynchronizing)
		return;		//  Nothing to change

	m_bFullSynchronizing = bFullSynchronizing;
	if (bFullSynchronizing)
		SetSynchronized(FALSE);
}


//  ---
void CGridHeader::SetGridColor(BOOL bGridColor)
{
	if (m_bGridColor == bGridColor)
		return;		//  Nothing to change

	m_bGridColor = bGridColor;
	if (m_pGrid)
		GridColorChanged(m_pGrid->GetGridColor());
	SetSectionHeight(m_nSectionHeight);
	Change();
}


//  ---
void CGridHeader::SetGridFont(BOOL bGridFont)
{
	if (m_bGridColor == bGridFont)
		return;		//  Nothing to change
	
	m_bGridFont = bGridFont;
	if (m_pGrid)
		GridFontChanged(m_pGrid->GetFont());
	Change();
}


//  ---
void CGridHeader::SetImageList(CImageList *pImageList)
{
	if (m_pImageList == pImageList)
		return;		//  Nothing to change

	m_pImageList = pImageList;

	//  Adjust height and (probably) other changes
	SetSectionHeight(m_nSectionHeight);
	Change();
}


//  -------------------------------------------------------------------
void CGridHeader::SetSections(const CGridHeaderSections& sections)
{
	m_sections = sections;
	SetSynchronized(FALSE);
}


//  ------------------------------------------------------------------
void CGridHeader::SetSectionHeight(int nHeight)
{
	int nTextHeight, nImageHeight;

	if (m_bAutoHeight)  {
		nTextHeight = m_pGrid->GetFontHeight(m_pFont) + 4;	//  NYI
		nImageHeight = 0;

		if (m_pImageList)  {
			IMAGEINFO ii;
			BOOL bSuccess;

			bSuccess = m_pImageList->GetImageInfo(0, &ii);
			ASSERT(bSuccess);

			nImageHeight = ii.rcImage.bottom - ii.rcImage.top + 2;
			if (!m_bGridColor)
				nImageHeight ++;
			if (!m_bFlat)
				nImageHeight ++;
		}

		nHeight = max(nTextHeight, nImageHeight);
	}

	if (nHeight < 0)
		nHeight = 0;
	
	if (m_nSectionHeight != nHeight)  {
		m_nSectionHeight = nHeight;
		Change();
	}
}



//  -------------------------------------------------------------
void CGridHeader::SetSynchronized(BOOL bSynchronized)
{
	if (m_bSynchronized == bSynchronized)
		return;		//  Nothing to change

	m_bSynchronized = bSynchronized;
	if ((bSynchronized || m_bAutoSynchronize) && (m_pGrid != NULL))  {
		m_bSynchronized = TRUE;
		SynchronizeSections();
	}
}


//  ---
void CGridHeader::Change()
{
	UpdateSections();
	if (m_pGrid)
		m_pGrid->HeaderChange();
}


//  ---
void CGridHeader::GridColorChanged(COLORREF newColor)
{

}


//  ---
void CGridHeader::GridFontChanged(CFont *pNewFont)
{

}



///////////////////////////////////////////////////////////////////////////////
////////////////////////////  CGridColumn  ////////////////////////////////////
IMPLEMENT_DYNCREATE(CGridColumn, CObject);


//  =====================  Default Constructor  =====================
CGridColumn::CGridColumn(CGridColumns *pOwnerColumns)
{
	m_pColumns = pOwnerColumns;
	m_nWidth = 64;
	m_nMinWidth = 0;
	m_nMaxWidth = 10000;
	m_nAlignment = LVCFMT_LEFT;
	m_bTabStop = TRUE;
	m_bVisible = TRUE;
	m_bAllowClick = TRUE;
	m_bFixedSize = FALSE;		//  !!!

	m_bWantReturns = FALSE;
	m_bWordWrap = FALSE;
	m_bReadOnly = FALSE;
	m_nMaxLength = 0;
	m_editStyle = geSimple;
	m_checkKind = gcNone;
	m_nTag = 0;
	m_bAlignEdit = FALSE;
}


//  ================================  Copy Constructor  =======================
CGridColumn::CGridColumn(const CGridColumn &otherColumn)
{
	m_bAlignEdit = otherColumn.m_bAlignEdit;
	m_bAllowClick = otherColumn.m_bAllowClick;
	m_bFixedSize = otherColumn.m_bFixedSize;
	m_bReadOnly = otherColumn.m_bReadOnly;
	m_bTabStop = otherColumn.m_bTabStop;
	m_bVisible = otherColumn.m_bVisible;
	m_bWantReturns = otherColumn.m_bWantReturns;
	m_bWordWrap = otherColumn.m_bWordWrap;
	m_checkKind = otherColumn.m_checkKind;
	m_editStyle = otherColumn.m_editStyle;
	m_nAlignment = otherColumn.m_nAlignment;
	m_nMaxWidth = otherColumn.m_nMaxWidth;
	m_nMinWidth = otherColumn.m_nMinWidth;
	m_nTag = otherColumn.m_nTag;
	m_nWidth = otherColumn.m_nWidth;
	m_pColumns = otherColumn.m_pColumns;

	m_pickList.RemoveAll();		//  ???
	m_pickList.Append(otherColumn.m_pickList);

	m_strCaption = otherColumn.m_strCaption;
	m_strEditMask = otherColumn.m_strEditMask;
}



//  ===========================================================================
CGridColumn::~CGridColumn()
{
}





//  ---
CGridColumn& CGridColumn::operator =(const CGridColumn& sourceColumn)
{
	int nRetVal;

	m_strCaption = sourceColumn.m_strCaption;
	m_nWidth = sourceColumn.m_nWidth;
	m_nMinWidth = sourceColumn.m_nMinWidth;
	m_nMaxWidth = sourceColumn.m_nMaxWidth;
	m_bFixedSize = sourceColumn.m_bFixedSize;
	m_nMaxLength = sourceColumn.m_nMaxLength;
	m_nAlignment = sourceColumn.m_nAlignment;
	m_bReadOnly = sourceColumn.m_bReadOnly;
	m_editStyle = sourceColumn.m_editStyle;
	m_strEditMask = sourceColumn.m_strEditMask;
	m_checkKind = sourceColumn.m_checkKind;
	m_bWantReturns = sourceColumn.m_bWantReturns;
	m_bWordWrap = sourceColumn.m_bWordWrap;
	m_bTabStop = sourceColumn.m_bTabStop;
	m_bVisible = sourceColumn.m_bVisible;

	m_pickList.RemoveAll();
	nRetVal = m_pickList.Append(sourceColumn.m_pickList);

	m_nTag = sourceColumn.m_nTag;
	m_bAllowClick = sourceColumn.m_bAllowClick;

	return *this;
}



//  --
int CGridColumn::GetEditAlignment()
{
	if (m_bAlignEdit)
		return m_nAlignment;

	return LVCFMT_LEFT;
}


//  --
CVirtualGridCtrl * CGridColumn::GetGrid()
{
	if (m_pColumns)
		return m_pColumns->m_pGrid;
	
	return NULL;
}


//  --
void CGridColumn::GetPickList(CStringArray& pickList)
{
	int nRetVal;

	pickList.RemoveAll();

	nRetVal = pickList.Append(m_pickList);
}


//  --
int CGridColumn::GetPickListCount()
{
	return m_pickList.GetSize();
}


//  --
CGridHeaderSection * CGridColumn::GetTitle()
{
	if (NULL == GetGrid())
		return NULL;

	return GetGrid()->GetHeaderSection(GetIndex(), -1);
}


//  --
int CGridColumn::GetWidth()
{
	if (!m_bVisible)
		return 0;

	return m_nWidth;
}


//  --
BOOL CGridColumn::IsPickListStored()
{
	return m_pickList.GetSize();
}


//  --
void CGridColumn::SetAlignEdit(BOOL bAlignEdit)
{
	if (m_bAlignEdit == bAlignEdit)
		return;		//  Nothing to change

	m_bAlignEdit = bAlignEdit;
	Changed(FALSE);
}


//  --
void CGridColumn::SetCheckKind(GridCheckKind checkKind)
{
	if (m_checkKind == checkKind)
		return;		//  Nothing to change

	m_checkKind = checkKind;
	Changed(FALSE);
}


//  --
void CGridColumn::SetMaxWidth(int nMaxWidth)
{
	if (nMaxWidth < m_nMinWidth)
		nMaxWidth = m_nMinWidth;
	if (nMaxWidth > 10000)
		nMaxWidth = 10000;

	m_nMaxWidth = nMaxWidth;
	SetWidth(m_nWidth);
}


//  ---
void CGridColumn::SetMinWidth(int nMinWidth)
{
	if (nMinWidth < 0)
		nMinWidth = 0;
	if (nMinWidth > m_nMaxWidth)
		nMinWidth = m_nMaxWidth;
	m_nMinWidth = nMinWidth;
	SetWidth(m_nWidth);
}


//  --
void CGridColumn::SetPickList(const CStringArray &pickList)
{
	int nRetVal;

	m_pickList.RemoveAll();
	nRetVal = m_pickList.Append(pickList);
}



//  --
void CGridColumn::SetTabStop(BOOL bTabStop)
{
	if (m_bTabStop == bTabStop)
		return;		//  Nothing to change
	m_bTabStop = bTabStop;
	Changed(FALSE);
}


//  --
void CGridColumn::SetWantReturns(BOOL bWantReturns)
{
	if (m_bWantReturns == bWantReturns)
		return;		//  Nothing to change
	m_bWantReturns = bWantReturns;
	Changed(FALSE);
}


//  --
void CGridColumn::SetWordWrap(BOOL bWordWrap)
{
	if (m_bWordWrap == bWordWrap)
		return;		//  Nothing to change
	m_bWordWrap = bWordWrap;
	Changed(FALSE);
}


//  --
void CGridColumn::SetAlignment(int nAlignment)
{
	if (m_nAlignment == nAlignment)
		return;		//  Nothing to change
	m_nAlignment = nAlignment;
	Changed(FALSE);
}


//  ---
void CGridColumn::SetCaption(LPCTSTR lpCaption)
{
	if (!m_strCaption.Compare(lpCaption))
		return;		//  The caption remains the same

	m_strCaption = lpCaption;
	Changed(FALSE);
}


//  --
void CGridColumn::SetEditMask(LPCTSTR lpEditMask)
{
	if (!m_strEditMask.Compare(lpEditMask))
		return;		//  Nothing to change
	m_strEditMask = lpEditMask;
	Changed(FALSE);
}


//  --
void CGridColumn::SetEditStyle(GridEditStyle style)
{
	if (m_editStyle == style)
		return;		//  Nothing to change
	m_editStyle = style;
	Changed(FALSE);
}


//  --
void CGridColumn::SetMaxLength(int nMaxLength)
{
	if (m_nMaxLength == nMaxLength)
		return;		//  Nothing to change
	m_nMaxLength = nMaxLength;
	Changed(FALSE);
}


//  --
void CGridColumn::SetReadOnly(BOOL bReadOnly)
{
	if (m_bReadOnly = bReadOnly)
		return;		//  Nothing to change
	m_bReadOnly = bReadOnly;
	Changed(FALSE);
}


//  --
void CGridColumn::SetVisible(BOOL bVisible)
{
	if (m_bVisible == bVisible)
		return;		//  Nothing to change
	m_bVisible = bVisible;
	Changed();
}



//  --
void CGridColumn::SetWidth(int nWidth)
{
	if (m_nWidth == nWidth)
		return;		//  Nothing to change

	if (nWidth < m_nMinWidth)
		nWidth = m_nMinWidth;
	if (nWidth > m_nMaxWidth)
		nWidth = m_nMaxWidth;

	m_nWidth = nWidth;
	Changed();
}





//  --------------------------------------------------------
BOOL CGridColumn::GetAllowClick()
{
	return m_bAllowClick;
}


//  ------------------------------------------------------
void CGridColumn::SetAllowClick(BOOL bAllowClick)
{
	m_bAllowClick = bAllowClick;
}


//  -------------------------------------------------------
int CGridColumn::GetMaxWidth()
{
	return m_nMaxWidth;
}


//  ------------------------------------------------------
int CGridColumn::GetMinWidth()
{
	return m_nMinWidth;
}


//  -----------------------------------------------------
GridEditStyle CGridColumn::GetEditStyle()
{
	return m_editStyle;
}






/////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////  CGridColumns  /////////////////////////////////////////


//  ====================================================================================
CGridColumns::CGridColumns()
{
	m_pGrid = NULL;
}



//  ======================  Copy constructor  ============================
CGridColumns::CGridColumns(const CGridColumns& otherColumns)
{
	int i;

	for (i = m_columns.GetUpperBound(); i >= 0; i --)
		delete m_columns[i];

	m_columns.RemoveAll();


	for (i = 0; i < otherColumns.m_columns.GetSize(); i ++)  {
		m_columns.Add(new CGridColumn(* otherColumns.m_columns[i]));
	}
}


//  ===========================================================================
CGridColumns::~CGridColumns()
{
	int i;

	for (i = m_columns.GetUpperBound(); i >= 0; i --)
		delete m_columns[i];
}


//  ---
CGridColumns& CGridColumns::operator =(const CGridColumns& otherColumns)
{
	int i;
	int nRetVal;
	CGridColumn *pColumn;

	for (i = 0; i < m_columns.GetSize(); i ++)  {
		delete m_columns.GetAt(i);
	}

	m_columns.RemoveAll();

//	if (m_pGrid)
//		m_pGrid->ColumnsChange(this);

	for (i = 0; i < otherColumns.m_columns.GetSize(); i ++)  {
		pColumn = new CGridColumn(*otherColumns.m_columns[i]);
		pColumn->m_pColumns = this;
		nRetVal = m_columns.Add(pColumn);
	}

	if (m_pGrid)
		m_pGrid->ColumnsChange();

	return *this;
}


//  --
CGridColumn * CGridColumns::GetAt(int nIndex)
{
	return m_columns.GetAt(nIndex);
}


//  --
int CGridColumns::GetSize()
{
	return m_columns.GetSize();
}


//  --
int CGridColumns::GetUpperBound()
{
	return m_columns.GetUpperBound();
}

//  --
CGridColumn * CGridColumns::Add()
{
	int nIndex;

	if (m_pGrid == NULL)  {
		nIndex = m_columns.Add(new CGridColumn(this));
		return m_columns.GetAt(nIndex);
	}

	nIndex = m_columns.Add(new CGridColumn(this));
	m_pGrid->ColumnsChange();
	return m_columns.GetAt(nIndex);
}




//  ---
void CGridColumns::RemoveAt(int nIndex)
{
	if (nIndex < 0 || nIndex > m_columns.GetUpperBound())
		return;		//  Nothing to remove

	delete m_columns[nIndex];
	m_columns.RemoveAt(nIndex);

	if (m_pGrid)
		m_pGrid->ColumnsChange();
}


//  --
CGridColumn * CGridColumns::GetColumn(int nIndex)
{
	return m_columns.GetAt(nIndex);
}



//  --
void CGridColumns::SetColumn(int nIndex, CGridColumn *pColumn)
{
	if (nIndex < 0 || nIndex > m_columns.GetUpperBound())
		return;

	delete m_columns.GetAt(nIndex);
	m_columns.SetAt(nIndex, pColumn);
}



//  ---
void CGridColumns::Update(CGridColumn *pColumn)
{
	if (m_pGrid)
		m_pGrid->ColumnsChange();
}



////////////////////////////////////////////////////////////////////////////////
/////////////////////////////  CGridRows  //////////////////////////////////////


//  ============================================================================
CGridRows::CGridRows(CVirtualGridCtrl *pGrid)
{
	m_nCount = 0;
	m_pGrid = pGrid;
	m_nHeight = 17;
	m_bAutoHeight = FALSE;
}


//  ============================================================================
CGridRows::~CGridRows()
{
}


//  --
int CGridRows::GetMaxCount()
{
	if (m_nHeight > 0)
		return (INT_MAX - 2) / m_nHeight - 2;

	return INT_MAX - 2;
}


//  ----
void CGridRows::SetAutoHeight(BOOL bAutoHeight)
{
	if (m_bAutoHeight == bAutoHeight)
		return;		//  Nothing to change

	m_bAutoHeight = bAutoHeight;
	if (bAutoHeight)
		SetHeight(m_nHeight);
}


//  ---
void CGridRows::SetHeight(int nHeight)
{
	int nTextHeight, nFixedTextHeight;
	int nCheckHeight, nImageHeight;
	int nHeightWithGridLines;
	CGridFixed *pFixed;
	CImageList *pImageList;

	pFixed = m_pGrid->GetFixed();
	pImageList = m_pGrid->GetImageList();

	if (m_bAutoHeight && m_pGrid)  {
		//  Text height
		nTextHeight = m_pGrid->GetFontHeight(m_pGrid->GetFont()) + m_pGrid->GetTextTopIndent();
		nFixedTextHeight = m_pGrid->GetFontHeight(pFixed->GetFont());

		//  Flags height
		nCheckHeight = 0;
		if (m_pGrid->GetCheckBoxes())  {
			nCheckHeight = m_pGrid->GetCheckHeight() + m_pGrid->GetCheckTopIndent();
			if (pFixed->GetCount())  {
				if (!pFixed->GetFlat())
					nCheckHeight += 3;
				else if ((!pFixed->m_bGridColor) && (m_pGrid->GetGridLines())
					&& (gsHorzLine && m_pGrid->GetGridStyle()))

					nCheckHeight ++;
			}  else  {
				if ((m_pGrid->GetGridLines()) && (gsHorzLine & m_pGrid->GetGridStyle()))
					nCheckHeight += 1;
			}
		}

		//  Image height
		nImageHeight = 0;

		if (pImageList)  {
			IMAGEINFO ii;

			pImageList->GetImageInfo(0, &ii);
			nImageHeight = ii.rcImage.right - ii.rcImage.left + m_pGrid->GetImageTopIndent();

			if (pFixed->GetCount())  {
				if (!pFixed->GetFlat())
					nImageHeight += 3;
				else if ((!pFixed->m_bGridColor) && m_pGrid->GetGridLines()
					&& (gsHorzLine && m_pGrid->GetGridStyle()))

					nImageHeight ++;
			}  else  {
				if (m_pGrid->GetGridLines() && (gsHorzLine & m_pGrid->GetGridStyle()))
					nImageHeight ++;
			}
		}

		//  Taking into account the grid lines
		nHeightWithGridLines = m_pGrid->GetGridLineWidth() * 2 * (m_pGrid->GetGridLines() && (gsHorzLine & m_pGrid->GetGridStyle()));

		//  At last... row height:
		nHeight = 0;
		nHeight = max(nHeight, nTextHeight);
		nHeight = max(nHeight, nFixedTextHeight);
		nHeight = max(nHeight, nCheckHeight);
		nHeight = max(nHeight, nImageHeight);
		nHeight = max(nHeight, nHeightWithGridLines);
	}

	//  ---
	if (nHeight < 0)
		nHeight = 0;

	//  Set...
	if (m_nHeight == nHeight)
		return;		//  Nothing to change

	m_nHeight = nHeight;

	if (m_nCount > GetMaxCount())
		SetCount(m_nCount);
	else
		Change();
}


//  --------------------------  User's event  -----------------------------
void CGridRows::Change()
{
	if (m_pGrid)
		m_pGrid->RowsChange();
}


//  -----------------------------------------------------------------------
void CGridRows::SetCount(int nCount)
{
	if (nCount < 0)
		nCount = 0;

	if (nCount > GetMaxCount())
		nCount = GetMaxCount();

	if (nCount == m_nCount)
		return;		//  Nothing to change

	m_nCount = nCount;
	Change();
}



///////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////  CGridFixed  ////////////////////////////////////////


//  =================================================================================
CGridFixed::CGridFixed(CVirtualGridCtrl *pGrid)
{
	m_nCount = 0;
	m_pGrid = pGrid;
	m_color = ::GetSysColor(COLOR_BTNFACE);
	m_bGridFont = TRUE;
	m_bFlat = TRUE;
}


//  ==================================================================================
CGridFixed::~CGridFixed()
{
}


//  --
BOOL CGridFixed::IsColorStored()
{
	return !m_bGridColor;
}


//  --
BOOL CGridFixed::IsFontStored()
{
	return !m_bGridFont;
}


//  ---
void CGridFixed::FontChange()
{
	m_bGridFont = FALSE;
	Change();
}


//  ----------------------------------------------------------------
void CGridFixed::SetColor(COLORREF color)
{
	if (m_color == color)
		return;		//  Nothing to change

	m_color = color;
	m_bGridColor = FALSE;
	Change();
}


//  -----------------------------------------------------------------
void CGridFixed::SetFlat(BOOL bFlat)
{
	if (m_bFlat == bFlat)
		return;		//  Nothing to change

	m_bFlat = bFlat;

	//  Adjust 3D-effect of grid's header
	if (!bFlat && m_pGrid)
		m_pGrid->GetHeader()->SetFlat(FALSE);

	Change();
}


//  ----------------------------------------------------------------
void CGridFixed::SetFont(CFont *pFont)
{
	//  NYI
}


//  ----------------------------------------------------------------
void CGridFixed::SetGridColor(BOOL bGridColor)
{
	if (m_bGridColor == bGridColor)
		return;		//  Nothing to change

	m_bGridColor = bGridColor;
	if (m_pGrid)
		GridColorChanged(m_pGrid->GetGridColor());

	Change();
}


//  ---------------------------------------------------------------
void CGridFixed::SetGridFont(BOOL bGridFont)
{
	if (m_bGridFont == bGridFont)
		return;		//  Nothing to change

	m_bGridFont = bGridFont;
	if (m_pGrid)
		GridFontChanged(m_pGrid->GetFont());
	Change();
}


//  ----------------------------------------------------------------
void CGridFixed::Change()
{
	if (m_pGrid == NULL)
		return;		//  Nothing to change

	m_pGrid->FixedChange();
}


//  ----------------------------------------------------------------
void CGridFixed::GridColorChanged(COLORREF newColor)
{
	if (m_bGridColor)  {
		SetColor(newColor);
		m_bGridColor = TRUE;
	}
}


//  ---------------------------------------------------------------
void CGridFixed::GridFontChanged(CFont *pFont)
{
	if (m_bGridFont)  {
		SetFont(pFont);
		m_bGridFont = TRUE;
	}
}



//  --------------------------------------------------------------
void CGridFixed::SetCount(int nCount)
{
	//  Check the value
	if (m_pGrid && nCount > m_pGrid->GetColumns()->GetUpperBound())
		nCount = m_pGrid->GetColumns()->GetUpperBound();

	if (nCount < 0)
		nCount = 0;

	//  Set...
	if (m_nCount == nCount)
		return;		//  Nothing to set

	m_nCount = nCount;
	Change();
}


//  ------
CFont * CGridFixed::GetFont()
{
	return m_pFont;
}


//  ---------------------------------------------------------
BOOL CGridFixed::GetFlat()
{
	return m_bFlat;
}


//  ---
int CGridRows::GetCount()
{
	return m_nCount;
}


//  ---
int CGridFixed::GetCount()
{
	return m_nCount;
}


//  ------------------------------------------------------
int CGridHeaderSections::GetSize()
{
	return m_sections.GetSize();
}


//  -------------------------------------------------------
CVirtualGridCtrl * CGridHeader::GetGrid()
{
	return m_pGrid;
}


//  ------------------------------------------------------
CGridColumns * CVirtualGridCtrl::GetColumns()
{
	return &m_columns;
}



//  --
int CGridHeaderSections::GetUpperBound()
{
	return m_sections.GetUpperBound();
}


//  --
BOOL CVirtualGridCtrl::ColumnsSizing()
{
	return m_bColumnsSizing;
}


//  --
int CGridColumn::GetIndex()
{
	int i;

	if (!m_pColumns->GetSize())
		return -1;

	for (i = 0; i < m_pColumns->GetSize(); i ++)  {
		if (this == m_pColumns->GetColumn(i))
			return i;
	}

	return -1;
}



//  ---
CString CGridColumn::GetCaption()
{
	return m_strCaption;
}

int CGridColumn::GetAlignment()
{
	return m_nAlignment;
}


//  --
void CGridColumn::SetFixedSize(BOOL bFixedWidth)
{
	if (m_bFixedSize == bFixedWidth)
		return;		//  Nothing to change

	m_bFixedSize = bFixedWidth;
}

int CGridColumn::GetFixedSize()
{
	return m_bFixedSize;
}


//  ---
BOOL CGridColumn::GetReadOnly()
{
	return m_bReadOnly;
}


//  ---
BOOL CGridColumn::GetVisible()
{
	return m_bVisible;
}


//  --
void CGridColumn::Changed(BOOL bAllItems)
{
	if (m_pColumns)
		m_pColumns->Update(this);
}



//  --
void CGridHeaderSections::DeleteSections()
{
	int i;
	CGridHeaderSection *pSection;

	for (i = m_sections.GetUpperBound(); i >= 0; i --)  {
		pSection = m_sections[i];

		pSection->m_sections.DeleteSections();

		if (0 == pSection->m_sections.GetSize()
			&& pSection->m_nColumnIndex > m_pHeader->GetGrid()->GetColumns()->GetUpperBound())  {

			delete pSection;
			m_sections.RemoveAt(i);
		}
	}
}



//  ----------------------------------------------------------------
void CGridHeaderSections::Synchronize(int &nColumn)
{
	int i;
	CGridHeaderSection *pSection;
	CGridColumn *pColumn;

	for (i = m_sections.GetUpperBound(); i >= 0; i --)  {
		pSection = m_sections[i];

		if (0 == pSection->m_sections.GetSize())  {
			nColumn = pSection->m_nColumnIndex;

			pColumn = m_pHeader->GetGrid()->GetColumns()->GetColumn(nColumn);
			pSection->m_nWidth = pColumn->GetWidth();

			if (m_pHeader->m_bFullSynchronizing)  {
				pSection->m_strCaption = pColumn->GetCaption();
				pSection->m_nAlignment = pColumn->GetAlignment();
			}
		}  else  {
			pSection->m_sections.Synchronize(nColumn);
		}
	}
}


//  ---
CGridHeaderSections * CGridHeader::GetSections()
{
	return &m_sections;
}



//  ----
CGridHeaderSections& CGridHeaderSections::operator =(const CGridHeaderSections& otherSections)
{
	int i;

	for (i = 0; i < m_sections.GetSize(); i ++)
		delete m_sections[i];

	m_sections.RemoveAll();


	for (i = 0; i < otherSections.m_sections.GetSize(); i ++)
		m_sections.Add(otherSections.m_sections[i]);

	return *this;
}



//  ---
CString CGridHeaderSection::GetCaption()
{
	return m_strCaption;
}


//  ---
void CGridHeaderSection::Changed(BOOL bAllItems)
{

}




//  ---
CGridHeaderSection * CGridHeaderSection::Add()
{
	CGridHeaderSection *pSection;
	CGridHeader *pHeader;

	pSection = m_sections.Add();
	pHeader = GetHeader();
	if (pHeader)
		pHeader->GetGrid()->HeaderChange();

	return pSection;
}



//  ---
int CGridColumn::GetMaxLength()
{
	return m_nMaxLength;
}



//  --
BOOL CGridColumn::GetWantReturns()
{
	return m_bWantReturns;
}


//  ---
BOOL CGridColumn::GetWordWrap()
{
	return m_bWordWrap;
}



BOOL CGridColumn::GetAlignEdit()
{
	return m_bAlignEdit;
}


//  --
BOOL CGridColumn::GetTabStop()
{
	return m_bTabStop;
}


//  -----
GridCheckKind CGridColumn::GetCheckKind()
{
	return m_checkKind;
}


//  ----
BOOL CGridRows::GetAutoHeight()
{
	return m_bAutoHeight;
}


//  ----
int CGridRows::GetHeight()
{
	return m_nHeight;
}


//  ---
BOOL CGridHeader::GetAutoHeight()
{
	return m_bAutoHeight;
}


//  ---
BOOL CGridHeader::GetAutoSynchronize()
{
	return m_bAutoSynchronize;
}


//  ---
BOOL CGridHeader::GetFlat()
{
	return m_bFlat;
}

//  ---
BOOL CGridHeader::GetFullSynchronizing()
{
	return m_bFullSynchronizing;
}

//  ---
BOOL CGridHeader::GetGridColor()
{
	return m_bGridColor;
}

//  --
int CGridHeader::GetSectionHeight()
{
	return m_nSectionHeight;
}


//  --
BOOL CGridFixed::GetGridColor()
{
	return m_bGridColor;
}


