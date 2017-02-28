#if !defined(AFX_HEADERSPAGE_H__F001D4CC_5D35_47FA_AF6F_DD39D6EAD62E__INCLUDED_)
#define AFX_HEADERSPAGE_H__F001D4CC_5D35_47FA_AF6F_DD39D6EAD62E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HeadersPage.h : header file
//

#include "VirtualGridCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CHeadersPage dialog

class CHeadersPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CHeadersPage)

// Construction
public:
	void CheckModified();
	CGridHeaderSection * FindSection(HTREEITEM hItem, CGridHeaderSections *pSections);
	void FillSectionsTree(HTREEITEM hParentItem, CGridHeaderSection *pSection);
	CHeadersPage();
	~CHeadersPage();

// Dialog Data
	//{{AFX_DATA(CHeadersPage)
	enum { IDD = IDD_PROPPAGE_HEADERS };
	CTreeCtrl	m_treeHeaders;
	BOOL	m_bWordWrap;
	int		m_nAlignment;
	CString	m_strText;
	int		m_nWidth;
	int		m_nColumnIndex;
	//}}AFX_DATA
	CVirtualGridCtrl *m_pGrid;
	CGridHeaderSections *m_pSections;
	HTREEITEM m_hRootItem;


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CHeadersPage)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CHeadersPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangedTreeHeaders(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetdispinfoTreeHeaders(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusEditText();
	afx_msg void OnKillfocusEditWidth();
	afx_msg void OnKillfocusComboAlignment();
	afx_msg void OnChangeEditText();
	afx_msg void OnChangeEditWidth();
	afx_msg void OnSelchangeComboAlignment();
	afx_msg void OnCheckWordWrap();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonRemove();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HEADERSPAGE_H__F001D4CC_5D35_47FA_AF6F_DD39D6EAD62E__INCLUDED_)
