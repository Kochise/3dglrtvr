// VirtualGridDemo.h : main header file for the VIRTUALGRIDDEMO application
//

#if !defined(AFX_VIRTUALGRIDDEMO_H__10AD4AA2_2F2D_425E_8D9B_D0EAE2DE3AB5__INCLUDED_)
#define AFX_VIRTUALGRIDDEMO_H__10AD4AA2_2F2D_425E_8D9B_D0EAE2DE3AB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CVirtualGridDemoApp:
// See VirtualGridDemo.cpp for the implementation of this class
//

class CVirtualGridDemoApp : public CWinApp
{
public:
	CVirtualGridDemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVirtualGridDemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CVirtualGridDemoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIRTUALGRIDDEMO_H__10AD4AA2_2F2D_425E_8D9B_D0EAE2DE3AB5__INCLUDED_)
