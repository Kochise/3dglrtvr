// View.h : interface of the CView class

#pragma once

#include "ShellCtrls.h"
#include "TreeMapView.h"


class CView : public CSplitterWindowImpl<CView>
{
public:
   DECLARE_WND_CLASS(NULL)

   CShellTreeCtrl m_ctrlShellTree;
   CTreeMapView m_ctrlMap;

   BOOL PreTranslateMessage(MSG* pMsg);

   BEGIN_MSG_MAP(CView)
      MESSAGE_HANDLER(WM_CREATE, OnCreate)
      NOTIFY_CODE_HANDLER(TVN_SELCHANGED, OnSelChanged);
      REFLECT_NOTIFICATIONS()
      CHAIN_MSG_MAP( CSplitterWindowImpl<CView> )
   END_MSG_MAP()

   LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
   LRESULT OnSelChanged(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);
};

