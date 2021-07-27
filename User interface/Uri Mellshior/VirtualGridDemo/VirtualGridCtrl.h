#if !defined(AFX_VIRTUALGRIDCTRL_H__201F6FEE_CCA2_4B13_BEE2_6EEEF5D2BEF4__INCLUDED_)
#define AFX_VIRTUALGRIDCTRL_H__201F6FEE_CCA2_4B13_BEE2_6EEEF5D2BEF4__INCLUDED_


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


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VirtualGridCtrl.h : header file
//

#include <afxtempl.h>
#include "GridEdit.h"
#include "TitleTip.h"

#define VIRTUAL_GRID_CLASS_NAME			_T("CVirtualGridCtrl")

#define MAX_COLUMN_WIDTH			10000


//  -------------  Thank you, Chris  ----------
#define IS_SHIFT_PRESSED()	((GetKeyState(VK_SHIFT) & (1 << (sizeof(SHORT) * 8 - 1))) != 0)
#define IS_CTRL_PRESSED()	((GetKeyState(VK_CONTROL) & (1 << (sizeof(SHORT) * 8 - 1))) != 0)


//  =======================  Grid styles  ==============================
enum {gsHorzLine = 1, gsVertLine = 2, gsFullHorzLine = 4, gsFullVertLine = 8,
	gsListViewLike = 16, gsDotLines = 32};

enum {gkArrows = 1, gkTabs = 2, gkReturn = 4, gkMouse = 8, gkMouseMove = 16,
	gkMouseWheel = 32};

//  ----
enum GridCheckKind {gcNone, gcCheckBox, gcRadioButton, gcUserDefine};

//  --
enum GridCheckStyle {cs3D, csFlat};

//  ---
enum GridCursorOffset {goLeft, goRight, goUp, goDown, goPageUp, goPageDown,
	goHome, goEnd, goGridHome, goGridEnd, goSelect, goFirst, goNext, goPrev};

//  ---
enum GridSortDirection  {gsNone, gsAcsending, gsDescending};


#define VGN_GETDISPINFO			LVN_GETDISPINFO
#define VGN_SETDISPINFO			LVN_SETDISPINFO

#define VGN_CELLACCEPTCURSOR		(LVN_FIRST - 60)
#define VGN_GETEDITSTYLE		(LVN_FIRST - 61)
#define VGN_GETALLOWEDIT		(LVN_FIRST - 62)
#define VGN_EDITBUTTONPRESS		(LVN_FIRST - 63)
#define VGN_GETEDITLIST			(LVN_FIRST - 64)
#define VGN_EDITACCEPTKEY		(LVN_FIRST - 65)
#define VGN_EDITCANCELED		(LVN_FIRST - 66)
#define VGN_GETCELLTEXTINDENT		(LVN_FIRST - 67)
#define VGN_DRAWCELL			(LVN_FIRST - 68)
#define VGN_GETCELLIMAGEINDENT		(LVN_FIRST - 69)
#define VGN_GETEDITTEXT			(LVN_FIRST - 70)
#define VGN_HEADERCLICKING		(LVN_FIRST - 71)
#define VGN_HEADERCLICK			(LVN_FIRST - 72)
#define VGN_SETEDITTEXT			(LVN_FIRST - 73)
#define VGN_GETCHECKKIND		(LVN_FIRST - 74)
#define VGN_GETCHECKSTATE		(LVN_FIRST - 75)
#define VGN_GETHEADERIMAGE		(LVN_FIRST - 76)
#define VGN_CHECKCLICK			(LVN_FIRST - 77)


#define LVIF_COLOR			LVIF_PARAM

#define VGM_EXIT			(WM_USER)


//  ------------------  Pre-definitions  ---------------------
class CGridHeaderSection;
class CGridHeaderSections;
class CGridHeader;
class CGridColumn;
class CGridColumns;
class CGridRow;
class CGridRows;
class CGridFixed;
class CVirtualGridCtrl;



//  --------
struct VGITEM  {
	//  ------------  "Traditional" ListCtrl-like part  ------------
	int mask;			//  LVIF_TEXT, LVIF_IMAGE, LVIF_COLOR
	int nColumn;			//  Cell's column (0 - based)
	int nRow;			//  Cell's row (0 - based)
	CString strText;		//  For use with VGN_GETDISPINFO (with LVIF_TEXT mask)
	int nAlign;			//  LVCFMT_LEFT, LVCFMT_RIGHT, LVCFMT_CENTER
	int nImage;			//  Cell's image index (- 1) if no image required

	//  -------  Extended, VirtualGridCtrl-like part  ------
	BOOL bAcceptCursor;		//  For use with VGN_CELLACCEPTCURSOR
	BOOL bReadOnly;			//  For use with VGN_GETEDITSTYLE
	BOOL bEditAcceptKey;		//  For use with VGN_EDITACCEPTKEY
	BOOL bDefDraw;			//  *reserved*
	GridEditStyle style;		//  For use with VGN_GETEDITSTYLE
	CStringArray listStrings;	//  For use with VGN_GETEDITLIST
	CPoint indent;			//  For use with VGN_GETCELLTEXTINDENT
	RECT rc;			//  *reserved*
	CDC *pDC;			//  To set cell's background and text colors...
	GridCheckKind checkKind;	//  For use with VGN_GETCHECKKIND
	int nCheckState;		//  For use with VGN_GETCHECKSTATE
	CGridHeaderSection *pSection;	//  For VGN_HEADERCLICKING / VGN_HEADERCLICK handling
};



