// CodeGenPage.cpp : implementation file
//

#include "stdafx.h"
#include "VirtualGridDemo.h"
#include "CodeGenPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CCodeGenPage property page

IMPLEMENT_DYNCREATE(CCodeGenPage, CPropertyPage)

CCodeGenPage::CCodeGenPage() : CPropertyPage(CCodeGenPage::IDD)
{
	//{{AFX_DATA_INIT(CCodeGenPage)
	m_strGridName = _T("");
	m_strCode = _T("");
	m_bAllowGridEdit = FALSE;
	m_nRowCount = 0;
	//}}AFX_DATA_INIT

	m_psp.dwFlags &= ~PSP_HASHELP;
}

CCodeGenPage::~CCodeGenPage()
{
}

void CCodeGenPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCodeGenPage)
	DDX_Control(pDX, IDC_EDIT_ROW_COUNT, m_editRowCount);
	DDX_Control(pDX, IDC_SPIN_ROW_COUNT, m_spinRowCount);
	DDX_Control(pDX, IDC_EDIT_CODE, m_editCode);
	DDX_Text(pDX, IDC_EDIT_GRID_NAME, m_strGridName);
	DDX_Text(pDX, IDC_EDIT_CODE, m_strCode);
	DDX_Check(pDX, IDC_CHECK_ALLOW_EDIT, m_bAllowGridEdit);
	DDX_Text(pDX, IDC_EDIT_ROW_COUNT, m_nRowCount);
	DDV_MinMaxInt(pDX, m_nRowCount, 0, 10000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCodeGenPage, CPropertyPage)
	//{{AFX_MSG_MAP(CCodeGenPage)
	ON_BN_CLICKED(IDC_BUTTON_GENERATE, OnButtonGenerate)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_COPY, OnButtonCopy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CCodeGenPage message handlers

BOOL CCodeGenPage::OnInitDialog() 
{
	BOOL bSuccess;

	CPropertyPage::OnInitDialog();
	
	m_strGridName = _T("m_grid");
	m_spinRowCount.SetRange(0, 10000);

	bSuccess = UpdateData(FALSE);
	ASSERT(bSuccess);

	return TRUE;
}



