#if !defined(AFX_CODEGENPAGE_H__DAC15B56_8A29_421F_AB62_CCF06081B32E__INCLUDED_)
#define AFX_CODEGENPAGE_H__DAC15B56_8A29_421F_AB62_CCF06081B32E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CodeGenPage.h : header file
//

#include "VirtualGridCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CCodeGenPage dialog

class CCodeGenPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CCodeGenPage)

// Construction
public:
	CString ComposeSectionTree(CGridHeaderSection *pSection);
	CCodeGenPage();
	~CCodeGenPage();

// Dialog Data
	//{{AFX_DATA(CCodeGenPage)
	enum { IDD = IDD_PROPPAGE_CODEGEN };
	CEdit	m_editRowCount;
	CSpinButtonCtrl	m_spinRowCount;
	CEdit	m_editCode;
	CString	m_strGridName;
	CString	m_strCode;
	BOOL	m_bAllowGridEdit;
	int		m_nRowCount;
	//}}AFX_DATA
	CVirtualGridCtrl *m_pGrid;


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCodeGenPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCodeGenPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonGenerate();
	afx_msg void OnButtonClear();
	afx_msg void OnButtonCopy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CODEGENPAGE_H__DAC15B56_8A29_421F_AB62_CCF06081B32E__INCLUDED_)