//  -----
struct VG_DISPINFO  {
	NMHDR hdr;
	VGITEM item;
};


//  -------------
class CGridCell  {
public:
	CGridCell(int nColumn = 0, int nRow = 0)
	{
		m_nColumn = nColumn;
		m_nRow = nRow;
	}

	//  -------------------------------------
	BOOL operator ==(CGridCell& cell) const
	{
		if (m_nColumn == cell.m_nColumn && m_nRow == cell.m_nRow)
			return TRUE;
		return FALSE;
	}

	BOOL IsEmpty() const
	{
		return (m_nColumn < 0) || (m_nRow < 0);
	}

	//  ------------------------------------
	BOOL operator !=(CGridCell& cell) const
	{
		if (m_nColumn == cell.m_nColumn && m_nRow == cell.m_nRow)
			return FALSE;
		return TRUE;
	}

	//  -----------------------------------

	int m_nColumn;
	int m_nRow;
};



/////////////////////////////////////////////////////////////////////////////
///////////////////////////  CGridHeaderSections  ///////////////////////////
class CGridHeaderSections : public CObject  {
	friend class CVirtualGridCtrl;
	friend class CGridHeader;
	friend class CGridHeaderSection;
public:
	CGridHeaderSections();
	~CGridHeaderSections();
	CGridHeaderSections& operator =(const CGridHeaderSections& otherSections);
	CGridHeaderSection * GetSection(int nColumnIndex, int nLevel);
	CGridHeaderSection * operator[](int nIndex)
	{
		return m_sections[nIndex];
	}

	int GetUpperBound();
	int GetSize();
	BOOL FindResizeSection(CPoint point, CGridHeaderSection *& pSection);
	BOOL FindSection(CPoint point, CGridHeaderSection *& pRetrievedSection);
	void SetSection(int nIndex, CGridHeaderSection *pSection);
	CGridHeaderSection * GetSection(int nIndex);
	int GetMaxLevel();
	int GetMaxColumn();
	CGridHeaderSection * Add();
	CTypedPtrArray<CObArray, CGridHeaderSection *> m_sections;
protected:
	virtual void Update(CGridHeaderSection *pSection);
	virtual CGridHeader * GetOwner();
private:
	void Synchronize(int& nColumn);
	void DeleteSections();
	void UpdateSectionsBounds(LPRECT lpRect);
	void UpdateColumnIndex(int& nColumn);
	CGridHeader *m_pHeader;
	CGridHeaderSection *m_pOwnerSection;
};



/////////////////////////////////////////////////////////////////////////////
//////////////////////////  CGridHeaderSection  /////////////////////////////
class CGridHeaderSection : public CObject  {
	friend class CVirtualGridCtrl;
	friend class CGridHeader;
	friend class CGridHeaderSections;
public:
	int GetColumnIndex();
	CGridHeaderSection(const CGridHeaderSection& otherSection);
	CGridHeaderSection(CGridHeaderSections *pParentSections);
	~CGridHeaderSection();
	CGridHeaderSection& operator =(CGridHeaderSection& otherSection);
	BOOL GetWordWrap();
	int GetAlignment();
	CGridHeaderSection * Add();
	virtual void Changed(BOOL bAllItems = TRUE);
	CString GetCaption();
	void SetWordWrap(BOOL bWordWrap = TRUE);
	void SetWidth(int nWidth = 64);
	void SetSections(const CGridHeaderSections& sections);
	void SetCaption(LPCTSTR lpCaption);
	void SetAlignment(int nAlignment);
	int GetWidth();
	BOOL GetVisible();
	CGridHeaderSections * GetSections();
	int GetResizeColumnIndex();
	CGridHeaderSections * GetParentSections();
	CGridHeaderSection * GetParent();
	int GetLevel();
	CGridHeader * GetHeader();
	BOOL GetFixedColumn();
	int GetFirstColumnIndex();
	CRect GetBoundsRect();
	BOOL GetAllowClick();
	BOOL IsWidthStored();
	BOOL IsSectionsStored();
	CGridHeaderSections m_sections;

	//  ----------  Design Time  -----------
	HTREEITEM m_hItem;
private:
	CGridHeaderSections *m_pParentSections;
	CString m_strCaption;
	int m_nWidth;
	int m_nAlignment;	//  LVCFMT_LEFT / LVCFMT_RIGHT / LVCFMT_CENTER
	BOOL m_bWordWrap;
	CRect m_boundsRect;
	int m_nColumnIndex;
};





