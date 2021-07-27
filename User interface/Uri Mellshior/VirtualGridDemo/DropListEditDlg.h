#if !defined(AFX_DROPLISTEDITDLG_H__F6AA4A67_22B4_4304_A92E_E7B71C34EE13__INCLUDED_)
#define AFX_DROPLISTEDITDLG_H__F6AA4A67_22B4_4304_A92E_E7B71C34EE13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DropListEditDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDropListEditDlg dialog

class CDropListEditDlg : public CDialog
{
// Construction
public:
	CDropListEditDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDropListEditDlg)
	enum { IDD = IDD_DROP_LIST_EDITOR };
	CEdit	m_editDropList;
	CString	m_strDropList;
	//}}AFX_DATA
	CStringArray m_pickList;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDropListEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDropListEditDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DROPLISTEDITDLG_H__F6AA4A67_22B4_4304_A92E_E7B71C34EE13__INCLUDED_)
