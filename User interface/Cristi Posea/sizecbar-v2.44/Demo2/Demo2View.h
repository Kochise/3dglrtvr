// Demo2View.h : interface of the CDemo2View class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEMO2VIEW_H__9A9D0B60_94E7_40A9_8186_6F9B2F857863__INCLUDED_)
#define AFX_DEMO2VIEW_H__9A9D0B60_94E7_40A9_8186_6F9B2F857863__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDemo2View : public CEditView
{
protected: // create from serialization only
	CDemo2View();
	DECLARE_DYNCREATE(CDemo2View)

// Attributes
public:
	CDemo2Doc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemo2View)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDemo2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDemo2View)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in Demo2View.cpp
inline CDemo2Doc* CDemo2View::GetDocument()
   { return (CDemo2Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMO2VIEW_H__9A9D0B60_94E7_40A9_8186_6F9B2F857863__INCLUDED_)
