
#include "stdafx.h"
#include "resource.h"

#include "TreeMapView.h"


/////////////////////////////////////////////////////////////////////////////
// CFileCollectorThread

void CFileCollectorThread::Init(LPCTSTR pstrPath, HWND hWndNotify, UINT uMsg, DWORD dwUpdateInterval, HANDLE hReadyEvent)
{
   m_sPath = pstrPath;
   m_wndNotify = hWndNotify;
   m_uMsg = uMsg;
   m_hReadyEvent = hReadyEvent;
   m_dwUpdateInterval = dwUpdateInterval;
}

DWORD CFileCollectorThread::Run()
{
   // This is a thread that collects files recursively while building the TreeModel.
   // We add files to a list inside each directory node, while we scan directories
   // picked randomly from the directory structure.

   PATHITEM Root;
   Root.sPath = m_sPath;
   Root.pParent = NULL;
   CSimpleArray<PATHITEM> aPaths;
   aPaths.Add(Root);

   UINT uStartDepth = _GetCharCount(m_sPath, '\\');

   CTreeMapModel* pModel = new CTreeMapModel();

   CString sPattern;
   DWORD dwLastUpdate = ::GetTickCount();
   DWORD dwOrder = 0;
   while( aPaths.GetSize() > 0 && !IsAborted() ) 
   {
      int iLastItem = aPaths.GetSize() - 1;
      PATHITEM Path = aPaths[iLastItem];
      aPaths.RemoveAt(iLastItem);

      UINT uDepth = _GetCharCount(Path.sPath, '\\') - uStartDepth;
      CTreeMapItem* pTreeItem = new CTreeMapItem(::PathFindFileName(Path.sPath), ++dwOrder, uDepth, 0.0);

      WIN32_FIND_DATA wfd = { 0 };
      sPattern.Format(_T("%s\\*.*"), Path.sPath);
      HANDLE hFind = ::FindFirstFile(sPattern, &wfd);
      BOOL bRes = (hFind != NULL);
      UINT nFolders = 0;
      while( bRes && !IsAborted() ) 
      {
         if( (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 ) {
            // Process folder item...
            if( wfd.cFileName[0] != '.' ) {
               PATHITEM NewPath;
               NewPath.sPath.Format(_T("%s\\%s"), Path.sPath, wfd.cFileName);
               NewPath.pParent = pTreeItem;
               aPaths.Add(NewPath);
               // Don't process paths in sequence; build paths a little random
               // to improve interactive user experience.
               _RandomizeLastItem(aPaths);
               // FIX: Too many sub-folders can hurt us!
               if( ++nFolders > 2000 ) break;
            }
         }
         else {
            // Process file item...
            TMapItemInfo Info(wfd.cFileName, ++dwOrder, uDepth, wfd.nFileSizeLow);
            pTreeItem->m_aFiles.Add(Info);
         }
         bRes = ::FindNextFile(hFind, &wfd);
      }
      ::FindClose(hFind);

      // Attach in tree...
      if( Path.pParent == NULL ) pModel->m_pRoot = pTreeItem;
      else Path.pParent->m_aChildren.Add(pTreeItem);

      // Submit partial result when:
      //   1) The minimum update interval has passed
      //   2) The view is ready for another update
      //   3) The thread isn't aborted
      if( ::GetTickCount() > dwLastUpdate + m_dwUpdateInterval 
          && ::WaitForSingleObject(m_hReadyEvent, 0) != WAIT_TIMEOUT 
          && !IsAborted() ) 
      {
         ::ResetEvent(m_hReadyEvent);
         CTreeMapModel* pTempModel = new CTreeMapModel();
         pTempModel->Copy(pModel);
         m_wndNotify.PostMessage(m_uMsg, 0, (LPARAM) pTempModel);
         dwLastUpdate = ::GetTickCount();
      }
   }

   // Submit final result
   if( m_wndNotify.IsWindow() && !IsAborted() ) m_wndNotify.PostMessage(m_uMsg, 0, (LPARAM) pModel);
   else delete pModel;

   return 0;
}

UINT CFileCollectorThread::_GetCharCount(LPCTSTR pstrPath, TCHAR ch) const
{
   UINT dwCount = 0;
   while( *pstrPath != '\0' ) if( *pstrPath++ == ch ) dwCount++;
   return dwCount;
}

void CFileCollectorThread::_RandomizeLastItem(CSimpleArray<PATHITEM>& aList)
{
   DWORD nItems = (DWORD) aList.GetSize();
   if( nItems <= 1 ) return;
   DWORD dwRandom = _GetRandom() % nItems;
   if( dwRandom == nItems - 1 ) return;
   PATHITEM TempPath = aList[dwRandom];
   aList[dwRandom] = aList[nItems - 1];
   aList[nItems - 1] = TempPath;
}

DWORD CFileCollectorThread::_GetRandom()
{
   static DWORD s_dwRand = ::GetTickCount();
   s_dwRand = (s_dwRand * 214013UL + 2531011UL) >> 16;
   return (s_dwRand & 0x7FFF) + ::GetTickCount();
}


/////////////////////////////////////////////////////////////////////////////
// CTreeMapView

CTreeMapView::CTreeMapView() : m_pCurItem(NULL), m_bTrackingMouseLeave(false), m_Layout(TREELAYOUT_SQUARED)
{
   m_ReadyEvent.Create(NULL, TRUE, TRUE);
   m_TreeModel.SetLayout(new CTreeMapSquaredLayout());
   m_TreeModel.SetHueRange(20, 37);
   m_TreeModel.SetMinRectangle(3, 3);
}

BOOL CTreeMapView::PreTranslateMessage(MSG* pMsg)
{
   pMsg;
   return FALSE;
}

void CTreeMapView::SetSelection(LPCTSTR pstrPath)
{
   m_ctrlTip.TrackActivate(&m_ti, FALSE);
   m_collector.Stop();
   m_collector.Init(pstrPath, m_hWnd, WM_USER_TREEUPDATE, 100, m_ReadyEvent);
   m_collector.Start();
   m_ReadyEvent.SetEvent();
}

void CTreeMapView::SetLayout(TREELAYOUT Layout)
{
   // Remember the state
   m_Layout = Layout;
   // Create new layout algorithm...
   CTreeMapLayout* pLayout = NULL;
   switch( Layout ) {
   case TREELAYOUT_SLICE:    pLayout = new CTreeMapSliceLayout(); break;
   case TREELAYOUT_STRIP:    pLayout = new CTreeMapStripLayout(); break;
   case TREELAYOUT_SQUARED:  pLayout = new CTreeMapSquaredLayout(); break;
   }
   ATLASSERT(pLayout);
   m_TreeModel.SetLayout(pLayout);
   m_TreeModel.Prepare();
   // Apply layout algorithm
   CRect rcClient;
   GetClientRect(&rcClient);
   m_TreeModel.DoLayout(rcClient);
   // Repaint tree
   Invalidate();
}

TREELAYOUT CTreeMapView::GetLayout() const
{
   return m_Layout;
}

LRESULT CTreeMapView::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
   ModifyStyleEx(0, WS_EX_COMPOSITED);

   _CreateToolTip();
   
   bHandled = FALSE;
   return 0;
}

