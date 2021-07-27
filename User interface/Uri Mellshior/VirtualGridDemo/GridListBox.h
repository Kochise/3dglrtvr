#if !defined(AFX_GRIDLISTBOX_H__4588E9B7_CB21_4014_B653_F1DED9AAB9E2__INCLUDED_)
#define AFX_GRIDLISTBOX_H__4588E9B7_CB21_4014_B653_F1DED9AAB9E2__INCLUDED_

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


#include "GridButton.h"


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GridListBox.h : header file
//

class CVirtualGridCtrl;


/////////////////////////////////////////////////////////////////////////////
// CGridListBox window

class CGridListBox : public CListBox  {
	DECLARE_DYNCREATE(CGridListBox)
	friend class CGridEdit;
// Construction
public:
	CGridListBox();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridListBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGridListBox();

	// Generated message map functions
protected:
	int m_nLeft;
	int m_nTop;
	int m_nWidth;
	int m_nHeight;
	//{{AFX_MSG(CGridListBox)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	CVirtualGridCtrl *m_pGrid;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDLISTBOX_H__4588E9B7_CB21_4014_B653_F1DED9AAB9E2__INCLUDED_)
