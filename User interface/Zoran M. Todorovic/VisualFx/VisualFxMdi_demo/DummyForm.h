#if !defined(AFX_DUMMYFORM_H__1400D4B6_BDC3_11D3_8FAD_0080AD4311B1__INCLUDED_)
#define AFX_DUMMYFORM_H__1400D4B6_BDC3_11D3_8FAD_0080AD4311B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DummyForm.h : header file
//

#include "VisualFx.h"

/////////////////////////////////////////////////////////////////////////////
// CDummyForm form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CDummyForm : public TVisualFormView
{
protected:
	CDummyForm();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDummyForm)

// Form Data
public:
	//{{AFX_DATA(CDummyForm)
	enum { IDD = IDD_FORM };
	CEdit	m_Edit;
	CComboBox	m_Combo;
	CListBox	m_List;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDummyForm)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDummyForm();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CDummyForm)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DUMMYFORM_H__1400D4B6_BDC3_11D3_8FAD_0080AD4311B1__INCLUDED_)
