// TreeMapView.h : interface of the CView class

#pragma once

#include "TreeMap.h"

#include "Thread.h"


#define WM_USER_TREEUPDATE   WM_APP + 1200



/////////////////////////////////////////////////////////////////////////////
//

class CFileCollectorThread : public CThreadImpl<CFileCollectorThread>
{
public:
   CString m_sPath;                 // Pathname to begin scan from
   CWindow m_wndNotify;             // CTreeMapView window
   UINT m_uMsg;                     // Message to notify of view updates
   DWORD m_dwUpdateInterval;        // Minimum interval between view updates (ms)
   HANDLE m_hReadyEvent;            // Event when view is ready to receive update

   typedef struct tagPATHITEM
   {
      CString sPath;
      CTreeMapItem* pParent;
   } PATHITEM;

   void Init(LPCTSTR pstrPath, HWND hWndNotify, UINT uMsg, DWORD dwUpdateInterval, HANDLE hReadyEvent);

   DWORD Run();

   UINT _GetCharCount(LPCTSTR pstrPath, TCHAR ch) const;
   void _RandomizeLastItem(CSimpleArray<PATHITEM>& aList);
   DWORD _GetRandom();
};


/////////////////////////////////////////////////////////////////////////////
//

typedef enum TREELAYOUT
{
   TREELAYOUT_SLICE,
   TREELAYOUT_SQUARED,
   TREELAYOUT_STRIP,
};


class CTreeMapView : public CWindowImpl<CTreeMapView>
{
public:
   DECLARE_WND_CLASS(NULL)

   enum { TIMERID_TIP = 120 };

   CTreeMapView();

   CTreeMapModel m_TreeModel;
   CFileCollectorThread m_collector;
   CToolTipCtrl m_ctrlTip;
   CEvent m_ReadyEvent;
   TREELAYOUT m_Layout;
   const CTreeMapItem* m_pCurItem;
   TOOLINFO m_ti;
   bool m_bTrackingMouseLeave;

   BOOL PreTranslateMessage(MSG* pMsg);
   void SetSelection(LPCTSTR pstrPath);
   void SetLayout(TREELAYOUT Layout);
   TREELAYOUT GetLayout() const;

   BEGIN_MSG_MAP(CTreeMapView)
      MESSAGE_HANDLER(WM_CREATE, OnCreate)
      MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
      MESSAGE_HANDLER(WM_PAINT, OnPaint)
      MESSAGE_HANDLER(WM_SIZE, OnSize)
      MESSAGE_HANDLER(WM_USER_TREEUPDATE, OnTreeUpdate)
      MESSAGE_HANDLER(WM_TIMER, OnTimer)
      MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
      MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
      NOTIFY_CODE_HANDLER(TTN_GETDISPINFO, OnGetToolTipDispInfo);
   END_MSG_MAP()

   LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
   LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
   LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
   LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
   LRESULT OnMouseLeave(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
   LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
   LRESULT OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
   LRESULT OnTreeUpdate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
   LRESULT OnGetToolTipDispInfo(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);

   void _CreateToolTip();
   const CTreeMapItem* _HitTest(POINT pt) const;
   void _HitTestRecursive(const CTreeMapItem* pItem, POINT pt, const CTreeMapItem** ppResult) const;
};