LRESULT CTreeMapView::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
   if( m_ctrlTip.IsWindow() ) m_ctrlTip.DestroyWindow();
   m_collector.Abort();
   bHandled = FALSE;
   return 0;
}

LRESULT CTreeMapView::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
   CPaintDC dc = m_hWnd;
   CRect rcClient;
   GetClientRect(&rcClient);
   m_TreeModel.DoPaint(dc, rcClient);
   return 0;
}

LRESULT CTreeMapView::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
   CRect rcClient;
   GetClientRect(&rcClient);
   m_TreeModel.DoLayout(rcClient);
   return 0;
}

LRESULT CTreeMapView::OnMouseLeave(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
   KillTimer(TIMERID_TIP);
   m_bTrackingMouseLeave = false;
   m_ctrlTip.TrackActivate(&m_ti, FALSE);
   bHandled = FALSE;
   return 0;
}

LRESULT CTreeMapView::OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
{
   bHandled = FALSE;
   KillTimer(TIMERID_TIP);
   if( !m_bTrackingMouseLeave ) {
      TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_hWnd, 0 };
      _TrackMouseEvent(&tme);
      m_bTrackingMouseLeave = true;
   }
   POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
   const CTreeMapItem* pItem = _HitTest(pt);
   if( pItem == NULL ) 
   {
      // Not hovering over any item? Turn ToolTip off.
      m_ctrlTip.TrackActivate(&m_ti, FALSE);
   }
   else 
   {
      if( m_ctrlTip.GetCurrentTool(NULL) == 0 ) {
         // Start a timer. If it triggers, the ToolTip has hovered over
         // the same item for some time...
         SetTimer(TIMERID_TIP, m_ctrlTip.GetDelayTime(TTDT_INITIAL));
         m_pCurItem = pItem;
      }
      else {
         ClientToScreen(&pt);
         const INT X_OFFSET = 12;
         const INT Y_OFFSET = 18;
         m_ctrlTip.TrackPosition(pt.x + X_OFFSET, pt.y + Y_OFFSET);
         // Already visible! If we're hovering over a new
         // item, change tip text...
         if( m_pCurItem != pItem ) {
            m_pCurItem = pItem;
            m_ctrlTip.TrackActivate(&m_ti, TRUE);
         }
      }
   }
   return 0;
}

