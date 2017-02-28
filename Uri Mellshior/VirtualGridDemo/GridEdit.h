#if !defined(AFX_GRIDEDIT_H__94D600EE_A003_4E90_B877_9F9F64B47DF3__INCLUDED_)
#define AFX_GRIDEDIT_H__94D600EE_A003_4E90_B877_9F9F64B47DF3__INCLUDED_

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


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GridEdit.h : header file
//


#include "GridButton.h"
#include "GridListBox.h"

//  ----
enum GridEditStyle {geSimple, geEllipsis, gePickList, geDataList, geUserDefine};

class CVirtualGridCtrl;


/////////////////////////////////////////////////////////////////////////////
// CGridEdit window

class CGridEdit : public CEdit  {
	DECLARE_DYNCREATE(CGridEdit)
	friend class CVirtualGridCtrl;
	friend class CGridButton;
	friend class CGridListBox;
// Construction
public:
	CGridEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual BOOL EditCanModify();
	void Show();
	void SelectNext();
	void Press();
	void Hide();
	void DropDown();
	void Deselect();
	virtual void CloseUp(BOOL bAccept);
	void SetWordWrap(BOOL bWordWrap);
	void SetEditStyle(GridEditStyle style);
	void SetDropListVisible(BOOL bVisible);
	void SetButtonWidth(int nWidth);
	int GetLineCount();
	CRect GetButtonRect();
	virtual ~CGridEdit();

	// Generated message map functions
protected:
	virtual void UpdateListBounds();
	virtual void UpdateListItems();
	virtual void UpdateStyle();
	virtual void UpdateListValue(BOOL bAccept);
	virtual void UpdateContents();
	virtual void UpdateColors();
	virtual void UpdateBounds(BOOL bScrollCaret);
	virtual void CreateButton();
	virtual CWnd * GetDropList();
	//{{AFX_MSG(CGridEdit)
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	void ListLButtonUp(CPoint point);
	CVirtualGridCtrl *m_pGrid;
	int m_nClickTime;
	GridEditStyle m_editStyle;
	BOOL m_bWantReturns;
	BOOL m_bWordWrap;
	int m_nDropDownCount;
	BOOL m_bDropListVisible;
	CWnd *m_pActiveList;
	int m_nButtonWidth;
	BOOL m_bButtonPressed;
	BOOL m_bDefocusing;
	int m_nAlignment;	//  LVCFMT_LEFT, LVCFMT_RIGHT, LVCFMT_CENTER
	int m_nMaxLength;
	BOOL m_bReadOnly;
	CGridButton m_button;
	CGridListBox *m_pPickList;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDEDIT_H__94D600EE_A003_4E90_B877_9F9F64B47DF3__INCLUDED_)
