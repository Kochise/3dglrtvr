#if !defined(AFX_GENERALPAGEDLG_H__90414173_4E8B_4174_8EFF_ACE31FFC5B72__INCLUDED_)
#define AFX_GENERALPAGEDLG_H__90414173_4E8B_4174_8EFF_ACE31FFC5B72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GeneralPageDlg.h : header file
//


class CVirtualGridCtrl;


/////////////////////////////////////////////////////////////////////////////
// CGeneralPageDlg dialog

class CGeneralPageDlg : public CDialog
{
// Construction
public:
	CGeneralPageDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGeneralPageDlg)
	enum { IDD = IDD_PAGE_GENERAL };
	BOOL	m_bAllowEdit;
	BOOL	m_bAllowSelect;
	BOOL	m_bAlwaysEdit;
	BOOL	m_bAlwaysSelected;
	BOOL	m_bCancelOnExit;
	BOOL	m_bCheckBoxes;
	int	m_nCheckStyle;
	BOOL	m_bRowSelect;
	BOOL	m_bReadOnly;
	BOOL	m_bDoubleBuffered;
	BOOL	m_bGridLines;
	//}}AFX_DATA
	CVirtualGridCtrl *m_pGrid;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGeneralPageDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGeneralPageDlg)
	afx_msg void OnCheckAllowEdit();
	afx_msg void OnCheckRowSelect();
	afx_msg void OnCheckAllowSelect();
	afx_msg void OnCheckAlwaysEdit();
	afx_msg void OnCheckAlwaysSelected();
	afx_msg void OnCheckCancelOnExit();
	afx_msg void OnCheckCheckBoxes();
	afx_msg void OnSelchangeComboCheckStyle();
	afx_msg void OnCheckReadOnly();
	afx_msg void OnCheckDoubleBuff();
	afx_msg void OnCheckGridLines();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GENERALPAGEDLG_H__90414173_4E8B_4174_8EFF_ACE31FFC5B72__INCLUDED_)
