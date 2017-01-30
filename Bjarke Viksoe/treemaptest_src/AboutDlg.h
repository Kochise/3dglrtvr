// aboutdlg.h : interface of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ABOUTDLG_H__6AE0CDCD_EBC7_49C8_B646_1528ADD33783__INCLUDED_)
#define AFX_ABOUTDLG_H__6AE0CDCD_EBC7_49C8_B646_1528ADD33783__INCLUDED_


class CAboutDlg : public CDialogImpl<CAboutDlg>
{
public:
   enum { IDD = IDD_ABOUTBOX };

   BEGIN_MSG_MAP(CAboutDlg)
      MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
      COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
      COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
   END_MSG_MAP()

   LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
   LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};


#endif // !defined(AFX_ABOUTDLG_H__6AE0CDCD_EBC7_49C8_B646_1528ADD33783__INCLUDED_)
