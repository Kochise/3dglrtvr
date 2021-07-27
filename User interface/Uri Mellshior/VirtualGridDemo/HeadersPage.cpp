// HeadersPage.cpp : implementation file
//

#include "stdafx.h"
#include "VirtualGridDemo.h"
#include "HeadersPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHeadersPage property page

IMPLEMENT_DYNCREATE(CHeadersPage, CPropertyPage)


//  -------------------------------------------------------------------
CHeadersPage::CHeadersPage() : CPropertyPage(CHeadersPage::IDD)
{
	//{{AFX_DATA_INIT(CHeadersPage)
	m_bWordWrap = FALSE;
	m_nAlignment = 0;
	m_strText = _T("");
	m_nWidth = 0;
	m_nColumnIndex = 0;
	//}}AFX_DATA_INIT
	m_psp.dwFlags &= ~PSP_HASHELP;
}


//  ---------------------------------------------------------------------
CHeadersPage::~CHeadersPage()
{
}

void CHeadersPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHeadersPage)
	DDX_Control(pDX, IDC_TREE_HEADERS, m_treeHeaders);
	DDX_Check(pDX, IDC_CHECK_WORD_WRAP, m_bWordWrap);
	DDX_CBIndex(pDX, IDC_COMBO_ALIGNMENT, m_nAlignment);
	DDX_Text(pDX, IDC_EDIT_TEXT, m_strText);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_nWidth);
	DDV_MinMaxInt(pDX, m_nWidth, 0, 10000);
	DDX_Text(pDX, IDC_EDIT_COLUMN_INDEX, m_nColumnIndex);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHeadersPage, CPropertyPage)
	//{{AFX_MSG_MAP(CHeadersPage)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_HEADERS, OnSelchangedTreeHeaders)
	ON_NOTIFY(TVN_GETDISPINFO, IDC_TREE_HEADERS, OnGetdispinfoTreeHeaders)
	ON_EN_KILLFOCUS(IDC_EDIT_TEXT, OnKillfocusEditText)
	ON_EN_KILLFOCUS(IDC_EDIT_WIDTH, OnKillfocusEditWidth)
	ON_CBN_KILLFOCUS(IDC_COMBO_ALIGNMENT, OnKillfocusComboAlignment)
	ON_EN_CHANGE(IDC_EDIT_TEXT, OnChangeEditText)
	ON_EN_CHANGE(IDC_EDIT_WIDTH, OnChangeEditWidth)
	ON_CBN_SELCHANGE(IDC_COMBO_ALIGNMENT, OnSelchangeComboAlignment)
	ON_BN_CLICKED(IDC_CHECK_WORD_WRAP, OnCheckWordWrap)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, OnButtonRemove)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHeadersPage message handlers

BOOL CHeadersPage::OnInitDialog() 
{
	int i;
	BOOL bSuccess;

	CPropertyPage::OnInitDialog();

	m_pSections = m_pGrid->GetHeaderSections();

	m_hRootItem = m_treeHeaders.InsertItem(LPSTR_TEXTCALLBACK);


	for (i = 0; i < m_pSections->GetSize(); i ++)
		FillSectionsTree(m_hRootItem, m_pSections->GetSection(i));

	bSuccess = m_treeHeaders.Expand(m_hRootItem, TVE_EXPAND);

	return TRUE;
}


//  -----------------------------------------------------------------------
void CHeadersPage::FillSectionsTree(HTREEITEM hParentItem, CGridHeaderSection *pSection)
{
	HTREEITEM hItem;
	int i;
	CGridHeaderSections *pSections;

	hItem = m_treeHeaders.InsertItem(LPSTR_TEXTCALLBACK, hParentItem);
	pSection->m_hItem = hItem;
	pSections = pSection->GetSections();
	if (0 == pSections->GetSize())
		return;

	for (i = 0; i < pSections->GetSize(); i ++)  {
		pSection = pSections->GetSection(i);
		FillSectionsTree(hItem, pSection);
	}

	m_treeHeaders.Expand(hItem, TVE_EXPAND);
}