/////////////////////////////////////////////////////////////////////////////
///////////////////////////////  CGridHeader  ///////////////////////////////
class CGridHeader : public CObject  {
	friend class CVirtualGridCtrl;
	friend class CGridHeaderSections;
public:
	CGridHeader(CVirtualGridCtrl *pGrid = NULL);
	~CGridHeader();
	int GetSectionHeight();
	BOOL GetGridColor();
	BOOL GetFullSynchronizing();
	BOOL GetFlat();
	BOOL GetAutoSynchronize();
	BOOL GetAutoHeight();
	CGridHeaderSections * GetSections();
	CVirtualGridCtrl * GetGrid();
	void SetSynchronized(BOOL bSynchronized = TRUE);
	void SetSectionHeight(int nHeight = 17);
	void SetSections(const CGridHeaderSections& sections);
	void SetImageList(CImageList *pImageList);
	void SetGridFont(BOOL bGridFont = TRUE);
	void SetGridColor(BOOL bGridColor = TRUE);
	void SetFullSynchronizing(BOOL bFullSynchronizing = TRUE);
	void SetFont(CFont *pFont);
	void SetFlat(BOOL bFlat = TRUE);
	void SetColor(COLORREF color);
	void SetAutoSynchronize(BOOL bAutoSynchronize = TRUE);
	void SetAutoHeight(BOOL bAutoHeight = TRUE);
	int GetWidth();
	int GetMaxLevel();
	int GetMaxColumn();
	int GetHeight();
	BOOL IsSectionsStored();
	BOOL IsFontStored();
	BOOL IsColorStored();
	virtual void UpdateSections();
	void SynchronizeSections();
private:
	CVirtualGridCtrl *m_pGrid;
	CGridHeaderSections m_sections;
	int m_nSectionHeight;
	BOOL m_bAutoHeight;
	BOOL m_bSynchronized;
	BOOL m_bAutoSynchronize;
	BOOL m_bFullSynchronizing;
	COLORREF m_color;
	BOOL m_bGridColor;
	CFont *m_pFont;
	BOOL m_bGridFont;
	CImageList *m_pImageList;
	BOOL m_bFlat;
protected:
	virtual void GridFontChanged(CFont *pNewFont);
	virtual void GridColorChanged(COLORREF newColor);
	virtual void Change();
};




/////////////////////////////////////////////////////////////////////////////
////////////////////////////  CGridColumn  //////////////////////////////////
class CGridColumn : public CObject  {
	friend class CVirtualGridCtrl;
	friend class CGridHeaderSection;
	friend class CGridHeaderSections;
	friend class CGridColumns;
	DECLARE_DYNCREATE(CGridColumn)
public:
	GridCheckKind GetCheckKind();
	BOOL GetTabStop();
	BOOL GetAlignEdit();
	BOOL GetWordWrap();
	BOOL GetWantReturns();
	int GetMaxLength();
	GridEditStyle GetEditStyle();
	int GetMinWidth();
	int GetMaxWidth();
	void SetAllowClick(BOOL bAllowClick = TRUE);
	BOOL GetAllowClick();
	CGridColumn(CGridColumns *pOwnerColumns = NULL);
	~CGridColumn();
	CGridColumn(const CGridColumn& otherColumn);
	CGridColumn& operator =(const CGridColumn& otherColumn);
	BOOL GetVisible();
	BOOL GetReadOnly();
	int GetFixedSize();
	void SetFixedSize(BOOL bFixedWidth = TRUE);
	int GetAlignment();
	CString GetCaption();
	int GetIndex();
	virtual void SetWidth(int nWidth = 64);
	virtual void SetVisible(BOOL bVisible = TRUE);
	virtual void SetReadOnly(BOOL bReadOnly = TRUE);
	virtual void SetMaxLength(int nMaxLength);
	virtual void SetEditStyle(GridEditStyle style);
	virtual void SetEditMask(LPCTSTR lpEditMask);
	virtual void SetCaption(LPCTSTR lpCaption);
	void SetAlignment(int nAlignment);	//  LVCFMT_LEFT / LVCFMT_RIGHT / LVCFMT_CENTER
	void SetWordWrap(BOOL bWordWrap = TRUE);
	void SetWantReturns(BOOL bWantReturns = TRUE);
	void SetTabStop(BOOL bTabStop = TRUE);
	void SetPickList(const CStringArray& pickList);
	void SetMinWidth(int nMinWidth);
	void SetMaxWidth(int nMaxWidth);
	void SetCheckKind(GridCheckKind checkKind);
	void SetAlignEdit(BOOL bAlignEdit = TRUE);
	BOOL IsPickListStored();
	int GetWidth();
	CGridHeaderSection * GetTitle();
	int GetPickListCount();
	void GetPickList(CStringArray& pickList);
	CVirtualGridCtrl * GetGrid();
	int GetEditAlignment();		//  LVCFMT_LEFT / LVCFMT_RIGHT / LVCFMT_CENTER
private:
	CGridColumns *m_pColumns;
	CString m_strCaption;
	int m_nWidth;
	int m_nMinWidth;
	int m_nMaxWidth;
	BOOL m_bFixedSize;
	int m_nMaxLength;
	int m_nAlignment;
	BOOL m_bReadOnly;
	BOOL m_bWantReturns;
	BOOL m_bWordWrap;
	GridEditStyle m_editStyle;
	CString m_strEditMask;
	GridCheckKind m_checkKind;
	BOOL m_bTabStop;
	BOOL m_bVisible;
	CStringArray m_pickList;
	BOOL m_bAllowClick;
	int m_nTag;
	BOOL m_bAlignEdit;
protected:
	virtual void Changed(BOOL bAllItems = TRUE);
};



