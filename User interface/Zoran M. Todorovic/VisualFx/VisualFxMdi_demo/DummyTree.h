#if !defined(AFX_DUMMYTREE_H__A9AA1905_9687_11D2_899F_0040055A6A93__INCLUDED_)
#define AFX_DUMMYTREE_H__A9AA1905_9687_11D2_899F_0040055A6A93__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DummyTree.h : header file
//

#include <afxcview.h>

/////////////////////////////////////////////////////////////////////////////
// CDummyTree view

class CDummyTree : public CTreeView
{
protected:
	CDummyTree();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDummyTree)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDummyTree)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDummyTree();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CDummyTree)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DUMMYTREE_H__A9AA1905_9687_11D2_899F_0040055A6A93__INCLUDED_)
