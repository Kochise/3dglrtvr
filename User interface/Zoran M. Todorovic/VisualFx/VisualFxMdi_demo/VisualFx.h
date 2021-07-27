/*#############################################################################
# VISUALFX.H
#
# SCA Software International S.A.
# http://www.scasoftware.com
# scaadmin@scasoftware.com
#
# Copyright (c) 1999 SCA Software International S.A.
#
# Date: 03.01.2000
# Author: Zoran M.Todorovic
#
# This software is provided "AS IS", without a warranty of any kind.
# You are free to use/modify this code but leave this header intact.
#
#############################################################################*/

#ifndef __TABWINDOW_H__
#define __TABWINDOW_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable: 4786)

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include <list>
#include <map>

using namespace std;

//=============================================================================
// class TTabItem
//
// This class holds a description of one tab
//=============================================================================

class TTabItem {
private:
  CWnd *m_pWnd;             // Window (view) associated with this tab
  CStatic *m_pCaption;      // Caption of this tab
  BOOL m_bWndEnabled;       // Is window enabled
  BOOL m_bEnabled;          // Is tab enabled
  BOOL m_bVisible;          // Is tab visible
  int m_nMinX;              // Client left X coordinate
  int m_nMaxX;              // Client right X coordinate
  HICON m_hIcon;            // Optional icon handle for tab

public:
  TTabItem(CWnd *pParent, LPCTSTR label, HICON hIcon = NULL);
  TTabItem(const TTabItem& obj);
  TTabItem& operator=(const TTabItem& obj);
  virtual ~TTabItem();
  
  void SetRect(CRect& rect);
  void SetFont(CFont *pFont);
  void SetIcon(HICON hIcon);

  CString GetText(void);
  void SetText(LPCTSTR szLabel);

  void Enable(BOOL bEnable);
  void EnableTab(BOOL bEnable);
  void ShowTab(BOOL nShow);
  
  int GetLength(void);
  CWnd *GetSafeWnd(void);

  friend class TTabWnd;
};

typedef list<TTabItem*> TTabItemList;

//=============================================================================
// class TTabWnd
//
//=============================================================================

class TVisualFramework;

class TTabWnd : public CWnd {
  DECLARE_DYNCREATE(TTabWnd)

public:
  enum TTabPos { TP_TOP, TP_BOTTOM };

protected:
  CBrush m_BrushBlack, m_BrushLGray;
  CPen m_PenWhite, m_PenWhite2, m_PenBlack, m_PenLGray, m_PenDGray, m_PenDGray2;
  CFont m_Font;

protected:
  int m_nSelectedTab;
  BOOL m_bLockFlag;
  TTabItemList m_TabList;
  TTabPos m_nTabPos;

private:
  TTabItem *findTabItem(int nIndex);

protected:
  void createFont();
  int drawTabTop(CDC *pDC, int x, CRect& client, TTabItem *pItem);
  int drawTabBottom(CDC *pDC, int x, CRect& client, TTabItem *pItem);
  int drawSelTabTop(CDC *pDC, int x, CRect& client, TTabItem *pItem);
  int drawSelTabBottom(CDC *pDC, int x, CRect& client, TTabItem *pItem);
  void drawClient(CDC *pDc, CRect& rect);
  void invalidateTabArea(void);
  TTabItem *addTab(CWnd *pWnd, LPCTSTR szLabel);
  BOOL updateFrame(CFrameWnd *pFrame, CWnd *pWnd);

protected:
  // Following functions are protected since tab window cannot be created outside
  // of the visual framework. Otherwise, we have memory leaks.
  
  // Create a tab window
  virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, 
                      DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
                      UINT nID=AFX_IDW_PANE_FIRST, CCreateContext *pContext = NULL);
  // Resize tabs
  virtual void ResizeTab(int cx = -1, int cy = -1);
  // Create pane
  virtual TTabItem *CreatePane(LPCTSTR lpszLabel, CRuntimeClass *pViewClass, 
                                CCreateContext *pContext);
  // Create splitter
	virtual TTabItem *CreatePane(LPCTSTR lpszLabel, int nRows, int nCols, 
                                CWnd *pWnd, UINT nID = AFX_IDW_PANE_FIRST);