//  ------------------------------------------------------------------------
void CHeadersPage::OnSelchangedTreeHeaders(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CGridHeaderSection *pSection;
	BOOL bSuccess;
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	bSuccess = UpdateData();
	ASSERT(bSuccess);

	//  First, we should set previously selected header section data
	pSection = FindSection(pNMTreeView->itemOld.hItem, m_pSections);
	if (pSection)  {
		pSection->SetCaption(m_strText);
		pSection->SetAlignment(m_nAlignment);
		pSection->SetWordWrap(m_bWordWrap);
	}

	//  Second, we should set currently selected header section data
	pSection = FindSection(pNMTreeView->itemNew.hItem, m_pSections);
	if (pSection)  {
		m_nColumnIndex = pSection->GetColumnIndex();
		m_strText = pSection->GetCaption();
		m_nWidth = pSection->GetWidth();
		m_nAlignment = pSection->GetAlignment();
		m_bWordWrap = pSection->GetWordWrap();

		bSuccess = UpdateData(FALSE);
		ASSERT(bSuccess);
	}

	*pResult = 0;
}


//  ------------------------------------------------------------------------
void CHeadersPage::OnGetdispinfoTreeHeaders(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	CGridHeaderSection *pSection;
	TCHAR szBuffer[256];
	
	if (pTVDispInfo->item.mask & TVIF_TEXT)  {
		if (pTVDispInfo->item.hItem == m_hRootItem)  {
			pTVDispInfo->item.pszText = _T("Header sections");
		}  else  {
			pSection = FindSection(pTVDispInfo->item.hItem, m_pSections);
			if (pSection)  {
				lstrcpy(szBuffer, pSection->GetCaption());
				pTVDispInfo->item.pszText = szBuffer;
			}
		}
	}
	
	*pResult = 0;
}


//  ----
void CHeadersPage::OnKillfocusEditText() 
{
	HTREEITEM hItem;
	CGridHeaderSection *pSection;

	hItem = m_treeHeaders.GetNextItem(m_hRootItem, TVGN_CARET);
	if (hItem == NULL)
		return;

	pSection = FindSection(hItem, m_pSections);
	if (pSection == NULL)
		return;

	pSection->SetCaption(m_strText);
}


//  ---
void CHeadersPage::OnKillfocusEditWidth() 
{
	HTREEITEM hItem;
	CGridHeaderSection *pSection;

	hItem = m_treeHeaders.GetNextItem(m_hRootItem, TVGN_CARET);
	if (hItem == NULL)
		return;

	pSection = FindSection(hItem, m_pSections);
	if (pSection == NULL)
		return;

	pSection->SetWidth(m_nWidth);
}


//  ---
void CHeadersPage::OnKillfocusComboAlignment() 
{
	HTREEITEM hItem;
	CGridHeaderSection *pSection;

	hItem = m_treeHeaders.GetNextItem(m_hRootItem, TVGN_CARET);
	if (hItem == NULL)
		return;

	pSection = FindSection(hItem, m_pSections);
	if (pSection == NULL)
		return;

	//  LVCFMT_LEFT is 0
	//  LVCFMT_RIGHT is 1
	//  LVCFMT_CENTER is 2
	//  (see commctrl.h)
	pSection->SetAlignment(m_nAlignment);
}


//  ----
CGridHeaderSection * CHeadersPage::FindSection(HTREEITEM hItem, CGridHeaderSections *pSections)
{
	int i;
	CGridHeaderSection *pSection;

	for (i = 0; i < pSections->GetSize(); i ++)  {
		pSection = pSections->GetSection(i);
		if (pSection->m_hItem == hItem)
			return pSection;
		pSection = FindSection(hItem, &pSection->m_sections);
		if (pSection && pSection->m_hItem == hItem)
			return pSection;
	}

	return NULL;
}