////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////  CGridColumns  ///////////////////////////////
class CGridColumns : public CObject  {
	friend class CVirtualGridCtrl;
	friend class CGridColumn;
public:
	CGridColumns();
	CGridColumns(const CGridColumns& otherColumns);
	CGridColumns& operator =(const CGridColumns& otherColumns);
	~CGridColumns();
	int GetUpperBound();
	int GetSize();
	CGridColumn * GetAt(int nIndex);
	void SetColumn(int nIndex, CGridColumn *pColumn);
	CGridColumn * GetColumn(int nIndex);
	CGridColumn * Add();
	CGridColumn * operator[](int i)
	{
		return m_columns[i];
	}
	void RemoveAt(int nIndex);
private:
	CTypedPtrArray<CObArray, CGridColumn *> m_columns;
	CVirtualGridCtrl *m_pGrid;


protected:
	virtual void Update(CGridColumn *pColumn);
};




/////////////////////////////////////////////////////////////////////////////////
////////////////////////////  CGridRows  ////////////////////////////////////////
class CGridRows : public CObject  {
	friend class CVirtualGridCtrl;
public:
	int GetHeight();
	BOOL GetAutoHeight();
	CGridRows(CVirtualGridCtrl *pGrid = NULL);
	~CGridRows();
	int GetCount();
	virtual void SetCount(int nCount);
	void SetHeight(int nHeight = 17);
	void SetAutoHeight(BOOL bAutoHeight = TRUE);
	int GetMaxCount();
private:
	CTypedPtrArray<CObArray, CGridRow *> m_rows;
	CVirtualGridCtrl *m_pGrid;
	int m_nCount;
	int m_nHeight;
	BOOL m_bAutoHeight;
protected:
	virtual void Change();
};




////////////////////////////////////////////////////////////////////////////
////////////////////////////  CGridFixed  //////////////////////////////////
class CGridFixed : public CObject  {
	friend class CVirtualGridCtrl;
	friend class CGridRows;
public:
	BOOL GetGridColor();
	BOOL GetFlat();
	CFont * GetFont();
	int GetCount();
	virtual void SetCount(int nCount);
	void SetGridFont(BOOL bGridFont = TRUE);
	void SetGridColor(BOOL bGridColor = TRUE);
	void SetFont(CFont *pFont);
	void SetFlat(BOOL bFlat = TRUE);
	void SetColor(COLORREF color);
	void FontChange();
	BOOL IsFontStored();
	BOOL IsColorStored();
	CGridFixed(CVirtualGridCtrl *pGrid = NULL);
	~CGridFixed();
private:
	CVirtualGridCtrl *m_pGrid;
	int m_nCount;
	COLORREF m_color;
	BOOL m_bGridColor;
	CFont *m_pFont;
	BOOL m_bGridFont;
	BOOL m_bFlat;

protected:
	virtual void GridFontChanged(CFont *pFont);
	virtual void GridColorChanged(COLORREF newColor);
	virtual void Change();
};



/////////////////////////////////////////////////////////////////////////////
// CVirtualGridCtrl window

class CVirtualGridCtrl : public CWnd  {
	DECLARE_DYNCREATE(CVirtualGridCtrl)
	friend class CGridEdit;
// Construction
public:
	CVirtualGridCtrl();

	virtual ~CVirtualGridCtrl();

	//  You construct a CVirtualGridCtrl object in two steps. First call the constructor,
	//  then call Create, which creates the Virtual Grid control and attaches it to
	//  the CVirtualGridCtrl object.
	BOOL Create(const RECT& rect, CWnd *pParentWnd, UINT nID,
		DWORD dwStyle = WS_CHILD | WS_BORDER | WS_TABSTOP | WS_VISIBLE);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVirtualGridCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	//  Is currently using the CMemDC?
	BOOL GetDoubleBuffered();

	//  To use or not to use CMemDC...
	void SetDoubleBuffered(BOOL bDoubleBuffered = TRUE);

	//  Simple approximate calculation of font height using CDC::GetTextExtent()
	int GetFontHeight(CFont *pFont);

	//  Retrieves the style of cell's check boxes.
	//  Possible vaues:
	//	- cs3D (default)
	//	- csFlat (using the DFCS_FLAT style)
	GridCheckStyle GetCheckStyle();

	//  Is the focus must always be shown?
	BOOL GetAlwaysSelected();

	//  Always show in-cell edit control?
	BOOL GetAlwaysEdit();

