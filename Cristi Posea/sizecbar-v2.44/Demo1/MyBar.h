#if !defined(__MYBAR_H__)
#define __MYBAR_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// mybar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyBar window

#ifndef baseCMyBar
#define baseCMyBar CSizingControlBarG
#endif

class CMyBar : public baseCMyBar
{
// Construction
public:
	CMyBar();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyBar();

protected:
	CEdit	m_wndChild;
	CFont	m_font;

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(__MYBAR_H__)