public:
  TTabWnd();
  virtual ~TTabWnd();

  int GetTabLength(void);
  int GetTabCount(void);
  CWnd *GetTabWnd(int nIndex);
  CString GetTabLabel(int nIndex);
  void SetTabLabel(int nIndex, LPCTSTR szLabel);

  int GetTabIndex(void);
  int GetTabIndex(CWnd *pWnd);

  void Enable(int nIndex, BOOL bEnable);
  void EnableTab(int nIndex, BOOL bEnable);
  void ShowTab(int nIndex, BOOL bShow);
  
  BOOL IsTabEnabled(int nIndex);
  BOOL IsTabVisible(int nIndex);
  
  void SetFont(CFont *pFont);
  void SetTabPos(TTabPos nTabPos);

  virtual int HitTest(int x, int y);
  virtual int HitTest(CPoint& point);

  // Change active pane
  virtual BOOL SetActivePane(int nIndex, BOOL bActivate = TRUE);

  // Empty functions (implement in derived classes)
  virtual BOOL CanSetActivePane(CWnd *pOldPane, CWnd *pNewPane);
  virtual void OnSetActivePane(CWnd *pOldPane, CWnd *pNewPane);
  
protected:
  //{{AFX_MSG(TTabWnd)
  afx_msg LRESULT OnSizeParent(WPARAM, LPARAM lParam);
  afx_msg BOOL OnEraseBkgnd(CDC* pDC);
  afx_msg void OnPaint();
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnDestroy();
  afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
  DECLARE_MESSAGE_MAP()

  friend class TVisualFramework;
};

//=============================================================================
// class TVisualObject
//
//=============================================================================

class TVisualObject;
class TVisualFramework;

typedef list<TVisualObject*> TVisualObjectList;
typedef map<DWORD,TVisualObject*> TVisualObjectMap;

class TVisualObject {
public:
  enum TObjectStyle {
    TOS_TABTOP        = 0x00000001, // Tabs on top (tab window)
    TOS_TABBOTTOM     = 0x00000002, // Tabs on bottom (tab window)
    TOS_SELECTED      = 0x00000004  // This tab is active
  };

private:
  enum TObjectType { 
    OT_UNKNOWN,
    OT_SPLITTER,                    // Splitter window
    OT_SPLITTERVIEW,                // View within a splitter window
    OT_SPLITTERSPLITTER,            // Nested splitter window
    OT_TAB,                         // Tab window
    OT_TABVIEW,                     // View within a tab window
    OT_VIEW,                        // Plain view
  };
  
private:
  TObjectType m_nObjectType;
  DWORD m_dwId;                     // Unique identifier of this object
  CWnd *m_pWnd;                     // Window associated with this object
  CWnd *m_pParent;                  // Parent window
  CString m_strTitle;               // Title of this window
  CString m_strDescription;         // Optional description of this window
  int m_nRows;                      // Splitters: number of rows
  int m_nCols;                      // Splitters: number of columns
  int m_nRowIndex;                  // Splitter view: row index
  int m_nColIndex;                  // Splitter view: column index
  CCreateContext *m_pContext;       // Context 
  CRuntimeClass *m_pRuntimeClass;   // Class of this window
  CSize m_Size;                     // Splitter: initial size
  BOOL m_bEnabled;                  // Enable state
  DWORD m_dwStyle;                  // Additonal style (for tab window)
  CHAR m_cHotKey;                   // Optional hot key (for example, '2' for ALT+2)
  HICON m_hIcon;                    // Optional icon for tab

private:
  TVisualObject *m_pOwner;          // Owner of this child (or NULL for root level)
  TVisualFramework *m_pFramework;   // Pointer to framework that owns this object
  TVisualObjectList m_ObjectList;   // Child object list

private:
  TVisualObject();
  void zeroAll(void);
  void checkStyle(void);

public:
  // Create a plain view
  TVisualObject(DWORD dwId, CCreateContext *pContext, 
                CRuntimeClass *pClass);
  // Create a tab window or a view within a tab window
  TVisualObject(DWORD dwId, LPCTSTR szTitle, CCreateContext *pContext, 
                CRuntimeClass *pClass, DWORD dwStyle = 0);
  // Create a splitter window
  TVisualObject(DWORD dwId, LPCTSTR szTitle, int nRows, int nCols, 
                CCreateContext *pContext, DWORD dwStyle = 0); 
  // Create a nested splitter window
  TVisualObject(DWORD dwId, int nRow, int nCol, CCreateContext *pContext, 
                CRuntimeClass *pClass, CSize size, DWORD dwStyle = 0);
  // Create a view within a splitter window
  TVisualObject(DWORD dwId, int nRow, int nCol, int nRows, int nCols, 
                CCreateContext *pContext, DWORD dwStyle = 0); 
  TVisualObject(const TVisualObject& obj);
  virtual ~TVisualObject();