//  ----------------------  Generate code  ------------------------------
void CCodeGenPage::OnButtonGenerate() 
{
	int i;
	BOOL bSuccess;
	CGridColumns *pColumns;
	CGridColumn *pColumn;
	CGridHeaderSections *pSections;
	TCHAR szParamBuffer[256];
	CGridHeaderSection *pSection;

	m_strCode.Empty();

	//  To get true grid object name, permission to edit grid cells and
	//  initial row count
	bSuccess = UpdateData();
	ASSERT(bSuccess);

	pColumns = m_pGrid->GetColumns();

	//  -----------------  Let's add some wise comments  -------------------

	//  Variable definition part...
	m_strCode = _T("\t//  ---------------  Required variables  -----------------\r\n");
	m_strCode += _T("\tCGridColumn *pColumn;\r\n");
	m_strCode += _T("\tCGridHeaderSections *pSections;\r\n");
	m_strCode += _T("\tCGridHeaderSection *pSection;\r\n");
	m_strCode += _T("\tCGridHeaderSection *pUpperSection;\r\n");

	m_strCode += _T("\r\n");


	//  --------------------  ...Columns part  -----------------------------
	m_strCode += _T("\t//  -----------------  Let's add some columns  --------------  \r\n");

	for (i = 0; i < pColumns->GetSize(); i ++)  {
		pColumn = pColumns->GetAt(i);

		m_strCode += _T("\t");
		m_strCode += m_strGridName + _T(".AddColumn(");
		m_strCode += _T("_T(\"");
		m_strCode += pColumn->GetCaption();
		m_strCode += _T("\"), ");

		itoa(pColumn->GetWidth(), szParamBuffer, 10);
		m_strCode += szParamBuffer;
		m_strCode += _T(", ");


		switch (pColumn->GetAlignment())  {
		case LVCFMT_LEFT:
			m_strCode += _T("LVCFMT_LEFT");
			break;
		case LVCFMT_RIGHT:
			m_strCode += _T("LVCFMT_RIGHT");
			break;
		case LVCFMT_CENTER:
			m_strCode += _T("LVCFMT_CENTER");
			break;
		default:
			break;
		}

		m_strCode += _T(");\r\n");
	}

	m_strCode += _T("\r\n");

	m_strCode += _T("\t//  ---------------  Set additional column properties  ----------------  \r\n");
	
	for (i = 0; i < pColumns->GetSize(); i ++)  {
		int nWidth;
		GridEditStyle editStyle;

		pColumn = pColumns->GetAt(i);


		if (10000 != pColumn->GetMaxWidth()
			|| 0 != pColumn->GetMinWidth()
			|| geSimple != pColumn->GetEditStyle()
			|| pColumn->GetAlignEdit()
			|| !pColumn->GetAllowClick()
			|| pColumn->GetFixedSize()
			|| pColumn->GetReadOnly()
			|| !pColumn->GetTabStop()
			|| !pColumn->GetVisible()
			|| pColumn->GetWantReturns()
			|| pColumn->GetWordWrap()
			)  {

			m_strCode += _T("\r\n");
			m_strCode += _T("\tpColumn = ");
			m_strCode += m_strGridName;
			m_strCode += _T(".GetColumn(");
			itoa(i, szParamBuffer, 10);
			m_strCode += szParamBuffer;
			m_strCode += _T(");\r\n");
		}


		editStyle = pColumn->GetEditStyle();
		if (editStyle != geSimple)  {
			m_strCode += _T("\tpColumn->SetEditStyle(");
			switch (editStyle)  {
			case geEllipsis:
				m_strCode += _T("geEllipsis");
				break;
			case gePickList:
				m_strCode += _T("gePickList");
				break;
			case geDataList:
				m_strCode += _T("geDataList");
				break;
			case geUserDefine:
				m_strCode += _T("geUserDefine");
			default:
				break;
			}

			m_strCode += _T(");\r\n");
		}

		nWidth = pColumn->GetMaxWidth();
		if (nWidth != MAX_COLUMN_WIDTH)  {
			m_strCode += _T("\tpColumn->SetMaxWidth(");
			itoa(nWidth, szParamBuffer, 10);
			m_strCode += szParamBuffer;
			m_strCode += _T(");\r\n");
		}

		nWidth = pColumn->GetMinWidth();
		if (nWidth)  {
			m_strCode += _T("\tpColumn->SetMinWidth(");
			itoa(nWidth, szParamBuffer, 10);
			m_strCode += szParamBuffer;
			m_strCode += _T(");\r\n");
		}

		if (pColumn->GetAlignEdit())
			m_strCode += _T("\tpColumn->SetAlignEdit(TRUE);\r\n");

		if (!pColumn->GetAllowClick())
			m_strCode += _T("\tpColumn->SetAllowClick(FALSE);\r\n");

		if (pColumn->GetFixedSize())
			m_strCode += _T("\tpColumn->SetFixedSize(TRUE);\r\n");

		if (pColumn->GetReadOnly())
			m_strCode += _T("\tpColumn->SetReadOnly(TRUE);\r\n");

		if (!pColumn->GetTabStop())
			m_strCode += _T("\tpColumn->SetTabStop(FALSE);\r\n");

		if (!pColumn->GetVisible())
			m_strCode += _T("\tpColumn->SetVisible(FALSE);\r\n");

		if (pColumn->GetWantReturns())
			m_strCode += _T("\tpColumn->SetWantReturns(TRUE);\r\n");

		if (pColumn->GetWordWrap())
			m_strCode += _T("\tpColumn->SetWordWrap(TRUE);\r\n");
	}
	

	//  --------------------  Header sections part  -------------------------
	m_strCode += _T("\r\n");
	m_strCode += _T("\t//  ---------------  Let's put the grid header into shape  ------------\r\n");
	m_strCode += _T("\tpSections = ");
	m_strCode += m_strGridName;
	m_strCode += _T(".GetHeader()->GetSections();\r\n");

	pSections = m_pGrid->GetHeader()->GetSections();

	for (i = 0; i < pSections->GetSize(); i ++)  {
		int nAlignment;

		pSection = pSections->GetSection(i);
		m_strCode += _T("\tpUpperSection = pSections->GetSection(");
		itoa(i, szParamBuffer, 10);
		m_strCode += szParamBuffer;
		m_strCode += _T(");\r\n");

		if (pSection->GetCaption().GetLength())  {
			m_strCode += _T("\tpUpperSection->SetCaption(_T(\"");
			m_strCode += pSection->GetCaption();
			m_strCode += _T("\"));\r\n");
		}

		nAlignment = pSection->GetAlignment();
		if (nAlignment != LVCFMT_LEFT)  {
			m_strCode += _T("\tpUpperSection->SetAlignment(");

			switch (nAlignment)  {
			case LVCFMT_RIGHT:
				m_strCode += _T("LVCFMT_RIGHT");
				break;
			case LVCFMT_CENTER:
				m_strCode += _T("LVCFMT_CENTER");
				break;
			default:
				m_strCode += _T("LVCFMT_LEFT");
				break;
			}

			m_strCode += _T(");\r\n");
		}

		if (pSection->GetWordWrap())
			m_strCode += _T("\tpUpperSection->SetWordWrap(TRUE);\r\n");

		m_strCode += ComposeSectionTree(pSection);
	}

	m_strCode += _T("\tm_grid.GetHeader()->SynchronizeSections();\r\n\r\n");

	//  ---------------  Additional code generation  ----------------
	m_strCode += _T("\t//  --------------  Some additional initializations...  ------\r\n");
	if (m_bAllowGridEdit)  {
		m_strCode += _T("\t");
		m_strCode += m_strGridName;
		m_strCode += _T(".SetAllowEdit();\r\n");
	}

	if (m_nRowCount > 0)  {
		CString m_strNumber;
		m_strCode += _T("\t");
		m_strCode += m_strGridName;
		m_strCode += _T(".SetRowCount(");
		m_editRowCount.GetWindowText(m_strNumber);
		m_strCode += m_strNumber;
		m_strCode += _T(");\r\n");
	}

	bSuccess = UpdateData(FALSE);
	ASSERT(bSuccess);
}