LRESULT CTreeMapView::OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
   bHandled = FALSE;
   if( wParam != TIMERID_TIP ) return 0;
   m_ctrlTip.TrackActivate(&m_ti, TRUE);
   return 0;
}

LRESULT CTreeMapView::OnTreeUpdate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
   //ATLTRACE(_T("OnTreeUpdate\n"));
   CTreeMapModel* pTreeModel = reinterpret_cast<CTreeMapModel*>(lParam);
   if( pTreeModel == NULL ) return 0;
   CRect rcClient;
   GetClientRect(&rcClient);
   m_TreeModel.Attach(pTreeModel);
   m_TreeModel.Prepare();
   m_TreeModel.DoLayout(rcClient);
   // The tree-model is owned by us. Destroy it sice we have attached
   // its root to our own instance.
   delete pTreeModel;
   m_pCurItem = NULL;
   // User interface should be refreshed now.
   Invalidate();
   // Signal to the collector thread that we can processes another
   // update now.
   m_ReadyEvent.SetEvent();
   return 0;
}

LRESULT CTreeMapView::OnGetToolTipDispInfo(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled)
{
   LPNMTTDISPINFO lpnmtdi = (LPNMTTDISPINFO) pnmh;
   if( pnmh->hwndFrom != m_ctrlTip || m_pCurItem == NULL ) {
      bHandled = FALSE;
      return 0;
   }
   // Enable multi-line tooltip
   m_ctrlTip.SetMaxTipWidth(LOWORD(::GetDialogBaseUnits()) * 100);
   // Get the size formatted in bytes
   char szSize[80] = { 0 };
   ULONGLONG ullValue = (ULONGLONG) m_pCurItem->m_Info.dblSize;
   ::StrFormatByteSize64(ullValue, szSize, 80);
   // Format the tooltip string
   // NOTE: Documented size for wsprintf() is 1024 characters.
   static char s_szInfoTip[1025] = { 0 };
   ::wsprintf(s_szInfoTip, "File: %s\r\nSize: %s\r\nDepth: %u", m_pCurItem->m_Info.szName, szSize, m_pCurItem->m_Info.uDepth);
   lpnmtdi->lpszText = s_szInfoTip;
   return 0;
}

// Implementation

void CTreeMapView::_CreateToolTip()
{
   // Create the tooltip
   DWORD dwStyle = WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP;
   m_ctrlTip.Create(NULL, CWindow::rcDefault, NULL, dwStyle, WS_EX_TOPMOST);
   ATLASSERT(m_ctrlTip.IsWindow());
   m_ctrlTip.SetWindowPos(HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
   // Add the ListView as a ToolTip tool...
   ::ZeroMemory(&m_ti, sizeof(m_ti));
   m_ti.cbSize = sizeof(TOOLINFO);
   m_ti.uFlags = TTF_IDISHWND | TTF_TRACK | TTF_ABSOLUTE | TTF_PARSELINKS;
   m_ti.hwnd = m_hWnd;
   m_ti.hinst = ModuleHelper::GetResourceInstance();
   m_ti.uId = (UINT) m_hWnd;
   m_ti.lpszText = LPSTR_TEXTCALLBACK;
   m_ctrlTip.AddTool(&m_ti);
   m_ctrlTip.TrackActivate(&m_ti, FALSE);
}

const CTreeMapItem* CTreeMapView::_HitTest(POINT pt) const
{
   const CTreeMapItem* pItem = NULL;
   _HitTestRecursive(m_TreeModel.m_pRoot, pt, &pItem);
   return pItem;
}

void CTreeMapView::_HitTestRecursive(const CTreeMapItem* pItem, POINT pt, const CTreeMapItem** ppResult) const
{
   // Find a match as deep as possible; smaller rectangles are at the bottom
   // of the tree.
   if( pItem == NULL ) return;
   if( ::PtInRect(&pItem->m_Info.rc, pt) ) *ppResult = pItem;
   for( int i = 0; i < pItem->m_aChildren.GetSize(); i++ ) _HitTestRecursive(pItem->m_aChildren[i], pt, ppResult);
}

