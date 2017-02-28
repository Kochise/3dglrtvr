#if !defined(AFX_ROWSPAGEDLG_H__A0DF76BC_F427_441A_A777_21AF1E24F928__INCLUDED_)
#define AFX_ROWSPAGEDLG_H__A0DF76BC_F427_441A_A777_21AF1E24F928__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RowsPageDlg.h : header file
//

#include "VirtualGridDemoDlg.h"


/////////////////////////////////////////////////////////////////////////////
// CRowsPageDlg dialog

class CRowsPageDlg : public CDialog
{
// Construction
public:
	CRowsPageDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRowsPageDlg)
	enum { IDD = IDD_PAGE_ROWS };
	CSpinButtonCtrl	m_spinHeight;
	CSpinButtonCtrl	m_spinCount;
	BOOL	m_bAutoHeight;
	int		m_nCount;
	int		m_nHeight;
	//}}AFX_DATA
	CVirtualGridCtrl *m_pGrid;
	CArray<CDemoGridRow, CDemoGridRow> *m_pDemoData;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRowsPageDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRowsPageDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposSpinCount(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinHeight(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckAutoHeight();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROWSPAGEDLG_H__A0DF76BC_F427_441A_A777_21AF1E24F928__INCLUDED_)
