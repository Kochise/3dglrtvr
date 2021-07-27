#if !defined(AFX_DUMMYSPLITTER_H__1400D4B7_BDC3_11D3_8FAD_0080AD4311B1__INCLUDED_)
#define AFX_DUMMYSPLITTER_H__1400D4B7_BDC3_11D3_8FAD_0080AD4311B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DummySplitter.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDummySplitter view

class CDummySplitter : public CSplitterWnd 
{
public:
	CDummySplitter();           // protected constructor used by dynamic creation
	virtual ~CDummySplitter();

// Attributes
public:

// Operations
public:

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDummySplitter)
  virtual void SetActivePane( int row, int col, CWnd* pWnd = NULL );
  //}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CDummySplitter)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DUMMYEDIT_H__1400D4B7_BDC3_11D3_8FAD_0080AD4311B1__INCLUDED_)
