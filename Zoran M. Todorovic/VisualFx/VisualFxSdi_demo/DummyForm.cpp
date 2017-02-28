// DummyForm.cpp : implementation file
//

#include "stdafx.h"
#include "tabwnd.h"
#include "DummyForm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDummyForm

IMPLEMENT_DYNCREATE(CDummyForm, TVisualFormView)

CDummyForm::CDummyForm()
	: TVisualFormView(CDummyForm::IDD)
{
	//{{AFX_DATA_INIT(CDummyForm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CDummyForm::~CDummyForm()
{
}

void CDummyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDummyForm)
	DDX_Control(pDX, IDC_EDIT, m_Edit);
	DDX_Control(pDX, IDC_COMBO, m_Combo);
	DDX_Control(pDX, IDC_LIST, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDummyForm, TVisualFormView)
	//{{AFX_MSG_MAP(CDummyForm)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDummyForm diagnostics

#ifdef _DEBUG
void CDummyForm::AssertValid() const
{
	CFormView::AssertValid();
}

void CDummyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDummyForm message handlers

void CDummyForm::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
  m_List.AddString(_T("Item A"));
  m_List.AddString(_T("Item B"));
  m_List.AddString(_T("Item C"));
  m_List.AddString(_T("Item D"));
  m_List.AddString(_T("Item E"));
  m_List.AddString(_T("Item F"));

	m_Combo.SetCurSel(0);
}