//  ---------------------  Clear code  ---------------------------
void CCodeGenPage::OnButtonClear() 
{
	BOOL bSuccess;

	m_strCode.Empty();
	bSuccess = UpdateData(FALSE);
	ASSERT(bSuccess);
}


//  ---------  Select ALL text in editor and copy into clipboard  ---------
void CCodeGenPage::OnButtonCopy() 
{
	m_editCode.SetSel(0, -1);
	m_editCode.Copy();
}



//  -----------------------------------------------------------------------
CString CCodeGenPage::ComposeSectionTree(CGridHeaderSection *pSection)
{
	CString strCode;
	int i;
	CGridHeaderSections *pSections;

	pSections = pSection->GetSections();
	if (!pSections->GetSize())
		return strCode;

	for (i = 0; i < pSections->GetSize(); i ++)  {
		int nAlignment;

		pSection = pSections->GetSection(i);
		strCode += _T("\tpSection = pUpperSection->Add();\r\n");

		if (pSection->GetCaption().GetLength())  {
			strCode += _T("\tpSection->SetCaption(_T(\"");
			strCode += pSection->GetCaption();
			strCode += _T("\"));\r\n");
		}

		nAlignment = pSection->GetAlignment();
		if (nAlignment != LVCFMT_LEFT)  {
			strCode += _T("\tpSection->SetAlignment(");
			switch (nAlignment)  {
			case LVCFMT_RIGHT:
				strCode += _T("LVCFMT_RIGHT");
				break;
			case LVCFMT_CENTER:
				strCode += _T("LVCFMT_CENTER");
				break;
			default:
				strCode += _T("LVCFMT_LEFT");
				break;
			}

			strCode += _T(");\r\n");
		}

		if (pSection->GetWordWrap())
			strCode += _T("\tpSection->SetWordWrap(TRUE);\r\n");

		strCode += _T("\tpUpperSection = pSection;\r\n");
		strCode += ComposeSectionTree(pSection);
		strCode += _T("\tpUpperSection = pSection->GetParent();\r\n");
	}

	return strCode;
}


