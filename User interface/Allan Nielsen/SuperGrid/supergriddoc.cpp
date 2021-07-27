// SuperGridDoc.cpp : implementation of the CSuperGridDoc class
//

#include "stdafx.h"
#include "SuperGrid.h"

#include "SuperGridDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSuperGridDoc

IMPLEMENT_DYNCREATE(CSuperGridDoc, CDocument)

BEGIN_MESSAGE_MAP(CSuperGridDoc, CDocument)
	//{{AFX_MSG_MAP(CSuperGridDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSuperGridDoc construction/destruction

CSuperGridDoc::CSuperGridDoc()
{
	// TODO: add one-time construction code here

}

CSuperGridDoc::~CSuperGridDoc()
{
}

BOOL CSuperGridDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CSuperGridDoc serialization

void CSuperGridDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CSuperGridDoc diagnostics

#ifdef _DEBUG
void CSuperGridDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSuperGridDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSuperGridDoc commands
