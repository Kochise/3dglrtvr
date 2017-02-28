// VirtualGridDemoDlg.h : header file
//

#if !defined(AFX_VIRTUALGRIDDEMODLG_H__072AB3E8_3A4F_4773_BC8C_4619B6BCF120__INCLUDED_)
#define AFX_VIRTUALGRIDDEMODLG_H__072AB3E8_3A4F_4773_BC8C_4619B6BCF120__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VirtualGridCtrl.h"
#include "RollupCtrl.h"



//  ---------------  Used only in demo  ---------
class CDemoGridRow  {
public:
	CDemoGridRow(LPCTSTR lpPointName = _T(""), int x = 0, int y = 0, int z = 0)
	{
		m_strPointName = lpPointName;
		m_x = x;
		m_y = y;
		m_z = z;

		m_strText = _T("Double-click me");
		m_crBkColor = ::GetSysColor(COLOR_WINDOW);
		m_crTextColor = ::GetSysColor(COLOR_WINDOWTEXT);

		m_bAcceptCursor = TRUE;
		m_bReadOnly = FALSE;
		m_bChecked = FALSE;
	}

	CString m_strPointName;

	//  Position data
	int m_x;
	int m_y;
	int m_z;

	//  Additional demo data
	CString m_strText;
	COLORREF m_crBkColor;
	COLORREF m_crTextColor;
	BOOL m_bAcceptCursor;
	BOOL m_bReadOnly;

	//  -------  NEW  --------
	BOOL m_bChecked;
};



/////////////////////////////////////////////////////////////////////////////
// CVirtualGridDemoDlg dialog

class CVirtualGridDemoDlg : public CDialog
{
// Construction
public:
	CVirtualGridDemoDlg(CWnd* pParent = NULL);	// standard constructor
	CVirtualGridCtrl m_grid;
	CRollupCtrl m_rollup;
	CImageList m_images;
	CArray<CDemoGridRow, CDemoGridRow> m_demoData;

// Dialog Data
	//{{AFX_DATA(CVirtualGridDemoDlg)
	enum { IDD = IDD_VIRTUALGRIDDEMO_DIALOG };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVirtualGridDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	afx_msg void OnGridGetCheckState(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnGridGetDispInfo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnGridSetDispInfo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnGridEditButtonPress(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnGridCheckClick(NMHDR *pNMHDR, LRESULT *pResult);

	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CVirtualGridDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnExit();
	afx_msg void OnButtonHeadConstruct();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIRTUALGRIDDEMODLG_H__072AB3E8_3A4F_4773_BC8C_4619B6BCF120__INCLUDED_)
