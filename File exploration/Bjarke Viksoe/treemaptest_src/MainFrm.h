// MainFrm.h : interface of the CMainFrame class

#pragma once

#include "View.h"


class CMainFrame : 
   public CFrameWindowImpl<CMainFrame>, 
   public CUpdateUI<CMainFrame>,
   public CMessageFilter, 
   public CIdleHandler
{
public:
   DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

   CView m_view;

   CCommandBarCtrl m_CmdBar;

   virtual BOOL PreTranslateMessage(MSG* pMsg);
   virtual BOOL OnIdle();

   BEGIN_UPDATE_UI_MAP(CMainFrame)
      UPDATE_ELEMENT(ID_GRAPH_SLICE, UPDUI_MENUBAR | UPDUI_TOOLBAR)
      UPDATE_ELEMENT(ID_GRAPH_STRIP, UPDUI_MENUBAR | UPDUI_TOOLBAR)
      UPDATE_ELEMENT(ID_GRAPH_SQUARED, UPDUI_MENUBAR | UPDUI_TOOLBAR)
      UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)
      UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
   END_UPDATE_UI_MAP()

   BEGIN_MSG_MAP(CMainFrame)
      MESSAGE_HANDLER(WM_CREATE, OnCreate)
      MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
      COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
      COMMAND_ID_HANDLER(ID_FILE_NEW, OnFileNew)
      COMMAND_ID_HANDLER(ID_GRAPH_SLICE, OnGraphSlice)
      COMMAND_ID_HANDLER(ID_GRAPH_STRIP, OnGraphStrip)
      COMMAND_ID_HANDLER(ID_GRAPH_SQUARED, OnGraphSquared)
      COMMAND_ID_HANDLER(ID_VIEW_TOOLBAR, OnViewToolBar)
      COMMAND_ID_HANDLER(ID_VIEW_STATUS_BAR, OnViewStatusBar)
      COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
      CHAIN_MSG_MAP( CUpdateUI<CMainFrame> )
      CHAIN_MSG_MAP( CFrameWindowImpl<CMainFrame> )
   END_MSG_MAP()

   LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
   LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
   LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
   LRESULT OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
   LRESULT OnGraphSlice(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
   LRESULT OnGraphStrip(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
   LRESULT OnGraphSquared(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
   LRESULT OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
   LRESULT OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
   LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};

