#if !defined(AFX_COLUMNSPAGE_H__E0A859AB_9D00_4B2E_8C73_3661F7835642__INCLUDED_)
#define AFX_COLUMNSPAGE_H__E0A859AB_9D00_4B2E_8C73_3661F7835642__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColumnsPage.h : header file
//

#include "VirtualGridCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CColumnsPage dialog

class CColumnsPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CColumnsPage)

// Construction
public:
	void CheckModified();
	CColumnsPage();
	~CColumnsPage();

// Dialog Data
	//{{AFX_DATA(CColumnsPage)
	enum { IDD = IDD_PROPPAGE_COLUMNS };
	CButton	m_buttonPickList;
	CListCtrl	m_listColumns;
	int		m_nAlignment;
	BOOL	m_bFixedWidth;
	BOOL	m_bInvisible;
	BOOL	m_bMultiline;
	BOOL	m_bReadOnly;
	CString	m_strCaption;
	int		m_nWidth;
	BOOL	m_bTabStop;
	BOOL	m_bAlignEdit;
	BOOL	m_bAllowClick;
	BOOL	m_bWantReturns;
	int		m_nEditStyle;
	int		m_nMaxWidth;
	int		m_nMinWidth;
	int		m_nCheckKind;
	//}}AFX_DATA
	CGridColumns m_columns;
	CVirtualGridCtrl *m_pGrid;


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CColumnsPage)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CColumnsPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnGetdispinfoListColumns(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonAdd();
	afx_msg void OnItemchangedListColumns(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonRemove();
	afx_msg void OnChangeEditCaption();
	afx_msg void OnChangeEditWidth();
	afx_msg void OnSelchangeComboAlignment();
	afx_msg void OnCheckFixedWidth();
	afx_msg void OnCheckMultiline();
	afx_msg void OnCheckReadOnly();
	afx_msg void OnCheckInvisible();
	afx_msg void OnCheckAlignEdit();
	afx_msg void OnCheckAllowClick();
	afx_msg void OnCheckTabStop();
	afx_msg void OnCheckWantReturns();
	afx_msg void OnChangeEditMaxWidth();
	afx_msg void OnChangeEditMinWidth();
	afx_msg void OnSelchangeComboEditStyle();
	afx_msg void OnButtonPickList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLUMNSPAGE_H__E0A859AB_9D00_4B2E_8C73_3661F7835642__INCLUDED_)
