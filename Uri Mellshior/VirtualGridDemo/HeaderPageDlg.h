#if !defined(AFX_HEADERPAGEDLG_H__DE602492_A428_439E_8032_5CD4186CDBE6__INCLUDED_)
#define AFX_HEADERPAGEDLG_H__DE602492_A428_439E_8032_5CD4186CDBE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HeaderPageDlg.h : header file
//


class CVirtualGridCtrl;

/////////////////////////////////////////////////////////////////////////////
// CHeaderPageDlg dialog

class CHeaderPageDlg : public CDialog
{
// Construction
public:
	CHeaderPageDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHeaderPageDlg)
	enum { IDD = IDD_PAGE_HEADER };
	CSpinButtonCtrl	m_spinSectionHeight;
	BOOL	m_bAutoHeight;
	BOOL	m_bAutoSynchronize;
	BOOL	m_bFullSynchronizing;
	BOOL	m_bFlat;
	BOOL	m_bGridColor;
	int		m_nSectionHeight;
	//}}AFX_DATA
	CVirtualGridCtrl *m_pGrid;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHeaderPageDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHeaderPageDlg)
	afx_msg void OnCheckFlat();
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckAutoHeight();
	afx_msg void OnCheckAutoSynchronize();
	afx_msg void OnCheckFullSynchronizing();
	afx_msg void OnCheckGridColor();
	afx_msg void OnDeltaposSpinSectionHeight(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HEADERPAGEDLG_H__DE602492_A428_439E_8032_5CD4186CDBE6__INCLUDED_)
