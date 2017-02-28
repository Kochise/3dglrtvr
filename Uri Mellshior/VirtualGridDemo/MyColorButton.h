#if !defined(AFX_COLORBUTTON_H__CAA5CFD3_0C49_43ED_B3C3_A614271F10B4__INCLUDED_)
#define AFX_COLORBUTTON_H__CAA5CFD3_0C49_43ED_B3C3_A614271F10B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyColorButton window

class CMyColorButton : public CButton
{
// Construction
public:
	CMyColorButton();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyColorButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	COLORREF GetColor();
	void SetColor(COLORREF crNewColor);
	virtual ~CMyColorButton();

	// Generated message map functions
protected:
	COLORREF m_crColor;
	//{{AFX_MSG(CMyColorButton)
	afx_msg void OnClicked();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORBUTTON_H__CAA5CFD3_0C49_43ED_B3C3_A614271F10B4__INCLUDED_)
