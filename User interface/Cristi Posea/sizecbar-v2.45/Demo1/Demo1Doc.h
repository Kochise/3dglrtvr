// Demo1Doc.h : interface of the CDemo1Doc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEMO1DOC_H__7C36E658_7403_49A3_99D1_D84798465270__INCLUDED_)
#define AFX_DEMO1DOC_H__7C36E658_7403_49A3_99D1_D84798465270__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDemo1Doc : public CDocument
{
protected: // create from serialization only
	CDemo1Doc();
	DECLARE_DYNCREATE(CDemo1Doc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemo1Doc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDemo1Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDemo1Doc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMO1DOC_H__7C36E658_7403_49A3_99D1_D84798465270__INCLUDED_)
