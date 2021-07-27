#if !defined(AFX_MYSUPERGRID_H__09B964C4_953D_11D2_9B04_002018026B76__INCLUDED_)
#define AFX_MYSUPERGRID_H__09B964C4_953D_11D2_9B04_002018026B76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MySuperGrid.h : header file
//
#include "SuperGridCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CMySuperGrid window

// an example of how to use the CSuperGridCtrl class.

class CMySuperGrid : public CSuperGridCtrl
{
// Construction
public:
	CMySuperGrid();
	
// Attributes
public:
// Operations
public:
	//HOWTO:
	
	void InitializeGrid(void);
	void HowToInsertItemsAfterTheGridHasBeenInitialized(int nIndex, const CString& str);
	void HowToLoopThroughAllItems_if_we_wanted_to_print_them_or_what_ever(CDC *pDC);
	void HowToLoopThroughAllItems_that_has_a_checkmark_and_print_them_or_what_ever(CDC *pDC);
	void HowToSearch_I_am_using_hardcoded_values_here_cause_I_am_tired_now(void);
	void SortData(void);
	void _DeleteAll(void);
	void DynamicUpdateSomeItems(int nItem);
	void SetNewImage(int nItem);
	CImageList *CreateDragImageEx(int nItem);
	BOOL m_bDrag;
	//search item and subitems 
	//usage:
	//	CTreeItem *pResult = Search("Item 1","subitem x","Subitem y","subitem 0", NULL); //must finish with NULL terminator thank you
	//  if(pResult!=NULL) .....bla. bla. bla.
	// returns the pointer to node that matches the search criteria or NULL if not found
	CTreeItem *Search(CString strItem, ...);
	CTreeItem* SearchEx(CTreeItem *pStartPosition, CString strItem);
	// Overrides
	void OnControlLButtonDown(UINT nFlags, CPoint point, LVHITTESTINFO& ht);	
	void OnUpdateListViewItem(CTreeItem* lpItem, LV_ITEM *plvItem);
	CItemInfo* CopyData(CItemInfo* lpSrc);
	int GetIcon(const CTreeItem* pItem);
	COLORREF GetCellRGB(void);
	BOOL OnItemExpanding(CTreeItem *pItem, int iItem);
	BOOL OnItemExpanded(CTreeItem* pItem, int iItem);
	BOOL OnCollapsing(CTreeItem *pItem);
	BOOL OnItemCollapsed(CTreeItem *pItem);
	BOOL OnDeleteItem(CTreeItem* pItem, int nIndex);
	BOOL OnVkReturn(void);
	BOOL OnItemLButtonDown(LVHITTESTINFO& ht);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMySuperGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMySuperGrid();

protected:
	CImageList m_image;//Must provide an imagelist
	//positions and creates/initalize a combobox control
	CComboBox* ShowList(int nItem, int nCol, CStringList *lstItems);
	//helper function called from ShowList...calcs the lists max horz extent
	int CalcHorzExtent(CWnd* pWnd, CStringList *pList);
	// Generated message map functions
protected:
	//{{AFX_MSG(CMySuperGrid)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSUPERGRID_H__09B964C4_953D_11D2_9B04_002018026B76__INCLUDED_)
