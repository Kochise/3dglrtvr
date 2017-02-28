#if !defined(AFX_GRIDPROPERTIESSHEET_H__4C5D8673_8907_4B70_A717_929D9053E79D__INCLUDED_)
#define AFX_GRIDPROPERTIESSHEET_H__4C5D8673_8907_4B70_A717_929D9053E79D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GridPropertiesSheet.h : header file
//

#include "VirtualGridCtrl.h"
#include "ColumnsPage.h"
#include "HeadersPage.h"
#include "CodeGenPage.h"


/////////////////////////////////////////////////////////////////////////////
// CGridPropertiesSheet

class CGridPropertiesSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CGridPropertiesSheet)

// Construction
public:
	CGridPropertiesSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CGridPropertiesSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:
	CColumnsPage m_columnsPage;
	CHeadersPage m_headersPage;
	CCodeGenPage m_codeGenPage;
	CVirtualGridCtrl *m_pGrid;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridPropertiesSheet)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGridPropertiesSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGridPropertiesSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDPROPERTIESSHEET_H__4C5D8673_8907_4B70_A717_929D9053E79D__INCLUDED_)