  TVisualObject& operator=(const TVisualObject& obj);
  void Destroy(BOOL bDestroyWindow = FALSE);

  BOOL CanFocus(void);
  void SetHotKey(CHAR cHotKey);
  void SetDescription(LPCTSTR szDesc);
  BOOL SetIcon(UINT nIconId);
  HICON GetIcon(void);

  BOOL SetActivePane(void);
  BOOL SetActiveTab(void);

  BOOL Enable(BOOL bEnable);
  BOOL EnableTab(BOOL bEnable);
  BOOL ShowTab(BOOL bShow);

  BOOL IsEnabled(BOOL& bEnabled);
  BOOL IsTabEnabled(BOOL& bEnabled);
  BOOL IsTabVisible(BOOL& bVisible);

  BOOL IsTabPane(void);
  BOOL IsTabWindow(void);
  BOOL IsSplitterPane(void);
  BOOL IsSplitterWindow(void);
  BOOL IsView(void);

  DWORD GetID(void);
  CWnd *GetWnd(void);
  CWnd *GetSafeWnd(void);
  CString GetTitle(void);
  CString GetDescription(void);
  CWnd *GetParentWnd(void);
  TVisualFramework *GetFramework(void);
  TVisualObject *GetOwner(void);

  friend class TVisualFramework;
  friend class TVisualFrameworkIterator;
};

#ifndef _DEBUG

inline DWORD TVisualObject::GetID(void)
  { return m_dwId; }
inline CWnd *TVisualObject::GetWnd(void)
  { return m_pWnd; }
inline CWnd *TVisualObject::GetSafeWnd(void)
  { return ::IsWindow(m_pWnd->m_hWnd) ? m_pWnd : NULL;; }
inline CString TVisualObject::GetTitle(void)
  { return m_strTitle; }
inline CString TVisualObject::GetDescription(void)
  { return m_strDescription; }
inline CWnd *TVisualObject::GetParentWnd(void)
  { return m_pParent; }
inline TVisualFramework *TVisualObject::GetFramework(void)
  { return m_pFramework; }
inline TVisualObject *TVisualObject::GetOwner(void)
  { return m_pOwner; }

#endif

//=============================================================================
// class TVisualFramework
//
//=============================================================================

class TVisualFrameworkIterator;

class TVisualFramework : public CCmdTarget {
	DECLARE_DYNCREATE(TVisualFramework)

private:
  CWnd *m_pOwner;
  TVisualObjectList m_ObjectList;
  TVisualObjectMap m_ObjectMap;

private:
  BOOL m_bEnableCtrlTab;

private:
  TVisualObject *findObject(DWORD dwId);
  TVisualObject *findObject(CWnd *pWnd);
  void execDestroy(TVisualObject *pObject);
  BOOL execCreate(CWnd *pWnd, TVisualObject *pObject);
  BOOL execCreateView(CWnd *pWnd, TVisualObject *pObject);
  BOOL execCreateTabView(CWnd *pWnd, TVisualObject *pObject);
  BOOL execCreateSplitter(CWnd *pWnd, TVisualObject *pObject);
  BOOL execCreateSplitterView(CWnd *pWnd, TVisualObject *pObject);
  BOOL execCreateSplitterSplitter(CWnd *pWnd, TVisualObject *pObject);
  BOOL execCreateTabWnd(CWnd *pWnd, TVisualObject *pObject);
  void setTabWndProperties(TVisualObject *pObject);

public:
  TVisualFramework();
  virtual ~TVisualFramework();

  BOOL Add(TVisualObject *pObject);
  BOOL Add(TVisualObject *pOwner, TVisualObject *pObject);

  virtual BOOL Create(CWnd *pWnd = NULL);
  virtual void Destroy(void);

