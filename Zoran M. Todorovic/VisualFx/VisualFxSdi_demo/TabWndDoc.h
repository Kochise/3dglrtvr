// TabWndDoc.h : interface of the CTabWndDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABWNDDOC_H__5D77F19E_9685_11D2_899F_0040055A6A93__INCLUDED_)
#define AFX_TABWNDDOC_H__5D77F19E_9685_11D2_899F_0040055A6A93__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTabWndDoc : public CDocument
{
protected: // create from serialization only
	CTabWndDoc();
	DECLARE_DYNCREATE(CTabWndDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabWndDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTabWndDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTabWndDoc)
	afx_msg void OnEnumPanes();
	afx_msg void OnProperties();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABWNDDOC_H__5D77F19E_9685_11D2_899F_0040055A6A93__INCLUDED_)
