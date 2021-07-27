// SuperGrid.h : main header file for the SUPERGRID application
//

#if !defined(AFX_SUPERGRID_H__EC8E021B_199D_11D2_991C_002018026B76__INCLUDED_)
#define AFX_SUPERGRID_H__EC8E021B_199D_11D2_991C_002018026B76__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CSuperGridApp:
// See SuperGrid.cpp for the implementation of this class
//

class CSuperGridApp : public CWinApp
{
public:
	CSuperGridApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSuperGridApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSuperGridApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUPERGRID_H__EC8E021B_199D_11D2_991C_002018026B76__INCLUDED_)
