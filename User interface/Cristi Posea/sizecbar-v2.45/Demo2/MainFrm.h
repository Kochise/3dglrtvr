// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__6FD01C79_B1B2_4CA8_82F6_47FEC71DF867__INCLUDED_)
#define AFX_MAINFRM_H__6FD01C79_B1B2_4CA8_82F6_47FEC71DF867__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyBar.h"

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL DestroyWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	BOOL VerifyBarState(LPCTSTR lpszProfileName);

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
    CToolBar    m_wndToolBar2;
	
    CMyBar		m_wndMyBars[4];
	
    baseCMyBar	m_wndInstantBar; // instant bar
	CTreeCtrl	m_wndIBTree; // instant bar child

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewMytoolbar();
	afx_msg void OnUpdateViewMytoolbar(CCmdUI* pCmdUI);
	afx_msg void OnViewInstant();
	afx_msg void OnUpdateViewInstant(CCmdUI* pCmdUI);
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	afx_msg void OnViewMy(UINT nID);
	afx_msg void OnUpdateViewMy(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__6FD01C79_B1B2_4CA8_82F6_47FEC71DF867__INCLUDED_)
