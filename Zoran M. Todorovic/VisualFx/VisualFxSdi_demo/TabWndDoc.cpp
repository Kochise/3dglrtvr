// TabWndDoc.cpp : implementation of the CTabWndDoc class
//

#include "stdafx.h"
#include "TabWnd.h"
#include "MainFrm.h"

#include "TabWndDoc.h"
#include "FrameworkProperties.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabWndDoc

IMPLEMENT_DYNCREATE(CTabWndDoc, CDocument)

BEGIN_MESSAGE_MAP(CTabWndDoc, CDocument)
	//{{AFX_MSG_MAP(CTabWndDoc)
	ON_COMMAND(IDM_ENUMPANES, OnEnumPanes)
	ON_COMMAND(IDM_PROPERTIES, OnProperties)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabWndDoc construction/destruction

CTabWndDoc::CTabWndDoc()
{
	// TODO: add one-time construction code here

}

CTabWndDoc::~CTabWndDoc()
{
}

BOOL CTabWndDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CTabWndDoc serialization

void CTabWndDoc::Serialize(CArchive& ar)
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
// CTabWndDoc diagnostics

#ifdef _DEBUG
void CTabWndDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTabWndDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTabWndDoc commands

BOOL CTabWndDoc::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
  CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
  if (pFrame && pFrame->m_Framework.OnCmdMsg(nID,nCode,pExtra,pHandlerInfo))
    return TRUE;
	
	return CDocument::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CTabWndDoc::OnEnumPanes() 
{
  int nCount = ((CMainFrame*)AfxGetMainWnd())->m_Framework.GetCount();
  POSITION pos = GetFirstViewPosition();
  while (pos != NULL) {
    CView* pView = GetNextView(pos);
    ASSERT(pView != NULL);
  }
}

void CTabWndDoc::OnProperties() 
{
  CFrameworkProperties dialog(((CMainFrame*)AfxGetMainWnd())->m_Framework);
  dialog.DoModal();

}