  CWnd *GetWnd(void);
  CWnd *GetSafeWnd(void);
  CWnd *GetObject(DWORD dwId);
  DWORD GetObject(CWnd *pWnd);
  TVisualObject *Get(DWORD dwId);
  TVisualObject *Get(CWnd *pWnd);

  BOOL IsTabPane(TVisualObject *pObject);
  BOOL IsTabWindow(TVisualObject *pObject);
  BOOL IsSplitterPane(TVisualObject *pObject);
  BOOL IsSplitterWindow(TVisualObject *pObject);
  BOOL IsView(TVisualObject *pObject);

  int GetCount(void);

  TVisualObject *GetActiveTab(TVisualObject *pObject);
  BOOL SetActiveTab(TVisualObject *pObject);

  BOOL SetActivePane(TVisualObject *pObject);
  TVisualObject *GetActivePane(void);

  BOOL Enable(TVisualObject *pObject, BOOL bEnable);
  BOOL EnableTab(TVisualObject *pObject, BOOL bEnable);
  BOOL ShowTab(TVisualObject *pObject, BOOL bEnable);

  BOOL IsEnabled(TVisualObject *pObject, BOOL& bEnabled);
  BOOL IsTabEnabled(TVisualObject *pObject, BOOL& bEnabled);
  BOOL IsTabVisible(TVisualObject *pObject, BOOL& bVisible);

  void SetFont(CFont *pFont);

  void EnableCtrlTab(BOOL bEnable);

  virtual CSplitterWnd *CreateSplitter(DWORD dwId);
  virtual BOOL ProcessMessage(MSG *pMsg);

	//{{AFX_VIRTUAL(TVisualFramework)
	public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(TVisualFramework)
	//}}AFX_MSG

  DECLARE_MESSAGE_MAP()

  friend class TVisualFrameworkIterator;
};

//=============================================================================
// TVisualFrameworkIterator
//
// Iterates thru all visual objects in the framework (including objects that
// cannot be focused like tab windows and splitters)
//=============================================================================

class TVisualFrameworkIterator {
private:
  enum TIteratorType { IT_MAP, IT_LIST };
  TVisualObjectMap *m_pObjectMap;
  TVisualObjectMap::iterator m_MapIt;
  TVisualObjectList *m_pObjectList;
  TVisualObjectList::iterator m_ListIt;
  TIteratorType m_nType;

public:
  TVisualFrameworkIterator(TVisualFramework& obj)
    :m_pObjectMap(&(obj.m_ObjectMap))
  {
    m_MapIt = m_pObjectMap->begin();
    m_nType = IT_MAP;
  }
  TVisualFrameworkIterator(TVisualObject& obj)
    :m_pObjectList(&(obj.m_ObjectList))
  {
    m_ListIt = m_pObjectList->begin();
    m_nType = IT_LIST;
  }
  TVisualObject *operator->()
  {
    return Get();
  }
  TVisualObject *Get(void)
  {
    switch (m_nType) {
    case IT_MAP: return m_MapIt->second;
    case IT_LIST: return *m_ListIt;
    }
    ASSERT(FALSE);
    return NULL;
  }
  int End(void)
  {
    switch (m_nType) {
    case IT_MAP: return (m_MapIt != m_pObjectMap->end()) ? 0 : 1; 
    case IT_LIST: return (m_ListIt != m_pObjectList->end()) ? 0 : 1;
    }
    ASSERT(FALSE);
    return 1;
  }
  int operator++(int)
  { 
    switch (m_nType) {
    case IT_MAP: m_MapIt++; break;
    case IT_LIST: m_ListIt++; break;
    default: ASSERT(FALSE);
    }
    return End();
  }
};

//=============================================================================
// class TVisualFormView
//
//=============================================================================

class TVisualFormView : public CFormView {
	DECLARE_DYNAMIC(TVisualFormView)

protected:
	TVisualFormView(LPCTSTR lpszTemplateName);
	TVisualFormView(UINT nIDTemplate);

public:
  void SetFont(CFont *pFont);

	//{{AFX_MSG(TVisualFormView)
	afx_msg void OnEnable(BOOL bEnable);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif

/*#############################################################################
# End of file VISUALFX.H
#############################################################################*/
