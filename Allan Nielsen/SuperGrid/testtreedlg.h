#if !defined(AFX_TESTTREEDLG_H__C6DF171C_806D_11D2_9A94_002018026B76__INCLUDED_)
#define AFX_TESTTREEDLG_H__C6DF171C_806D_11D2_9A94_002018026B76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TestTreeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTestTreeDlg dialog
#include "MySuperGrid.h"


class CTestTreeDlg : public CDialog
{
// Construction
public:
	CTestTreeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTestTreeDlg)
	enum { IDD = IDD_DIALOG_SUPERGRID };
	CStatic	m_Frame;
	//}}AFX_DATA
	CMySuperGrid m_List;
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestTreeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTestTreeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonInsert();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTTREEDLG_H__C6DF171C_806D_11D2_9A94_002018026B76__INCLUDED_)
