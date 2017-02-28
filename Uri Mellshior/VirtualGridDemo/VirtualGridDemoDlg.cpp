// VirtualGridDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VirtualGridDemo.h"
#include "VirtualGridDemoDlg.h"
#include "GridPropertiesSheet.h"
#include "CellSettingsDlg.h"
#include "GeneralPageDlg.h"
#include "FixedPageDlg.h"
#include "HeaderPageDlg.h"
#include "StylesPageDlg.h"
#include "RowsPageDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define INITIAL_ROW_COUNT		8


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVirtualGridDemoDlg dialog

CVirtualGridDemoDlg::CVirtualGridDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVirtualGridDemoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVirtualGridDemoDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVirtualGridDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVirtualGridDemoDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_GRID, m_grid);
}

BEGIN_MESSAGE_MAP(CVirtualGridDemoDlg, CDialog)
	//{{AFX_MSG_MAP(CVirtualGridDemoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_EXIT, OnExit)
	ON_BN_CLICKED(IDC_BUTTON_HEAD_CONSTRUCT, OnButtonHeadConstruct)
	//}}AFX_MSG_MAP
	ON_NOTIFY(VGN_GETDISPINFO, IDC_GRID, OnGridGetDispInfo)
	ON_NOTIFY(VGN_SETDISPINFO, IDC_GRID, OnGridSetDispInfo)
	ON_NOTIFY(VGN_EDITBUTTONPRESS, IDC_GRID, OnGridEditButtonPress)
	ON_NOTIFY(VGN_GETCHECKSTATE, IDC_GRID, OnGridGetCheckState)
	ON_NOTIFY(VGN_CHECKCLICK, IDC_GRID, OnGridCheckClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVirtualGridDemoDlg message handlers

BOOL CVirtualGridDemoDlg::OnInitDialog()
{
	BOOL bSuccess;
	int nIndex;

	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	//  -------------------  Uri  ------------------------
	bSuccess = m_images.Create(IDB_SIGNS, 16, 0, RGB(192, 192, 192));
	ASSERT(bSuccess);

	//  -------  Adding grid callback data  ------
	m_demoData.Add(CDemoGridRow(_T("First point"), 0, 2, 4));
	m_demoData.Add(CDemoGridRow(_T("Second point"), 4, 6, 7));
	m_demoData.Add(CDemoGridRow(_T("Third point"), 6, 4, 1));
	m_demoData.Add(CDemoGridRow(_T("Fourth point"), 4, 56, 2));
	m_demoData.Add(CDemoGridRow(_T("Fifth point"), 5, 17, 5));
	m_demoData.Add(CDemoGridRow(_T("Sixth point"), 44, 237, 76));
	m_demoData.Add(CDemoGridRow(_T("Seventh point"), 44, 27, 68));
	m_demoData.Add(CDemoGridRow(_T("Eighth point"), 96, 12, 38));

	//  ------  Set last column's cells colors: both background and foreground ones ---
	m_demoData[0].m_crBkColor = RGB(0, 123, 120);
	m_demoData[0].m_crTextColor = RGB(255, 0, 0);
	m_demoData[1].m_crBkColor = RGB(111, 111, 111);
	m_demoData[1].m_crTextColor = RGB(255, 255, 255);
	m_demoData[2].m_crBkColor = RGB(250, 0, 111);
	m_demoData[2].m_crTextColor = RGB(0, 111, 230);
	m_demoData[3].m_crBkColor = RGB(0, 200, 0);
	m_demoData[3].m_crTextColor = RGB(240, 240, 240);

	m_grid.SetImageList(&m_images);
//	m_grid.GetHeader()->SetImageList(&m_images);


	//  =====================  Auto generated code goes here  =========================

	//  ---------------  Required variables  -----------------
	CGridColumn *pColumn;
	CGridHeaderSections *pSections;
	CGridHeaderSection *pSection;
	CGridHeaderSection *pUpperSection;

	//  -----------------  Let's add some columns  --------------  
	m_grid.AddColumn(_T(""), 24, LVCFMT_LEFT);
	m_grid.AddColumn(_T(""), 140, LVCFMT_LEFT);
	m_grid.AddColumn(_T(""), 64, LVCFMT_RIGHT);
	m_grid.AddColumn(_T(""), 64, LVCFMT_RIGHT);
	m_grid.AddColumn(_T(""), 64, LVCFMT_RIGHT);
	m_grid.AddColumn(_T(""), 90, LVCFMT_LEFT);


	//  ---------------  Set additional column properties  ----------------  
	pColumn = m_grid.GetColumn(0);
	pColumn->SetFixedSize(TRUE);
	pColumn->SetTabStop(FALSE);

	pColumn = m_grid.GetColumn(1);
	pColumn->SetCheckKind(gcCheckBox);

	pColumn = m_grid.GetColumn(4);
	pColumn->SetEditStyle(gePickList);
	CStringArray strPickList;
	strPickList.Add("1234");
	strPickList.Add("4321");
	strPickList.Add("1234789");
	pColumn->SetPickList(strPickList);

	pColumn = m_grid.GetColumn(5);
	pColumn->SetEditStyle(geEllipsis);


	//  ---------------  Let's put the grid header into shape  ------------
	pSections = m_grid.GetHeader()->GetSections();
	pUpperSection = pSections->GetSection(0);
	pUpperSection->SetCaption(_T("Points"));
	pUpperSection->SetAlignment(LVCFMT_CENTER);
	pSection = pUpperSection->Add();
	pSection->SetCaption(_T("#"));
	pUpperSection = pSection;
	pUpperSection = pSection->GetParent();
	pSection = pUpperSection->Add();
	pSection->SetCaption(_T("Point name"));
	pSection->SetAlignment(LVCFMT_CENTER);
	pUpperSection = pSection;
	pUpperSection = pSection->GetParent();
	pUpperSection = pSections->GetSection(1);
	pUpperSection->SetCaption(_T("Position data"));
	pUpperSection->SetAlignment(LVCFMT_CENTER);
	pSection = pUpperSection->Add();
	pSection->SetCaption(_T("X"));
	pSection->SetAlignment(LVCFMT_CENTER);
	pUpperSection = pSection;
	pUpperSection = pSection->GetParent();
	pSection = pUpperSection->Add();
	pSection->SetCaption(_T("Y"));
	pSection->SetAlignment(LVCFMT_CENTER);
	pUpperSection = pSection;
	pUpperSection = pSection->GetParent();
	pSection = pUpperSection->Add();
	pSection->SetCaption(_T("Z"));
	pSection->SetAlignment(LVCFMT_CENTER);
	pUpperSection = pSection;
	pUpperSection = pSection->GetParent();
	pUpperSection = pSections->GetSection(2);
	pUpperSection->SetCaption(_T("Additional"));
	pUpperSection->SetAlignment(LVCFMT_CENTER);
	m_grid.GetHeader()->SynchronizeSections();

	//  --------------  Some additional initializations...  ------
	m_grid.SetAllowEdit();
	m_grid.SetRowCount(INITIAL_ROW_COUNT);

	//  =====================  ...auto generated code  ============================
	m_grid.SetFixedCount(1);

	
	//  --------------  Setting up Rollup Control  --------------
	CRect rect;
	GetClientRect(rect);
	rect.left = rect.right - 180;
	rect.top += 10;
	rect.right -= 10;
	rect.bottom -= 44;

	bSuccess = m_rollup.Create(WS_CHILD | WS_DLGFRAME | WS_VISIBLE, rect, this, 0);
	ASSERT(bSuccess);


	//  -----  General grid properties  ---
	CGeneralPageDlg *pGeneralPage = new CGeneralPageDlg;
	pGeneralPage->m_pGrid = &m_grid;
	pGeneralPage->m_bAllowEdit = m_grid.GetAllowEdit();
	pGeneralPage->m_bAllowSelect = m_grid.GetAllowSelect();
	pGeneralPage->m_bAlwaysEdit = m_grid.GetAlwaysEdit();
	pGeneralPage->m_bAlwaysSelected = m_grid.GetAlwaysSelected();
	pGeneralPage->m_bCancelOnExit = m_grid.GetCancelOnExit();
	pGeneralPage->m_bCheckBoxes = m_grid.GetCheckBoxes();
	pGeneralPage->m_bGridLines = m_grid.GetGridLines();
	pGeneralPage->m_bRowSelect = m_grid.GetRowSelect();
	pGeneralPage->m_nCheckStyle = (int)m_grid.GetCheckStyle();
	pGeneralPage->m_bDoubleBuffered = m_grid.GetDoubleBuffered();

	bSuccess = pGeneralPage->Create(IDD_PAGE_GENERAL, &m_rollup);
	nIndex = m_rollup.InsertPage(_T("General properties"), pGeneralPage);

	//  ------------  Grid fixed properties  --------------
	CFixedPageDlg *pFixedPage = new CFixedPageDlg;
	pFixedPage->m_pGrid = &m_grid;
	CGridFixed *pFixed = m_grid.GetFixed();
	pFixedPage->m_bFlat = pFixed->GetFlat();
	pFixedPage->m_bGridColor = pFixed->GetGridColor();
	pFixedPage->m_nCount = pFixed->GetCount();
	bSuccess = pFixedPage->Create(IDD_PAGE_FIXED, &m_rollup);
	nIndex = m_rollup.InsertPage(_T("Fixed part"), pFixedPage);


	//  -----  Grid header properties  -----
	CHeaderPageDlg *pHeaderPage = new CHeaderPageDlg;
	pHeaderPage->m_pGrid = &m_grid;
	CGridHeader *pHeader = m_grid.GetHeader();
	pHeaderPage->m_bAutoHeight = pHeader->GetAutoHeight();
	pHeaderPage->m_bAutoSynchronize = pHeader->GetAutoSynchronize();
	pHeaderPage->m_bFlat = pHeader->GetFlat();
	pHeaderPage->m_bFullSynchronizing = pHeader->GetFullSynchronizing();
	pHeaderPage->m_bGridColor = pHeader->GetGridColor();
	pHeaderPage->m_nSectionHeight = pHeader->GetSectionHeight();
	bSuccess = pHeaderPage->Create(IDD_PAGE_HEADER, &m_rollup);
	nIndex = m_rollup.InsertPage(_T("Header properties"), pHeaderPage);

	//  -----  Grid styles  ------
	CStylesPageDlg *pStylesPage = new CStylesPageDlg;
	pStylesPage->m_pGrid = &m_grid;
	UINT nStyle = m_grid.GetGridStyle();
	if (nStyle & gsHorzLine)
		pStylesPage->m_bHorzLine = TRUE;
	if (nStyle & gsVertLine)
		pStylesPage->m_bVertLine = TRUE;
	if (nStyle & gsFullHorzLine)
		pStylesPage->m_bFullHorzLine = TRUE;
	if (nStyle & gsFullVertLine)
		pStylesPage->m_bFullVertLine = TRUE;
	if (nStyle & gsListViewLike)
		pStylesPage->m_bListView = TRUE;
	if (nStyle & gsDotLines)
		pStylesPage->m_bDotLines = TRUE;
	bSuccess = pStylesPage->Create(IDD_PAGE_STYLES, &m_rollup);
	nIndex = m_rollup.InsertPage(_T("Grid styles"), pStylesPage);

	//  ------------  Grid rows properties  ------------
	CRowsPageDlg *pRowsPage = new CRowsPageDlg;
	CGridRows *pRows = m_grid.GetRows();
	pRowsPage->m_pGrid = &m_grid;
	pRowsPage->m_pDemoData = &m_demoData;
	pRowsPage->m_bAutoHeight = pRows->GetAutoHeight();
	pRowsPage->m_nCount = pRows->GetCount();
	pRowsPage->m_nHeight = pRows->GetHeight();

	bSuccess = pRowsPage->Create(IDD_PAGE_ROWS, &m_rollup);
	nIndex = m_rollup.InsertPage(_T("Grid rows"), pRowsPage);

	return TRUE;
}


//  -------------------------------------------------------------------
void CVirtualGridDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVirtualGridDemoDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVirtualGridDemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


//  -----
void CVirtualGridDemoDlg::OnButtonHeadConstruct() 
{
	int nRetVal;
//
	SCROLLINFO si;
	BOOL bSuccess;

	ZeroMemory(&si, sizeof(SCROLLINFO));

	bSuccess = GetScrollInfo(SB_HORZ, &si);
//
	CGridPropertiesSheet sheet(_T("Header and columns constructor"), this);

	sheet.m_pGrid = &m_grid;
	sheet.m_columnsPage.m_pGrid = &m_grid;
	sheet.m_headersPage.m_pGrid = &m_grid;
	sheet.m_codeGenPage.m_pGrid = &m_grid;

	nRetVal = sheet.DoModal();
}


//  -------------------------------------------------------------------------
void CVirtualGridDemoDlg::OnOK() 
{
}


//  -------------------------------------------------------------------------
void CVirtualGridDemoDlg::OnCancel() 
{

}


//  -------------------------------------------------------------------------
void CVirtualGridDemoDlg::OnExit() 
{
	CDialog::OnCancel();	
}



//  ---
void CVirtualGridDemoDlg::OnGridGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult)
{
	char buffer[64];
	VG_DISPINFO *pDispInfo = (VG_DISPINFO *)pNMHDR;

	if (pDispInfo->item.mask & LVIF_TEXT)  {
		switch (pDispInfo->item.nColumn)  {
		case 0:		//  #
			itoa(pDispInfo->item.nRow + 1, buffer, 10);
			pDispInfo->item.strText = buffer;
			break;
		case 1:		//  Point name
			pDispInfo->item.strText = m_demoData[pDispInfo->item.nRow].m_strPointName;
			break;
		case 2:		//  X
			itoa(m_demoData[pDispInfo->item.nRow].m_x, buffer, 10);
			pDispInfo->item.strText = buffer;
			break;
		case 3:		//  Y
			itoa(m_demoData[pDispInfo->item.nRow].m_y, buffer, 10);
			pDispInfo->item.strText = buffer;
			break;
		case 4:		//  Z
			itoa(m_demoData[pDispInfo->item.nRow].m_z, buffer, 10);
			pDispInfo->item.strText = buffer;
			break;
		case 5:
			pDispInfo->item.strText = m_demoData[pDispInfo->item.nRow].m_strText;
			break;
		default:
			break;
		}
	}

	if (pDispInfo->item.mask & LVIF_IMAGE)  {
		switch (pDispInfo->item.nColumn)  {
		case 0:		//  #
			break;
		case 1:		//  Point name
			pDispInfo->item.nImage = 0;
			break;
		case 2:		//  X
			pDispInfo->item.nImage = 1;
			break;
		case 3:		//  Y
			pDispInfo->item.nImage = 2;
			break;
		case 4:
			pDispInfo->item.nImage = 3;
			break;
		default:
			break;
		}
	}

	if (pDispInfo->item.mask & LVIF_COLOR)  {
		if (pDispInfo->item.nColumn == 5)  {
			pDispInfo->item.pDC->SetBkColor(m_demoData[pDispInfo->item.nRow].m_crBkColor);
			pDispInfo->item.pDC->SetTextColor(m_demoData[pDispInfo->item.nRow].m_crTextColor);
		}
	}
}




