// ColumnsPage.cpp : implementation file
//

#include "stdafx.h"
#include "VirtualGridDemo.h"
#include "ColumnsPage.h"
#include "DropListEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColumnsPage property page

IMPLEMENT_DYNCREATE(CColumnsPage, CPropertyPage)


//  ===============================================================================
CColumnsPage::CColumnsPage() : CPropertyPage(CColumnsPage::IDD)
{
	//{{AFX_DATA_INIT(CColumnsPage)
	m_nAlignment = 0;
	m_bFixedWidth = FALSE;
	m_bInvisible = FALSE;
	m_bMultiline = FALSE;
	m_bReadOnly = FALSE;
	m_strCaption = _T("");
	m_nWidth = 0;
	m_bTabStop = FALSE;
	m_bAlignEdit = FALSE;
	m_bAllowClick = FALSE;
	m_bWantReturns = FALSE;
	m_nEditStyle = 0;
	m_nMaxWidth = 0;
	m_nMinWidth = 0;
	m_nCheckKind = 0;
	//}}AFX_DATA_INIT
	m_psp.dwFlags &= ~PSP_HASHELP;
}


//  ================================================================================
CColumnsPage::~CColumnsPage()
{
}



void CColumnsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColumnsPage)
	DDX_Control(pDX, IDC_BUTTON_PICK_LIST, m_buttonPickList);
	DDX_Control(pDX, IDC_LIST_COLUMNS, m_listColumns);
	DDX_CBIndex(pDX, IDC_COMBO_ALIGNMENT, m_nAlignment);
	DDX_Check(pDX, IDC_CHECK_FIXED_WIDTH, m_bFixedWidth);
	DDX_Check(pDX, IDC_CHECK_INVISIBLE, m_bInvisible);
	DDX_Check(pDX, IDC_CHECK_MULTILINE, m_bMultiline);
	DDX_Check(pDX, IDC_CHECK_READ_ONLY, m_bReadOnly);
	DDX_Text(pDX, IDC_EDIT_CAPTION, m_strCaption);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_nWidth);
	DDX_Check(pDX, IDC_CHECK_TAB_STOP, m_bTabStop);
	DDX_Check(pDX, IDC_CHECK_ALIGN_EDIT, m_bAlignEdit);
	DDX_Check(pDX, IDC_CHECK_ALLOW_CLICK, m_bAllowClick);
	DDX_Check(pDX, IDC_CHECK_WANT_RETURNS, m_bWantReturns);
	DDX_CBIndex(pDX, IDC_COMBO_EDIT_STYLE, m_nEditStyle);
	DDX_Text(pDX, IDC_EDIT_MAX_WIDTH, m_nMaxWidth);
	DDV_MinMaxInt(pDX, m_nMaxWidth, 0, 10000);
	DDX_Text(pDX, IDC_EDIT_MIN_WIDTH, m_nMinWidth);
	DDV_MinMaxInt(pDX, m_nMinWidth, 0, 1000);
	DDX_CBIndex(pDX, IDC_COMBO_CHECK_KIND, m_nCheckKind);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CColumnsPage, CPropertyPage)
	//{{AFX_MSG_MAP(CColumnsPage)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_COLUMNS, OnGetdispinfoListColumns)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_COLUMNS, OnItemchangedListColumns)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, OnButtonRemove)
	ON_EN_CHANGE(IDC_EDIT_CAPTION, OnChangeEditCaption)
	ON_EN_CHANGE(IDC_EDIT_WIDTH, OnChangeEditWidth)
	ON_CBN_SELCHANGE(IDC_COMBO_ALIGNMENT, OnSelchangeComboAlignment)
	ON_BN_CLICKED(IDC_CHECK_FIXED_WIDTH, OnCheckFixedWidth)
	ON_BN_CLICKED(IDC_CHECK_MULTILINE, OnCheckMultiline)
	ON_BN_CLICKED(IDC_CHECK_READ_ONLY, OnCheckReadOnly)
	ON_BN_CLICKED(IDC_CHECK_INVISIBLE, OnCheckInvisible)
	ON_BN_CLICKED(IDC_CHECK_ALIGN_EDIT, OnCheckAlignEdit)
	ON_BN_CLICKED(IDC_CHECK_ALLOW_CLICK, OnCheckAllowClick)
	ON_BN_CLICKED(IDC_CHECK_TAB_STOP, OnCheckTabStop)
	ON_BN_CLICKED(IDC_CHECK_WANT_RETURNS, OnCheckWantReturns)
	ON_EN_CHANGE(IDC_EDIT_MAX_WIDTH, OnChangeEditMaxWidth)
	ON_EN_CHANGE(IDC_EDIT_MIN_WIDTH, OnChangeEditMinWidth)
	ON_CBN_SELCHANGE(IDC_COMBO_EDIT_STYLE, OnSelchangeComboEditStyle)
	ON_BN_CLICKED(IDC_BUTTON_PICK_LIST, OnButtonPickList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColumnsPage message handlers

BOOL CColumnsPage::OnInitDialog() 
{
	int nRetVal;
	BOOL bSuccess;

	CPropertyPage::OnInitDialog();

	m_listColumns.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	
	nRetVal = m_listColumns.InsertColumn(0, _T("Index"), LVCFMT_LEFT, 40, 0);
	nRetVal = m_listColumns.InsertColumn(1, _T("Name"), LVCFMT_LEFT, 140, 1);

	m_columns = *m_pGrid->GetGridColumns();

	if (m_columns.GetSize())  {
		bSuccess = m_listColumns.SetItemCountEx(m_columns.GetSize());
		ASSERT(bSuccess);

		bSuccess = m_listColumns.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
	}
	
	return TRUE;
}


//  -------------------------------------------------------------------------------------
void CColumnsPage::OnGetdispinfoListColumns(NMHDR* pNMHDR, LRESULT* pResult) 
{
	char buffer[256];

	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (pDispInfo->item.mask & LVIF_TEXT)  {
		switch (pDispInfo->item.iSubItem)  {
		case 0:		//  Index
			itoa(pDispInfo->item.iItem, buffer, 10);
			pDispInfo->item.pszText = buffer;
			break;
		case 1:		//  Name
			strcpy(buffer, m_columns[pDispInfo->item.iItem]->GetCaption());
			pDispInfo->item.pszText = buffer;
			break;
		default:
			break;
		}
	}
	
	*pResult = 0;
}


//  ----  Add a new column to the Virtual Grid  ------
void CColumnsPage::OnButtonAdd() 
{
	CGridColumn *pColumn;
	BOOL bSuccess;

	pColumn = m_columns.Add();
	bSuccess = m_listColumns.SetItemCountEx(m_columns.GetSize());
	ASSERT(bSuccess);

	SetModified();
}



//  ------------------------------------------------------------------
void CColumnsPage::OnButtonRemove() 
{
	int i;
	UINT nState;
	BOOL bSuccess;

	for (i = m_listColumns.GetItemCount() - 1; i >= 0; i --)  {
		nState = m_listColumns.GetItemState(i, LVNI_SELECTED);
		if (nState & LVNI_SELECTED)
			m_columns.RemoveAt(i);
	}

	bSuccess = m_listColumns.SetItemCountEx(m_columns.GetSize(), 0);
	ASSERT(bSuccess);

	SetModified();
}


//  ------------------------------------------------------------------
BOOL CColumnsPage::OnApply() 
{
	int nIndex;
	BOOL bSuccess;

	bSuccess = UpdateData();
	ASSERT(bSuccess);

	nIndex = m_listColumns.GetNextItem(-1, LVNI_FOCUSED | LVNI_SELECTED);
	if (nIndex >= 0)  {
		CGridColumn *pColumn;

		pColumn = m_columns[nIndex];

		pColumn->SetAlignEdit(m_bAlignEdit);
		pColumn->SetAllowClick(m_bAllowClick);
		pColumn->SetFixedSize(m_bFixedWidth);
		pColumn->SetReadOnly(m_bReadOnly);
		pColumn->SetTabStop(m_bTabStop);
		pColumn->SetVisible(!m_bInvisible);
		pColumn->SetWantReturns(m_bWantReturns);
		pColumn->SetWordWrap(m_bMultiline);
		pColumn->SetCheckKind((GridCheckKind)m_nCheckKind);
		pColumn->SetEditStyle((GridEditStyle)m_nEditStyle);
		pColumn->SetAlignment(m_nAlignment);
		pColumn->SetMaxWidth(m_nMaxWidth);
		pColumn->SetMinWidth(m_nMinWidth);
		pColumn->SetWidth(m_nWidth);
		pColumn->SetCaption(m_strCaption);
		pColumn->SetVisible(!m_bInvisible);
	}

	m_pGrid->SetColumns(m_columns);

	return CPropertyPage::OnApply();
}



//  ------------------------------------------------------------------------
void CColumnsPage::OnItemchangedListColumns(NMHDR* pNMHDR, LRESULT* pResult) 
{
	BOOL bSuccess;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if (pNMListView->uChanged != LVIF_STATE)  {
		*pResult = 0;
		return;
	}

	if (pNMListView->uNewState == 0 && pNMListView->uOldState == LVIS_FOCUSED)  {
		CGridColumn *pColumn;

		bSuccess = UpdateData();
		ASSERT(bSuccess);

		pColumn = m_columns[pNMListView->iItem];

		pColumn->SetCaption(m_strCaption);
		pColumn->SetWidth(m_nWidth);
		pColumn->SetMaxWidth(m_nMaxWidth);
		pColumn->SetMinWidth(m_nMinWidth);
		pColumn->SetAlignment(m_nAlignment);
		pColumn->SetEditStyle((GridEditStyle)m_nEditStyle);
		pColumn->SetCheckKind((GridCheckKind)m_nCheckKind);

		pColumn->SetAlignEdit(m_bAlignEdit);
		pColumn->SetAllowClick(m_bAllowClick);
		pColumn->SetFixedSize(m_bFixedWidth);
		pColumn->SetReadOnly(m_bReadOnly);
		pColumn->SetTabStop(m_bTabStop);
		pColumn->SetVisible(!m_bInvisible);
		pColumn->SetWantReturns(m_bWantReturns);
		pColumn->SetWordWrap(m_bMultiline);
	}  else if ((pNMListView->uNewState == (LVIS_FOCUSED | LVIS_SELECTED)) && pNMListView->uOldState == 0)  {
		CGridColumn *pColumn;

		pColumn = m_columns[pNMListView->iItem];

		m_strCaption = pColumn->GetCaption();
		m_nWidth = pColumn->GetWidth();
		m_nMaxWidth = pColumn->GetMaxWidth();
		m_nMinWidth = pColumn->GetMinWidth();
		m_nAlignment = pColumn->GetAlignment();
		m_nEditStyle = pColumn->GetEditStyle();
		m_nCheckKind = pColumn->GetCheckKind();

		m_bAlignEdit = pColumn->GetAlignEdit();
		m_bAllowClick = pColumn->GetAllowClick();
		m_bFixedWidth = pColumn->GetFixedSize();
		m_bReadOnly = pColumn->GetReadOnly();
		m_bTabStop = pColumn->GetTabStop();
		m_bInvisible = !pColumn->GetVisible();
		m_bWantReturns = pColumn->GetWantReturns();
		m_bMultiline = pColumn->GetWordWrap();

		bSuccess = UpdateData(FALSE);
		ASSERT(bSuccess);

		//  !!!!!!!!!
		if (m_nEditStyle == (int)geDataList || m_nEditStyle == (int)gePickList)
			m_buttonPickList.EnableWindow();
		else
			m_buttonPickList.EnableWindow(FALSE);
	}

	*pResult = 0;
}


//  ==========================  CHANGED area  ==========================


//  ----------------------------------------------------------------------
void CColumnsPage::CheckModified()
{
	int nIndex;

	nIndex = m_listColumns.GetNextItem(-1, LVIS_FOCUSED | LVIS_SELECTED);
	if (nIndex >= 0)
		SetModified();
}

//  -------------------------------------------------------------------------
void CColumnsPage::OnChangeEditCaption() 
{
	CheckModified();
}


//  ------------------------------------------------------------------------
void CColumnsPage::OnChangeEditWidth() 
{
	CheckModified();
}


//  -----------------------------------------------------------------------
void CColumnsPage::OnSelchangeComboAlignment() 
{
	CheckModified();
}



//  ----------------------------------------------------------------------
void CColumnsPage::OnCheckFixedWidth() 
{
	CheckModified();
}


//  ---------------------------------------------------------------------
void CColumnsPage::OnCheckMultiline() 
{
	CheckModified();
}


//  ---------------------------------------------------------------------
void CColumnsPage::OnCheckReadOnly() 
{
	CheckModified();
}


//  ---------------------------------------------------------------------
void CColumnsPage::OnCheckInvisible() 
{
	CheckModified();
}


//  -------------------------------------------------------------------
void CColumnsPage::OnCheckAlignEdit() 
{
	CheckModified();
}


//  -----------------------------------------------------------
void CColumnsPage::OnCheckAllowClick() 
{
	CheckModified();
}

//  -----------------------------------------------------------
void CColumnsPage::OnCheckTabStop() 
{
	CheckModified();
}

//  -----------------------------------------------------------
void CColumnsPage::OnCheckWantReturns() 
{
	CheckModified();
}


//  ---------------------------------------------------------
void CColumnsPage::OnChangeEditMaxWidth() 
{
	CheckModified();
}


//  ---------------------------------------------------------
void CColumnsPage::OnChangeEditMinWidth() 
{
	CheckModified();
}


//  ---------------------------------------------------------
void CColumnsPage::OnSelchangeComboEditStyle() 
{
	BOOL bSuccess;

	CheckModified();

	bSuccess = UpdateData();
	ASSERT(bSuccess);

	if (m_nEditStyle == (int)geDataList || m_nEditStyle == (int)gePickList)
		m_buttonPickList.EnableWindow();
	else
		m_buttonPickList.EnableWindow(FALSE);
}


//  ------------------------------------------------------
void CColumnsPage::OnButtonPickList() 
{
	int nIndex;
	CGridColumn *pColumn;
	CStringArray pickList;
	int i;

	nIndex = m_listColumns.GetNextItem(-1, LVNI_FOCUSED | LVNI_SELECTED);
	if (nIndex < 0)
		return;

	pColumn = m_columns[nIndex];
	pColumn->GetPickList(pickList);

	CDropListEditDlg dlg(this);

	for (i = 0; i < pickList.GetSize(); i ++)  {
		dlg.m_strDropList += pickList[i];
		dlg.m_strDropList += _T("\r\n");
	}


	if (IDOK == dlg.DoModal())  {
		pColumn->SetPickList(dlg.m_pickList);
	}
}