	//  Is the in-cell edit control can be visible?
	//  NOTE: Although the edit control can be visible, it is not necessary that
	//  the cell could be edited.  Editing possibility is controlled by m_bReadOnly
	//  field of appropriate column or the whole grid.  (Use GetReadOnly() / SetReadOnly()
	//  for this).
	BOOL GetAllowEdit();

	//
	void SetCancelOnExit(BOOL bCancel = TRUE);

	//
	BOOL GetCancelOnExit();

	//  Retrieves current amount of regular grid rows.
	int GetRowCount();

	//  Set the height for every grid row.
	void SetRowHeight(int nHeight = 17);

	//  To select all the row or single cell only?
	BOOL GetRowSelect();

	//  Is cell selection allowed?
	BOOL GetAllowSelect();

	//  Setting amount of fixed columns.
	void SetFixedCount(int nCount);

	//  Retrieves current amount of fixed columns.
	int GetFixedCount();

	//  Retrieves the pointer to grid's CGridColumns object.
	CGridColumns* GetGridColumns();

	//  Retrieves currently celected regular grid cell (or cell with column and row both set
	//  to -1 if no cell is actually selected).
	CGridCell GetCellFocused();

	//  Adds new column to the grid with caption set to lpCaption, with set to nWidth and
	//  text alignment set to nAlignment.
	int AddColumn(LPCTSTR lpCaption = _T(""), int nWidth = 64, int nAlignment = LVCFMT_LEFT);

	//  Sets total amount of grid rows.
	void SetRowCount(int nCount);


