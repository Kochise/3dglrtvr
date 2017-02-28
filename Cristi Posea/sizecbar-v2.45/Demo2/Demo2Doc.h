// Demo2Doc.h : interface of the CDemo2Doc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEMO2DOC_H__B0BBD807_E676_41E3_9D3D_E3CC5CA2A0A8__INCLUDED_)
#define AFX_DEMO2DOC_H__B0BBD807_E676_41E3_9D3D_E3CC5CA2A0A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDemo2Doc : public CDocument
{
protected: // create from serialization only
	CDemo2Doc();
	DECLARE_DYNCREATE(CDemo2Doc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemo2Doc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDemo2Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDemo2Doc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMO2DOC_H__B0BBD807_E676_41E3_9D3D_E3CC5CA2A0A8__INCLUDED_)
