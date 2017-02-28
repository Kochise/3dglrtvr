#if !defined(AFX_CELLSETTINGSDLG_H__B08AE521_6404_423F_A050_4E757E40FE2C__INCLUDED_)
#define AFX_CELLSETTINGSDLG_H__B08AE521_6404_423F_A050_4E757E40FE2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CellSettingsDlg.h : header file
//

#include "MyColorButton.h"

/////////////////////////////////////////////////////////////////////////////
// CCellSettingsDlg dialog

class CCellSettingsDlg : public CDialog
{
// Construction
public:
	CCellSettingsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCellSettingsDlg)
	enum { IDD = IDD_DIALOG_CELL_SETTINS };
	CMyColorButton	m_buttonTextColor;
	CMyColorButton	m_buttonBkColor;
	CString	m_strCellText;
	//}}AFX_DATA
	COLORREF m_crBkColor;
	COLORREF m_crTextColor;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCellSettingsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCellSettingsDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CELLSETTINGSDLG_H__B08AE521_6404_423F_A050_4E757E40FE2C__INCLUDED_)