	void SetRowsHeight(int nHeight = 17);
	BOOL GetReadOnly();
	int GetTextRightIndent();
	CGridCell GetEditCell();
	int GetGridLineWidth();
	int GetImageTopIndent();
	CImageList * GetImageList();
	UINT GetGridStyle();
	BOOL GetGridLines();
	int GetCheckTopIndent();
	int GetCheckHeight();
	BOOL GetCheckBoxes();
	COLORREF GetGridColor();
	CGridColumn * GetColumn(int nIndex);
	int GetTextTopIndent();
	CGridHeaderSections * GetHeaderSections();
	BOOL ColumnsSizing();
	CGridColumns* GetColumns();
	virtual CGridEdit * CreateEdit(CRuntimeClass *pEditClass);
	virtual void CellTips(const CGridCell& cell, BOOL& bAllowTips);
	void VertScrollChange();
	void SetVisOrigin(const CGridCell& cell);
	void SetTopRow(int nTopRow);
	void SetTextTopIndent(int nTopIndent);
	void SetTextRightIndent(int nRightIndent);
	void SetTextLeftIndent(int nLeftIndent);
	void SetSortTopIndent(int nTopIndent);
	void SetSortLeftIndent(int nLeftIndent);
	void SetShowHeader(BOOL bShowHeader = TRUE);
	void SetShowFocusRect(BOOL bShowFocusRect = TRUE);
	void SetShowCellTips(BOOL bShowCellTips = TRUE);
	void SetRowSelect(BOOL bRowSelect = TRUE);
	void SetRows(CGridRows *pRows);
	void SetRow(int nRow);
	void SetReadOnly(BOOL bReadOnly = TRUE);
	void SetLeftCol(int nLeftCol);
	void SetImageTopIndent(int nTopIndent);
	void SetImageList(CImageList *pImageList);
	void SetImageLeftIndent(int nLeftIndent);
	void SetImageHighlight(BOOL bImageHighlight = TRUE);
	void SetImageIndexDef(int nIndex);
	void SetHideSelection(BOOL bHideSelection = TRUE);
	void SetHeader(CGridHeader *pHeader);
	void SetGridStyle(UINT nGridStyles);
	void SetGridLines(BOOL bGridLines = TRUE);
	void SetGridColor(COLORREF gridColor);
	void SetFixed(CGridFixed *pFixed);
	void SetFlatScrollBars(BOOL bFlatScrollBars = TRUE);
	void SetEndEllipsis(BOOL bEndEllipsis = TRUE);
	void SetEditing(BOOL bEditing = TRUE);
	void SetEditDropDown(BOOL bEditDropDown = TRUE);
	void SetCursorKeys(UINT nCursorKeys);
	void SetColumns(const CGridColumns& columns);
	void SetCol(int nCol);
	void SetCheckTopIndent(int nTopIndent);
	void SetCheckStyle(GridCheckStyle checkStyle);
	void SetCheckLeftIndent(int nLeftIndent);
	void SetCheckBoxes(BOOL bCheckBoxes = TRUE);
	void SetCellSelected(BOOL bCellSelected = TRUE);
	void SetCellFocused(CGridCell& cell);
	void SetCell(int nColumn, int nRow, CString& strText);
	void SetAlwaysSelected(BOOL bAlwaysSelected = TRUE);
	void SetAlwaysEdit(BOOL bAlwaysEdit = TRUE);
	void SetAllowSelect(BOOL bAllowSelect = TRUE);
	void SetAllowEdit(BOOL bAllowEdit = TRUE);
	void HeaderChange();
	int GetVisibleRowCount();
	int GetVisibleColCount();
	int GetTopRow();
	CGridRows* GetRows();
	int GetRow();
	int GetLeftCol();
	CGridHeader * GetHeader();
	BOOL GetEditing();
	BOOL GetEditDropDown();
	CGridColumn * GetEditColumn();
	CGridEdit * GetEdit();
	CGridFixed * GetFixed();
	int GetCol();
	BOOL GetChecked(int nColumn, int nRow);
	CString GetCell(int nColumn, int nRow);
	virtual void UpdateVisOriginSize();
	virtual void UpdateText();
	virtual void UpdateSelection(CGridCell& cell, BOOL& bSelected);
	virtual void UpdateScrollPos();
	virtual void UpdateScrollBars();
	virtual void UpdateRows();
	virtual void UpdateHeader();
	virtual void UpdateFonts();
	virtual void UpdateFocus();
	virtual void UpdateFixed();
	virtual void UpdateEditText();
	virtual void UpdateEditContents(BOOL bSaveText = TRUE);
	virtual void UpdateEdit(BOOL bActivate = TRUE);
	virtual void UpdateColors();
	virtual void UpdateCursor();
	virtual void UndoEdit();
	virtual void SetCursor(CGridCell& cell, BOOL bSelected, BOOL bVisible);
	BOOL IsRowVisible(int nRow);
	virtual BOOL IsHeaderPressed(CGridHeaderSection *pSection);
	virtual BOOL IsHeaderHasImage(CGridHeaderSection *pSection);
	BOOL IsFocusAllowed();
	BOOL IsColumnVisible(int nColumn);
	BOOL IsCellVisible(const CGridCell& cell, BOOL bPartialOK = TRUE);
	BOOL IsCellValid(const CGridCell& cell, BOOL bCheckPosition = TRUE, BOOL bCheckVisible = TRUE);
	virtual BOOL IsCellReadOnly(const CGridCell& cell);
	BOOL IsCellFocused(const CGridCell& cell);
	virtual BOOL IsCellHasImage(const CGridCell& cell);
	virtual BOOL IsCellHasCheck(const CGridCell& cell);
	virtual BOOL IsCellAcceptCursor(const CGridCell& cell);
	BOOL IsActiveControl();
	void InvalidateRows(int nFirstRow, int nLastRow);
	void InvalidateRow(int nRow);
	void InvalidateHeader();
	void InvalidateGrid();
	virtual void InvalidateFocus();
	void InvalidateFixed();
	void InvalidateEdit();
	void InvalidateColumns(int nFirstColumn, int nLastColumn);
	void InvalidateColumn(int nColumn);
	void InvalidateCheck(const CGridCell& cell);
	void InvalidateCell(const CGridCell& cell);
	CGridHeaderSection * GetSectionAt(CPoint point);
	CRect GetRowTopBottom(int nRow);
	int GetRowsHeight(int nFirstRow, int nLastRow);
	CRect GetRowsRect(int nFirstRow, int nLastRow);
	CRect GetRowRect(int nRow);
	virtual int GetRowAt(int x, int y);
	CGridHeaderSection * GetResizeSectionAt(CPoint point);
	CGridHeaderSection * GetHeaderSection(int nColumnIndex, int nLevel);
	virtual CRect GetHeaderRect();
	int GetHeaderHeight();
	virtual CRect GetGridRect();
	CPoint GetGridOrigin();
	int GetGridHeight();
	virtual CRect GetFocusRect();
	int GetFixedWidth();
	virtual CRect GetFixedRect();
	int GetFirstImageColumn();
	virtual CRect GetEditRect(const CGridCell& cell);
	int GetColumnsWidth(int nFirstColumn, int nLastColumn);
	CRect GetColumnsRect(int nFirstColumn, int nLastColumn);
	CRect GetColumnRect(int nColumn);
	int GetColumnMaxWidth(int nColumn);
	CRect GetColumnLeftRight(int nColumn);
	virtual int GetColumnAt(int x, int y);
	CRect GetCellRect(const CGridCell& cell);
	virtual CGridCell GetCellAt(int x, int y);
	virtual void DrawDragRect(CDC *pDC, const CGridCell& cell);
	virtual void DefaultDrawHeader(CDC *pDC, CGridHeaderSection *pSection, LPRECT lpRect);
	virtual void DefaultDrawCell(CDC *pDC, const CGridCell& cell, LPRECT lpRect);
	virtual void CancelEdit();
	virtual void ApplyEdit();
	void ColumnsChange();
	void RowsChange();
	void FixedChange();

