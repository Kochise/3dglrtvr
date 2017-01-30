
#pragma once

/////////////////////////////////////////////////////////////////////////////
// TreeMap - A TreeMap layout graph
//
// Written by Bjarke Viksoe (bjarke@viksoe.dk)
// Copyright (C) 2008 Bjarke Viksoe.
//
// This code was inspired by the Java TreeMap Layout Comparison
// library. This code may be covered by the Mozilla Public License
// (MPL, http://www.mozilla.org/MPL/).
// The Initial Developers of the Original Code is Ben Bederson
// and Martin Wattinberg.  
// Copyright (C) 2001 University of Maryland, College Park, MD 20742.
// All Rights Reserved.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to you or your
// computer whatsoever. It's free, so don't hassle me about it.
//
// Beware of bugs.
//


////////////////////////////////////////////////////////////////////
// TreeMap forward declares
//

class CTreeMapItem;


////////////////////////////////////////////////////////////////////
// TreeMap layout algorithms
//

/// Interface for layout algorithm
class CTreeMapLayout
{
public:
   virtual ~CTreeMapLayout() { };
   virtual VOID DoLayout(CTreeMapItem* pItem, RECT rc) = 0;
};


class CTreeMapSliceLayout : public CTreeMapLayout
{
public:
   // CTreeMapLayout
   VOID DoLayout(CTreeMapItem* pItem, RECT rc);

   // Operations
   VOID _DoSliceLayout(CSimpleValArray<CTreeMapItem*>& aItems, INT iStart, INT iEnd, RECT rc);
   
   // Implementation
   DOUBLE _GetChildrenSum(const CSimpleValArray<CTreeMapItem*>& aItems, INT iStart, INT iEnd) const;
};


class CTreeMapSquaredLayout : public CTreeMapSliceLayout
{
public:
   // CTreeMapLayout
   VOID DoLayout(CTreeMapItem* pItem, RECT rc);

   // Operations
   VOID _DoSquaredLayout(CSimpleValArray<CTreeMapItem*>& aItems, INT iStart, INT iEnd, RECT rc);
   
   // Implementation
   DOUBLE _GetNormAspect(DOUBLE big, DOUBLE smal, DOUBLE a, DOUBLE b) const;
};


class CTreeMapStripLayout : public CTreeMapSliceLayout
{
public:
   // CTreeMapLayout
   VOID DoLayout(CTreeMapItem* pItem, RECT rc);

   // Operations
   VOID _DoStripLayout(CSimpleValArray<CTreeMapItem*>& aItems, INT iStart, INT iEnd, RECT rc);

   // Implementation
   int _DoOneLayoutStrip(CSimpleValArray<CTreeMapItem*>& aItems, RECT rcBox, int iIndex, int iEnd);
   DOUBLE _ComputeHorizontalBoxLayout(CSimpleValArray<CTreeMapItem*>& aItems, RECT rcBox, int iStart, int nItems);
   DOUBLE _ComputeAverageAspectRatio(CSimpleValArray<CTreeMapItem*>& aItems, int iStart, int nItems);
};


////////////////////////////////////////////////////////////////////
// TreeMap item
//

/// Item structure that holds information about a file
struct TMapItemInfo
{
   TMapItemInfo();
   TMapItemInfo(LPCTSTR pstrName, DWORD dwOrder, UINT uDepth, DOUBLE dblSize);

   TCHAR szName[60];                             /// Name of node
   RECT rc;                                      /// Calculated Bounds of node
   UINT uDepth;                                  /// Depth in tree
   DWORD dwOrder;                                /// Collected order sequence
   DOUBLE dblSize;                               /// Weight (size) of node
};


class CTreeMapItem
{
public:
   CTreeMapItem();
   ~CTreeMapItem();

   CTreeMapItem(const CTreeMapItem& src);
   CTreeMapItem(LPCTSTR pstrName, DWORD dwOrder, UINT uDepth, DOUBLE dblSize);

   void Copy(CTreeMapItem* pSrc);

   TMapItemInfo m_Info;                          /// Directory information
   CSimpleValArray<TMapItemInfo> m_aFiles;       /// File items
   CSimpleValArray<CTreeMapItem*> m_aChildren;   /// Children
};


////////////////////////////////////////////////////////////////////
// TreeMap model
//

class CTreeMapModel
{
public:
   CTreeMapModel();
   ~CTreeMapModel();

   CTreeMapModel(const CTreeMapModel& src);

   typedef struct tagMINMAXPAIR
   {
      DOUBLE dblMin;
      DOUBLE dblMax;
   } MINMAXPAIR;

   CTreeMapLayout* m_pLayout;                    /// Active layout algorithm
   CTreeMapItem* m_pRoot;                        /// Root node
   HFONT m_hFont;                                /// Subtitle font
   TEXTMETRIC m_tmFont;                          /// Metrics of subtitle font
   CFont m_fontSmallTitle;                       /// Subtitle font if we created it
   INT m_cxDrawMinRect;                          /// Minimum width for rectangle displayed
   INT m_cyDrawMinRect;                          /// Minimum height for rectangle displayed
   UINT m_uDrawMaxDepth;                         /// Maximum depth for rectangle displayed
   UINT m_uHueStart;                             /// Hue value for start color range
   UINT m_uHueEnd;                               /// Hue value for end color range
   UINT m_uMaxDepth;                             /// Maximum depth for current display
   MINMAXPAIR m_MinMaxSize;                      /// Min and max values for current display
   CSimpleArray<MINMAXPAIR> m_aMinMaxDepth;      /// Min and max values for current display pr depth

   // Operations

   void Prepare();
   void Destroy();
   void Copy(CTreeMapModel* pSrc);
   void Attach(CTreeMapModel* pSrc);

   void SetLayout(CTreeMapLayout* pLayout);
   void SetFont(HFONT hFont);
   void SetMinRectangle(INT cx, INT cy);
   void SetMaxDepth(UINT uDepth);
   void SetHueRange(WORD uStart, WORD uEnd);

   void DoLayout(RECT rc);
   void DoPaint(HDC hDC, RECT rc);

   // Implementation

   void _DoLayout(CTreeMapItem* pItem, RECT rc);
   bool _FilterItem(const CTreeMapItem* pItem) const;
   void _DoPaint(const CTreeMapItem* pItem, CDCHandle& dc);
   void _DoClearCalculation(CTreeMapItem* pItem);
   void _DoColorCalculation(CTreeMapItem* pItem);
   double _DoSizeCalculation(CTreeMapItem* pItem);
};

