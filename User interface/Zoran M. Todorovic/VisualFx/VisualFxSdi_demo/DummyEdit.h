#if !defined(AFX_DUMMYEDIT_H__1400D4B7_BDC3_11D3_8FAD_0080AD4311B1__INCLUDED_)
#define AFX_DUMMYEDIT_H__1400D4B7_BDC3_11D3_8FAD_0080AD4311B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DummyEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDummyEdit view

class CDummyEdit : public CEditView
{
protected:
	CDummyEdit();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDummyEdit)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDummyEdit)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDummyEdit();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CDummyEdit)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DUMMYEDIT_H__1400D4B7_BDC3_11D3_8FAD_0080AD4311B1__INCLUDED_)