	virtual CPoint GetCellTextIndent(const CGridCell& cell);
	virtual CString GetEditText(const CGridCell& cell);
	virtual GridEditStyle GetEditStyle(const CGridCell& cell);
	virtual void EditButtonPress(const CGridCell& cell);
protected:
	virtual void Paint(CDC *pDC);
	afx_msg LRESULT OnExit(WPARAM wParam, LPARAM lParam);
	VG_DISPINFO m_dispInfo;
	virtual void HideEdit();
	CPen m_penBtnShadow;
	CPen m_penBtnHighlight;
	CPen m_penGridLines;
	BOOL m_bTitleTips;
	CTitleTip m_titleTip;
	virtual void Paint3DFrame(CDC *pDC, LPRECT lpRect, UINT nSideFlags);
	virtual void PaintCells(CDC *pDC);
	void StopHeaderClick(BOOL bAbort = TRUE);
	void StopColResize(BOOL bAbort = TRUE);
	void StepHeaderClick(CPoint point);
	void StepColResize(int x, int y);
	void StartHeaderClick(CGridHeaderSection *pSection, CPoint point);
	virtual void StartColResize(CGridHeaderSection *pSection, int x, int y);
	virtual void ShowFocus();
	virtual void ShowEditChar(UINT nChar);
	virtual void ShowEdit();
	virtual void ShowCursor();
	virtual void SetEditText(const CGridCell& cell, CString& strText);
	virtual void ResetClickPos();
	virtual void PreparePatternBitmap(CDC *pDC, COLORREF fillColor, BOOL bRemove);
	virtual void PaintText(CDC *pDC, LPRECT lpRect, int nLeftIndent, int nTopIndent, int nAlignmennt, BOOL bWantReturns, BOOL bWordWrap, LPCTSTR lpText);
	virtual void PaintResizeLine();
	virtual void PaintHeaderSections(CDC *pDC, CGridHeaderSections *pSections, BOOL bAllowFixed = TRUE);
	virtual void PaintHeaders(CDC *pDC, BOOL bDrawFixed = TRUE);
	virtual void PaintHeader(CDC *pDC, CGridHeaderSection *pSection, LPRECT lpRect);
	virtual void PaintGridLines(CDC *pDC);
	virtual void PaintFocus(CDC *pDC);
	virtual void PaintFreeField(CDC *pDC);
	virtual void PaintFixedGrid(CDC *pDC);
	virtual void PaintFixed(CDC *pDC);
	virtual void PaintDotGridLines(CDC *pDC, LPPOINT lpPoints, int nCount);
	virtual void PaintCell(CDC *pDC, const CGridCell& cell, LPRECT lpRect);
	virtual void HideFocus();
	virtual void HideCursor();
	virtual void HeaderClicking(CGridHeaderSection *pSection, BOOL& bAllowClick);
	virtual void HeaderClick(CGridHeaderSection *pSection);
	virtual CRect GetTextRect(CDC *pDC, LPRECT lpRect, int nLeftIndent, int nTopIndent, int nAlignment, BOOL bWantReturns, BOOL bWordWrap, LPTSTR lpText);
	virtual void GetSortImage(CGridHeaderSection *pSection, CBitmap *pSortImage);
	virtual GridSortDirection GetSortDirection(CGridHeaderSection *pSection);
	virtual void GetHeaderColors(CGridHeaderSection *pSection, CDC *pDC);
	virtual int GetHeaderImage(CGridHeaderSection *pSection);
	virtual COLORREF GetGridLineColor(COLORREF bkColor);
	virtual CString GetEditMask(const CGridCell& cell);
	virtual void GetEditListBounds(const CGridCell& cell, LPRECT lpRect);
	virtual void GetEditList(const CGridCell& cell, CStringArray& items);
	virtual CRuntimeClass * GetEditClass(const CGridCell& cell);
	virtual CGridCell GetCursorCell(CGridCell& cell, GridCursorOffset offset);
	virtual CRuntimeClass * GetColumnClass();
	virtual CPoint GetClientOrigin();
	virtual int GetCheckState(const CGridCell& cell);
	virtual CRect GetCheckRect(const CGridCell& cell);
	virtual GridCheckKind GetCheckKind(const CGridCell& cell);
	virtual CPoint GetCheckIndent(const CGridCell& cell);
	virtual void GetCheckImage(const CGridCell& cell, CBitmap *pCheckImage);
	virtual int GetCheckAlignment(const CGridCell& cell);	//  LVCFMT_LEFT / LVCFMT_RIGHT / LVCFMT_CENTER
	virtual CRect GetCellTextBounds(const CGridCell& cell);
	virtual CString GetCellText(const CGridCell& cell);
	virtual CRect GetCellHintRect(const CGridCell& cell);
	virtual CRect GetCellImageRect(const CGridCell& cell);
	virtual CPoint GetCellImageIndent(const CGridCell& cell);
	virtual int GetCellImage(const CGridCell& cell);
	virtual void GetCellColors(const CGridCell& cell, CDC *pDC);
	virtual void EditSelectNext(const CGridCell& cell, CString& strText);
	virtual void EditCloseUp(const CGridCell& cell, int nItemIndex, BOOL& bAccept);
	virtual void EditChange(const CGridCell& cell);
	virtual BOOL EditCanUndo(const CGridCell& cell);
	virtual BOOL EditCanShow(const CGridCell& cell);
	virtual BOOL EditCanModify(const CGridCell& cell);
	virtual void EditCanceled(const CGridCell& cell);
	virtual BOOL EditCanAcceptKey(const CGridCell& cell, char key);
	virtual void ColumnResizing(int nColumn, int& nWidth);
	virtual void ColumnResize(int nColumn, int& nWidth);
	virtual void ColumnAutoSize(int nColumn, int& nWidth);
	virtual void CheckClick(const CGridCell& cell);
	virtual void Changing(CGridCell& cell, BOOL& bSelected);
	virtual void ChangeRows();
	virtual void ChangeFixed();
	virtual void ChangeEditMode();
	virtual void ChangeEditing();
	virtual void ChangeColumns();
	virtual void Change(CGridCell& cell, BOOL& bSelected);
	virtual void CellClick(const CGridCell& cell, UINT nFlags, int x, int y);
	virtual BOOL AcquireFocus();
	void MakeCellVisible(const CGridCell& cell, BOOL bPartialOK = FALSE);
	//{{AFX_MSG(CVirtualGridCtrl)
	afx_msg void OnPaint();
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void PaintHorz3DLines(CDC *pDC, LPRECT lpRect);
	void PaintBottom3DMargin(CDC *pDC, LPRECT lpRect);
	void PaintVert3DLines(CDC *pDC, LPRECT lpRect, BOOL bDrawBottomLine);
	void ShiftGridPoints(int nPoints, LPPOINT lpPoins, int dx, int dy);
	void DoUpdateEdit();
	void ValidateEdit();
	CGridCell Prev(const CGridCell& cell);
	CGridCell Next(const CGridCell& cell);
	CGridCell First();
	CGridCell Select(const CGridCell& cell);
	CGridCell MoveGridEnd(const CGridCell& cell);
	CGridCell MoveGridHome(const CGridCell& cell);
	CGridCell MoveEnd(CGridCell& cell);
	CGridCell MoveHome(CGridCell& cell);
	CGridCell MoveDown(const CGridCell& cell, int nOffset);
	CGridCell MoveUp(const CGridCell& cell, int nOffset);
	CGridCell MoveRight(const CGridCell& cell, int nOffset);
	CGridCell MoveLeft(const CGridCell& cell, int nOffset);
	void ImagesChange();
	void HorzScrollChange();
	BOOL m_bFlatScrollBars;
	CGridHeader m_header;
	CGridColumns m_columns;
	CGridRows m_rows;
	CGridFixed m_fixed;
	CImageList *m_pImageList;
	int m_nImageLeftIndent;
	int m_nImageTopIndent;
	BOOL m_bImageHighlight;
	int m_nImageIndexDef;
	CGridCell m_cellFocused;
	BOOL m_bCellSelected;
	CGridCell m_visOrigin;
	CGridCell m_visSize;

