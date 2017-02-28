#if !defined(AFX_GRIDBUTTON_H__6B276AA0_88FF_4B80_9BAF_81142C325431__INCLUDED_)
#define AFX_GRIDBUTTON_H__6B276AA0_88FF_4B80_9BAF_81142C325431__INCLUDED_

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
// GridButton.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CGridButton window

class CGridButton : public CButton  {
	DECLARE_DYNCREATE(CGridButton)
// Construction
public:
	CGridButton();

// Attributes
public:
	BOOL m_bPressed;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGridButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGridButton)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDBUTTON_H__6B276AA0_88FF_4B80_9BAF_81142C325431__INCLUDED_)
