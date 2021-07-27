#if !defined(AFX_MDIFRAMES_H__07DE5383_C0DC_11D3_8FB3_0080AD4311B1__INCLUDED_)
#define AFX_MDIFRAMES_H__07DE5383_C0DC_11D3_8FB3_0080AD4311B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MdiFrames.h : header file
//

#include "VisualFx.h"

/////////////////////////////////////////////////////////////////////////////
// CSimpleViewFrame frame

class CSimpleViewFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CSimpleViewFrame)
protected:
	CSimpleViewFrame();           // protected constructor used by dynamic creation

// Attributes
public:
  TVisualFramework m_Framework;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSimpleViewFrame)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CSimpleViewFrame();

	// Generated message map functions
	//{{AFX_MSG(CSimpleViewFrame)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CSplitterFrame frame

class CSplitterFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CSplitterFrame)
protected:
	CSplitterFrame();           // protected constructor used by dynamic creation

// Attributes
public:
  TVisualFramework m_Framework;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSplitterFrame)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CSplitterFrame();

	// Generated message map functions
	//{{AFX_MSG(CSplitterFrame)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CSimpleTabFrame frame

class CSimpleTabFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CSimpleTabFrame)
protected:
	CSimpleTabFrame();           // protected constructor used by dynamic creation

// Attributes
public:
  TVisualFramework m_Framework;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSimpleTabFrame)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CSimpleTabFrame();

	// Generated message map functions
	//{{AFX_MSG(CSimpleTabFrame)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CSplitterTabFrame frame

class CSplitterTabFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CSplitterTabFrame)
protected:
	CSplitterTabFrame();           // protected constructor used by dynamic creation

// Attributes
public:
  TVisualFramework m_Framework;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSplitterTabFrame)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CSplitterTabFrame();

	// Generated message map functions
	//{{AFX_MSG(CSplitterTabFrame)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CTabSplitterFrame frame

class CTabSplitterFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CTabSplitterFrame)
protected:
	CTabSplitterFrame();           // protected constructor used by dynamic creation

// Attributes
public:
  TVisualFramework m_Framework;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabSplitterFrame)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CTabSplitterFrame();

	// Generated message map functions
	//{{AFX_MSG(CTabSplitterFrame)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CComplexFrameA frame

class CComplexFrameA : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CComplexFrameA)
protected:
	CComplexFrameA();           // protected constructor used by dynamic creation

// Attributes
public:
  TVisualFramework m_Framework;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComplexFrameA)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CComplexFrameA();

	// Generated message map functions
	//{{AFX_MSG(CComplexFrameA)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CComplexFrameB frame

class CComplexFrameB : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CComplexFrameB)
protected:
	CComplexFrameB();           // protected constructor used by dynamic creation

// Attributes
public:
  TVisualFramework m_Framework;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComplexFrameB)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CComplexFrameB();

	// Generated message map functions
	//{{AFX_MSG(CComplexFrameB)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CComplexFrameC frame

class CComplexFrameC : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CComplexFrameC)
protected:
	CComplexFrameC();           // protected constructor used by dynamic creation

// Attributes
public:
  TVisualFramework m_Framework;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComplexFrameC)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CComplexFrameC();

	// Generated message map functions
	//{{AFX_MSG(CComplexFrameC)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CComplexFrameD frame

class CComplexFrameD : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CComplexFrameD)
protected:
	CComplexFrameD();           // protected constructor used by dynamic creation

// Attributes
public:
  TVisualFramework m_Framework;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComplexFrameD)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CComplexFrameD();

	// Generated message map functions
	//{{AFX_MSG(CComplexFrameD)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MDIFRAMES_H__07DE5383_C0DC_11D3_8FB3_0080AD4311B1__INCLUDED_)