//  -------------------------------------------------------------------------
BOOL CHeadersPage::OnApply() 
{
	HTREEITEM hItem;
	CGridHeaderSection *pSection;
	BOOL bSuccess;

	hItem = m_treeHeaders.GetNextItem(m_hRootItem, TVGN_CARET);
	if (hItem == NULL)
		return CPropertyPage::OnApply();

	pSection = FindSection(hItem, m_pSections);
	if (pSection == NULL)
		return CPropertyPage::OnApply();

	bSuccess = UpdateData();
	ASSERT(bSuccess);

	pSection->SetCaption(m_strText);
	pSection->SetWidth(m_nWidth);
	pSection->SetAlignment(m_nAlignment);
	pSection->SetWordWrap(m_bWordWrap);

	m_pGrid->UpdateWindow();
	return CPropertyPage::OnApply();
}


//  -----------------------------------------------------------------------
void CHeadersPage::OnChangeEditText() 
{
	CheckModified();
}


//  -----------------------------------------------------------------------
void CHeadersPage::OnChangeEditWidth() 
{
	CheckModified();
}


//  ----------------------------------------------------------------------
void CHeadersPage::OnSelchangeComboAlignment() 
{
	CheckModified();
}


//  ---------------------------------------------------------------------
void CHeadersPage::OnCheckWordWrap() 
{
	CheckModified();
}


//  ---------------------------------------------------------------------
void CHeadersPage::CheckModified()
{
	HTREEITEM hItem;

	hItem = m_treeHeaders.GetNextItem(m_hRootItem, TVGN_CARET);
	if (hItem)
		SetModified();
}



//  -------------------  Add new header section...  -----------------------
void CHeadersPage::OnButtonAdd() 
{
	HTREEITEM hItem;
	CGridHeaderSection *pSection;
	BOOL bSuccess;
	CWnd *pWnd;

	hItem = m_treeHeaders.GetNextItem(m_hRootItem, TVGN_CARET);
	if (hItem == NULL || hItem == m_hRootItem)
		return;		//  Nowhere to add...

	pSection = FindSection(hItem, m_pSections);

	pSection = pSection->Add();

	pSection->m_hItem = m_treeHeaders.InsertItem(LPSTR_TEXTCALLBACK, hItem);

	bSuccess = m_treeHeaders.Expand(hItem, TVE_EXPAND);
	ASSERT(bSuccess);

	//  Set the focus to header sections tree...
	pWnd = m_treeHeaders.SetFocus();

	//  Remove the focus from currently selected header section tree item
	bSuccess = m_treeHeaders.SetItemState(hItem, 0, TVIS_SELECTED);
	//  Set the focus to the newly added section
	bSuccess = m_treeHeaders.SetItemState(pSection->m_hItem, TVIS_SELECTED, TVIS_SELECTED);
	ASSERT(bSuccess);
}



//  ----------------------  Remove selcted header section  -----------------------
void CHeadersPage::OnButtonRemove() 
{
	HTREEITEM hItem;
	CGridHeaderSection *pSection;
	CGridHeaderSection *pParentSection;
	int i;
	BOOL bSuccess;

	hItem = m_treeHeaders.GetNextItem(m_hRootItem, TVGN_CARET);
	if (hItem == NULL)
		return;

	pSection = FindSection(hItem, m_pSections);
	if (pSection == NULL)
		return;

	pParentSection = pSection->GetParent();
	if (pParentSection == NULL)
		return;		//  Top-level section

	//  Let's find the section through the sections array
	for (i = 0; i < pParentSection->GetSections()->GetSize(); i ++)  {
		if (pSection == pParentSection->m_sections[i])
			break;
	}

	delete pParentSection->GetSections()->GetSection(i);
	pParentSection->m_sections.m_sections.RemoveAt(i);

	//  --------------  Remove the item from the visual tree  ------
	bSuccess = m_treeHeaders.DeleteItem(hItem);
	ASSERT(bSuccess);
}

