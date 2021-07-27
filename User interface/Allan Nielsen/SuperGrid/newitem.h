#if !defined(AFX_NEWITEM_H__49AC6C51_1E92_11D2_9933_002018026B76__INCLUDED_)
#define AFX_NEWITEM_H__49AC6C51_1E92_11D2_9933_002018026B76__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// NewItem.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewItem dialog

class CNewItem : public CDialog
{
// Construction
public:
	CNewItem(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewItem)
	enum { IDD = IDD_DIALOG1 };
	CEdit	m_Edit;
	//}}AFX_DATA

	CString m_strItem;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewItem)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewItem)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWITEM_H__49AC6C51_1E92_11D2_9933_002018026B76__INCLUDED_)
