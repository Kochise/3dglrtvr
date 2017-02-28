#if !defined(AFX_STYLESPAGEDLG_H__82F8C5B3_9FE0_4C35_ACB5_95F70C316084__INCLUDED_)
#define AFX_STYLESPAGEDLG_H__82F8C5B3_9FE0_4C35_ACB5_95F70C316084__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StylesPageDlg.h : header file
//

class CVirtualGridCtrl;

/////////////////////////////////////////////////////////////////////////////
// CStylesPageDlg dialog

class CStylesPageDlg : public CDialog
{
// Construction
public:
	CStylesPageDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CStylesPageDlg)
	enum { IDD = IDD_PAGE_STYLES };
	BOOL	m_bDotLines;
	BOOL	m_bFullHorzLine;
	BOOL	m_bFullVertLine;
	BOOL	m_bHorzLine;
	BOOL	m_bListView;
	BOOL	m_bVertLine;
	//}}AFX_DATA
	CVirtualGridCtrl *m_pGrid;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStylesPageDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStylesPageDlg)
	afx_msg void OnCheckHorLine();
	afx_msg void OnCheckVertLine();
	afx_msg void OnCheckFullHorzLine();
	afx_msg void OnCheckFullVertLine();
	afx_msg void OnCheckListView();
	afx_msg void OnCheckDotLines();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STYLESPAGEDLG_H__82F8C5B3_9FE0_4C35_ACB5_95F70C316084__INCLUDED_)
