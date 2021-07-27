// TabWndView.h : interface of the CTabWndView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABWNDVIEW_H__7E5B323E_BEAF_11D3_8FAE_0080AD4311B1__INCLUDED_)
#define AFX_TABWNDVIEW_H__7E5B323E_BEAF_11D3_8FAE_0080AD4311B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTabWndView : public CView
{
protected: // create from serialization only
	CTabWndView();
	DECLARE_DYNCREATE(CTabWndView)

// Attributes
public:
	CTabWndDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabWndView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTabWndView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTabWndView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in TabWndView.cpp
inline CTabWndDoc* CTabWndView::GetDocument()
   { return (CTabWndDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABWNDVIEW_H__7E5B323E_BEAF_11D3_8FAE_0080AD4311B1__INCLUDED_)
