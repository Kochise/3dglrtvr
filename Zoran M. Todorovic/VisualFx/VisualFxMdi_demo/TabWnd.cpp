// TabWnd.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "TabWnd.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "TabWndDoc.h"
#include "TabWndView.h"
#include "MdiFrames.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabWndApp

BEGIN_MESSAGE_MAP(CTabWndApp, CWinApp)
	//{{AFX_MSG_MAP(CTabWndApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabWndApp construction

CTabWndApp::CTabWndApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTabWndApp object

CTabWndApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CTabWndApp initialization

BOOL CTabWndApp::InitInstance()
{
	// Standard initialization

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register document templates

	AddDocTemplate(new CMultiDocTemplate(IDR_TABWNDTYPE,RUNTIME_CLASS(CTabWndDoc),
                      RUNTIME_CLASS(CSimpleViewFrame),RUNTIME_CLASS(CTabWndView)));
	AddDocTemplate(new CMultiDocTemplate(IDR_TABWNDTYPE1,RUNTIME_CLASS(CTabWndDoc),
                      RUNTIME_CLASS(CSplitterFrame),RUNTIME_CLASS(CTabWndView)));
	AddDocTemplate(new CMultiDocTemplate(IDR_TABWNDTYPE2,RUNTIME_CLASS(CTabWndDoc),
                      RUNTIME_CLASS(CSimpleTabFrame),RUNTIME_CLASS(CTabWndView)));
	AddDocTemplate(new CMultiDocTemplate(IDR_TABWNDTYPE3,RUNTIME_CLASS(CTabWndDoc),
                      RUNTIME_CLASS(CSplitterTabFrame),RUNTIME_CLASS(CTabWndView)));
	AddDocTemplate(new CMultiDocTemplate(IDR_TABWNDTYPE4,RUNTIME_CLASS(CTabWndDoc),
                      RUNTIME_CLASS(CTabSplitterFrame),RUNTIME_CLASS(CTabWndView)));
	AddDocTemplate(new CMultiDocTemplate(IDR_TABWNDTYPE5,RUNTIME_CLASS(CTabWndDoc),
                      RUNTIME_CLASS(CComplexFrameA),RUNTIME_CLASS(CTabWndView)));
	AddDocTemplate(new CMultiDocTemplate(IDR_TABWNDTYPE6,RUNTIME_CLASS(CTabWndDoc),
                      RUNTIME_CLASS(CComplexFrameB),RUNTIME_CLASS(CTabWndView)));
	AddDocTemplate(new CMultiDocTemplate(IDR_TABWNDTYPE7,RUNTIME_CLASS(CTabWndDoc),
                      RUNTIME_CLASS(CComplexFrameC),RUNTIME_CLASS(CTabWndView)));
	AddDocTemplate(new CMultiDocTemplate(IDR_TABWNDTYPE8,RUNTIME_CLASS(CTabWndDoc),
                      RUNTIME_CLASS(CComplexFrameD),RUNTIME_CLASS(CTabWndView)));

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	//if (!ProcessShellCommand(cmdInfo))
//		return FALSE;
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}


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
		// No message handlers
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

// App command to run the dialog
void CTabWndApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CTabWndApp message handlers


void CTabWndApp::OnFileNew() 
{
	// TODO: Add your command handler code here
  CWinApp::OnFileNew();
	
}

BOOL CTabWndApp::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
  if (m_pMainWnd) {
    CMDIChildWnd *pWnd = ((CMDIFrameWnd*)m_pMainWnd)->MDIGetActive();
    if (pWnd && pWnd->IsKindOf(RUNTIME_CLASS(CComplexFrameC)))
      if (((CComplexFrameC*)pWnd)->m_Framework.ProcessMessage(pMsg))
        return TRUE;
  }
	return CWinApp::PreTranslateMessage(pMsg);
}
