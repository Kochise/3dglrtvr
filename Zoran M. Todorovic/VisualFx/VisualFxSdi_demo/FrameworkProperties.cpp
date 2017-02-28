// FrameworkProperties.cpp : implementation file
//

#include "stdafx.h"
#include "tabwnd.h"
#include "VisualFx.h"
#include "FrameworkProperties.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFrameworkProperties dialog


CFrameworkProperties::CFrameworkProperties(TVisualFramework& fx, 
                                           CWnd* pParent /*=NULL*/)
	: CDialog(CFrameworkProperties::IDD, pParent)
{
  m_pFx = &fx;
	//{{AFX_DATA_INIT(CFrameworkProperties)
	//}}AFX_DATA_INIT
}


void CFrameworkProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFrameworkProperties)
	DDX_Control(pDX, IDC_VISIBLETAB, m_VisibleTab);
	DDX_Control(pDX, IDC_ENABLETAB, m_EnableTab);
	DDX_Control(pDX, IDC_ENABLE, m_Enable);
	DDX_Control(pDX, IDC_LIST, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFrameworkProperties, CDialog)
	//{{AFX_MSG_MAP(CFrameworkProperties)
	ON_LBN_SELCHANGE(IDC_LIST, OnSelchangeList)
	ON_BN_CLICKED(IDC_ENABLE, OnEnable)
	ON_BN_CLICKED(IDC_ENABLETAB, OnEnabletab)
	ON_BN_CLICKED(IDC_VISIBLETAB, OnVisibletab)
	ON_BN_CLICKED(IDC_SET, OnSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFrameworkProperties message handlers

BOOL CFrameworkProperties::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
  int nIndex;
  TVisualFrameworkIterator it(*m_pFx);
  while (!it.End()) {
    TVisualObject *pObject = it.Get();
    if (pObject->CanFocus()) {
      nIndex = m_List.AddString(pObject->GetDescription());
      m_List.SetItemData(nIndex, (DWORD)pObject);
    }
    it ++;
  }
  m_List.SetCurSel(0);
	
  OnSelchangeList();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFrameworkProperties::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CFrameworkProperties::OnSelchangeList() 
{
  int nIndex = m_List.GetCurSel();
  if (nIndex > -1) {
    TVisualObject  *pObject = (TVisualObject*)m_List.GetItemData(nIndex);
    if (pObject->IsTabPane()) {
      m_VisibleTab.EnableWindow(TRUE);
      m_EnableTab.EnableWindow(TRUE);
      BOOL bFlag;
      pObject->IsTabVisible(bFlag);
      m_VisibleTab.SetCheck(bFlag ? 1 : 0);
      pObject->IsTabEnabled(bFlag);
      m_EnableTab.SetCheck(bFlag ? 1 : 0);
    } else {
      m_VisibleTab.EnableWindow(FALSE);
      m_EnableTab.EnableWindow(FALSE);
    }
    BOOL bEnabled;
    pObject->IsEnabled(bEnabled);
    m_Enable.SetCheck(bEnabled ? 1 : 0);
  }
}

void CFrameworkProperties::OnSet() 
{
  int nIndex = m_List.GetCurSel();
  if (nIndex > -1) {
    TVisualObject  *pObject = (TVisualObject*)m_List.GetItemData(nIndex);
    m_pFx->SetActivePane(pObject);
    OnSelchangeList();
  }
  
}

void CFrameworkProperties::OnEnable() 
{
  int nIndex = m_List.GetCurSel();
  if (nIndex > -1) {
    TVisualObject  *pObject = (TVisualObject*)m_List.GetItemData(nIndex);
    m_pFx->Enable(pObject, m_Enable.GetCheck() ? TRUE : FALSE);
    OnSelchangeList();
  }
}

void CFrameworkProperties::OnEnabletab() 
{
  int nIndex = m_List.GetCurSel();
  if (nIndex > -1) {
    TVisualObject  *pObject = (TVisualObject*)m_List.GetItemData(nIndex);
    m_pFx->EnableTab(pObject, m_EnableTab.GetCheck() ? TRUE : FALSE);
    OnSelchangeList();
  }
}

void CFrameworkProperties::OnVisibletab() 
{
  int nIndex = m_List.GetCurSel();
  if (nIndex > -1) {
    TVisualObject  *pObject = (TVisualObject*)m_List.GetItemData(nIndex);
    m_pFx->ShowTab(pObject, m_VisibleTab.GetCheck() ? TRUE : FALSE);
    OnSelchangeList();
  }
}
