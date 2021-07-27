#if !defined(AFX_FIXEDPAGEDLG_H__0E36DD5B_CABD_4B4A_83DE_DF8DF859E53B__INCLUDED_)
#define AFX_FIXEDPAGEDLG_H__0E36DD5B_CABD_4B4A_83DE_DF8DF859E53B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FixedPageDlg.h : header file
//

class CVirtualGridCtrl;

/////////////////////////////////////////////////////////////////////////////
// CFixedPageDlg dialog

class CFixedPageDlg : public CDialog
{
// Construction
public:
	CFixedPageDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFixedPageDlg)
	enum { IDD = IDD_PAGE_FIXED };
	CSpinButtonCtrl	m_spinCount;
	BOOL	m_bFlat;
	BOOL	m_bGridColor;
	int		m_nCount;
	//}}AFX_DATA
	CVirtualGridCtrl *m_pGrid;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFixedPageDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFixedPageDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposSpinCount(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckFlat();
	afx_msg void OnCheckGridColor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FIXEDPAGEDLG_H__0E36DD5B_CABD_4B4A_83DE_DF8DF859E53B__INCLUDED_)