	BOOL m_bHideSelection;
	BOOL m_bShowHeader;
	BOOL m_bGridLines;
	int m_nGridLineWidth;
	UINT m_nGridStyles;
	UINT m_nCursorKeys;
	COLORREF m_gridColor;
	BOOL m_bEndEllipsis;
	BOOL m_bShowFocusRect;
	BOOL m_bAlwaysSelected;
	BOOL m_bRowSelect;
	BOOL m_bRightClickSelect;
	BOOL m_bAllowSelect;
	BOOL m_bFocusOnScroll;
	int m_nTextLeftIndent;
	int m_nTextRightIndent;
	int m_nTextTopIndent;
	CPoint m_hitTest;
	CGridCell m_clickPos;
	BOOL m_bColumnsSizing;
	BOOL m_bColumnsFullDrag;
	BOOL m_bColumnClick;
	BOOL m_bColResizing;
	CGridHeaderSection *m_pColResizeSection;
	int m_nColResizeIndex;
	int m_nColResizeOffset;
	CRect m_colResizeRect;
	int m_nColResizePos;
	int m_nColResizeMinWidth;
	int m_nColResizeMaxWidth;
	int m_nColResizeCount;
	CGridHeaderSection *m_pHeaderClickSection;
	CRect m_headerClickRect;
	BOOL m_bHeaderClickState;
	BOOL m_bHeaderClicking;
	int m_nUpdateLock;
	BOOL m_bAllowEdit;
	BOOL m_bAlwaysEdit;
	BOOL m_bReadOnly;
	CGridEdit *m_pEdit;
	CGridCell m_editCell;
	BOOL m_bEditing;
	BOOL m_bShowCellTips;
	CGridCell m_tipsCell;
	CString m_strTipsText;
	BOOL m_bCheckBoxes;
	GridCheckStyle m_checkStyle;
	int m_nCheckWidth;
	int m_nCheckHeight;
	int m_nCheckLeftIndent;
	int m_nCheckTopIndent;

	int m_nSortLeftIndent;
	int m_nSortTopIndent;
	BOOL m_bDoubleBuffered;
	CBitmap m_bitmapPattern;
	BOOL m_bCancelOnExit;
	BOOL m_bDefaultEditMenu;
	CFont m_font;

	//  For scroll bars
	UINT m_nPageStep;
	int m_nLineStep;
	int m_nLineSize;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIRTUALGRIDCTRL_H__201F6FEE_CCA2_4B13_BEE2_6EEEF5D2BEF4__INCLUDED_)
