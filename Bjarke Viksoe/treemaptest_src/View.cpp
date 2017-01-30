// TreeMapTestView.cpp : implementation of the CView class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "View.h"


BOOL CView::PreTranslateMessage(MSG* pMsg)
{
   pMsg;
   return FALSE;
}

LRESULT CView::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
   m_ctrlShellTree.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | TVS_HASBUTTONS | TVS_SHOWSELALWAYS | TVS_NONEVENHEIGHT);
   m_ctrlShellTree.SetShellStyle(SCT_EX_NOFILES | SCT_EX_SHOWHIDDEN | SCT_EX_FILESYSTEMONLY);
   m_ctrlShellTree.ModifyStyleEx(0, WS_EX_COMPOSITED);

   m_ctrlMap.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE);

   SetSplitterPosPct(20);
   SetSplitterPanes(m_ctrlShellTree, m_ctrlMap);

   CPidl pidl;
   ::SHGetSpecialFolderLocation(m_hWnd, CSIDL_PROGRAM_FILES, &pidl);
   m_ctrlShellTree.Populate(CSIDL_DRIVES);
   m_ctrlShellTree.SelectPidl(pidl);

   bHandled = FALSE;
   return 0;
}

LRESULT CView::OnSelChanged(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
{
   CPidl pidl;
   m_ctrlShellTree.GetItemPidl(m_ctrlShellTree.GetSelectedItem(), &pidl);
   TCHAR szPath[MAX_PATH] = { 0 };
   if( !::SHGetPathFromIDList(pidl, szPath) ) return 0;
   if( szPath[0] == '\0' ) return 0;
   ::PathRemoveBackslash(szPath);
   m_ctrlMap.SetSelection(szPath);
   return 0;
}

