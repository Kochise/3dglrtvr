// TabWnd.h : main header file for the TABWND application
//

#if !defined(AFX_TABWND_H__7E5B3234_BEAF_11D3_8FAE_0080AD4311B1__INCLUDED_)
#define AFX_TABWND_H__7E5B3234_BEAF_11D3_8FAE_0080AD4311B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CTabWndApp:
// See TabWnd.cpp for the implementation of this class
//

class CTabWndApp : public CWinApp
{
public:
	CTabWndApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabWndApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CTabWndApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABWND_H__7E5B3234_BEAF_11D3_8FAE_0080AD4311B1__INCLUDED_)
