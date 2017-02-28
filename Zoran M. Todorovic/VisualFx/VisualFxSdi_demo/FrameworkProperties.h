#if !defined(AFX_FRAMEWORKPROPERTIES_H__833BCAC6_C1AC_11D3_8FB4_0080AD4311B1__INCLUDED_)
#define AFX_FRAMEWORKPROPERTIES_H__833BCAC6_C1AC_11D3_8FB4_0080AD4311B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FrameworkProperties.h : header file
//

class TVisualFramework;

/////////////////////////////////////////////////////////////////////////////
// CFrameworkProperties dialog

class CFrameworkProperties : public CDialog
{
private:
  TVisualFramework *m_pFx;
// Construction
public:
	CFrameworkProperties(TVisualFramework& fx, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFrameworkProperties)
	enum { IDD = IDD_PROPERTIES };
	CButton	m_VisibleTab;
	CButton	m_EnableTab;
	CButton	m_Enable;
	CListBox	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFrameworkProperties)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFrameworkProperties)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeList();
	afx_msg void OnEnable();
	afx_msg void OnEnabletab();
	afx_msg void OnVisibletab();
	afx_msg void OnSet();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRAMEWORKPROPERTIES_H__833BCAC6_C1AC_11D3_8FB4_0080AD4311B1__INCLUDED_)