//  ---
void CVirtualGridDemoDlg::OnGridSetDispInfo(NMHDR* pNMHDR, LRESULT* pResult)
{
	VG_DISPINFO *pDispInfo = (VG_DISPINFO *)pNMHDR;

	switch (pDispInfo->item.nColumn)  {
	case 0:		//  #
		break;
	case 1:		//  Point name
		m_demoData[pDispInfo->item.nRow].m_strPointName = pDispInfo->item.strText;
		break;
	case 4:
		m_demoData[pDispInfo->item.nRow].m_z = atoi(pDispInfo->item.strText);
		break;
	default:
		break;
	}

	*pResult = 0;
}



//  ---------------------------------------------------------------------------------
void CVirtualGridDemoDlg::OnGridEditButtonPress(NMHDR *pNMHDR, LRESULT *pResult)
{
	VG_DISPINFO *pDispInfo = (VG_DISPINFO *)pNMHDR;
	*pResult = 0;
	
	m_grid.CancelEdit();

	if (pDispInfo->item.nColumn == 5)  {
		CCellSettingsDlg dlg(this);
		dlg.m_strCellText = m_demoData[pDispInfo->item.nRow].m_strText;
		dlg.m_crBkColor = m_demoData[pDispInfo->item.nRow].m_crBkColor;
		dlg.m_crTextColor = m_demoData[pDispInfo->item.nRow].m_crTextColor;
		
		if (IDOK == dlg.DoModal())  {
			m_demoData[pDispInfo->item.nRow].m_strText = dlg.m_strCellText;
			m_demoData[pDispInfo->item.nRow].m_crBkColor = dlg.m_crBkColor;
			m_demoData[pDispInfo->item.nRow].m_crTextColor = dlg.m_crTextColor;
		}
	}
}



//  ---
void CVirtualGridDemoDlg::OnGridGetCheckState(NMHDR *pNMHDR, LRESULT *pResult)
{
	VG_DISPINFO *pDispInfo = (VG_DISPINFO *)pNMHDR;
	*pResult = 0;

	pDispInfo->item.nCheckState = m_demoData[pDispInfo->item.nRow].m_bChecked;
}


//  ---
void CVirtualGridDemoDlg::OnGridCheckClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	VG_DISPINFO *pDispInfo = (VG_DISPINFO *)pNMHDR;
	*pResult = 0;

	m_demoData[pDispInfo->item.nRow].m_bChecked = !m_demoData[pDispInfo->item.nRow].m_bChecked;
	m_grid.InvalidateCell(CGridCell(pDispInfo->item.nColumn, pDispInfo->item.nRow));
}




