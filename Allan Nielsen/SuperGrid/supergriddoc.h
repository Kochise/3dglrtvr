// SuperGridDoc.h : interface of the CSuperGridDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SUPERGRIDDOC_H__EC8E0221_199D_11D2_991C_002018026B76__INCLUDED_)
#define AFX_SUPERGRIDDOC_H__EC8E0221_199D_11D2_991C_002018026B76__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CSuperGridDoc : public CDocument
{
protected: // create from serialization only
	CSuperGridDoc();
	DECLARE_DYNCREATE(CSuperGridDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSuperGridDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSuperGridDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSuperGridDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUPERGRIDDOC_H__EC8E0221_199D_11D2_991C_002018026B76__INCLUDED_)
