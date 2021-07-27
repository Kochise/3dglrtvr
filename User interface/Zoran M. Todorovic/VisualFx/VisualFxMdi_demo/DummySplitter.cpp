// DummySplitter.cpp : implementation file
//

#include "stdafx.h"
#include "DummySplitter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDummySplitter

CDummySplitter::CDummySplitter()
{
}

CDummySplitter::~CDummySplitter()
{
}


BEGIN_MESSAGE_MAP(CDummySplitter, CSplitterWnd)
	//{{AFX_MSG_MAP(CDummySplitter)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDummySplitter::SetActivePane( int row, int col, CWnd* pWnd)
{

  CSplitterWnd::SetActivePane(row,col,pWnd);
}

/////////////////////////////////////////////////////////////////////////////
// CDummySplitter message handlers
