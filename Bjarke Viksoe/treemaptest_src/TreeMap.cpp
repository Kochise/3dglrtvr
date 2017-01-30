
#include "stdafx.h"

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

#include "TreeMap.h"

#include "Colors.h"

#pragma warning(disable : 4244)


////////////////////////////////////////////////////////////////////
// ATL Array Helpers
//
// The ATL CSimpleValArray has no copy-constructor (probably to prevent
// accidental ownership transfer) and it has no GrowBuffer() method either,
// so to optimize copying of the data we'll access the members directly.
//

template< typename T >
void CSimpleValArray_Resize(CSimpleValArray<T>& aList, int nSize)
{
   ATLASSERT(aList.GetSize()==0);
   aList.m_aT = (T*) realloc(aList.m_aT, nSize * sizeof(T));
   aList.m_nAllocSize = nSize;
   aList.m_nSize = nSize;
}

template< typename T >
void CSimpleValArray_Copy(CSimpleValArray<T>& aList, CSimpleValArray<T>& aSrc)
{
   aList.RemoveAll();
   int nItems = aSrc.GetSize();
   CSimpleValArray_Resize(aList, nItems);
   ::CopyMemory(aList.GetData(), aSrc.GetData(), sizeof(T) * nItems);
}

template< typename T >
void CSimpleArray_Resize(CSimpleArray<T>& aList, int nSize)
{
   ATLASSERT(aList.GetSize()==0);
   aList.m_aT = (T*) realloc(aList.m_aT, nSize * sizeof(T));
   aList.m_nAllocSize = nSize;
   aList.m_nSize = nSize;
}

template< typename T >
void CSimpleArray_Copy(CSimpleArray<T>& aList, CSimpleArray<T>& aSrc)
{
   aList.RemoveAll();
   int nItems = aSrc.GetSize();
   CSimpleArray_Resize(aList, nItems);
   for( int i = 0; i < nItems; i++ ) {
      T v = aSrc[i];
      aList.SetAtIndex(i, v);
   }
}


////////////////////////////////////////////////////////////////////
// CTreeMapSliceLayout
//

VOID CTreeMapSliceLayout::DoLayout(CTreeMapItem* pItem, RECT rc)
{
   _DoSliceLayout(pItem->m_aChildren, 0, pItem->m_aChildren.GetSize() - 1, rc);
}

VOID CTreeMapSliceLayout::_DoSliceLayout(CSimpleValArray<CTreeMapItem*>& aItems, INT iStart, INT iEnd, RECT rc)
{
   DOUBLE dblTotal = _GetChildrenSum(aItems, iStart, iEnd);
   DOUBLE a = 0.0;

   if( rc.bottom < rc.top ) rc.bottom = rc.top;
   if( rc.right < rc.left ) rc.right = rc.left;

   bool bVertical = ((rc.right - rc.left) < (rc.bottom - rc.top));
   bool bOrderAsc = true;

   if( dblTotal == 0.0 ) return;

   for( int i = iStart; i <= iEnd; i++ ) 
   {
      CTreeMapItem* pChild = aItems[i];
      RECT rcItem;
      DOUBLE b = pChild->m_Info.dblSize / dblTotal;
      if( bVertical ) 
      {
          rcItem.left = rc.left;
          rcItem.right = rc.right;
          if( bOrderAsc ) rcItem.top = rc.top + (rc.bottom - rc.top) * a;
          else rcItem.top = rc.top + (rc.bottom - rc.top) * (1.0 - a - b);
          rcItem.bottom = rcItem.top + (rc.bottom - rc.top) * b;
      }
      else 
      {
          if( bOrderAsc ) rcItem.left = rc.left + (rc.right - rc.left) * a;
          else rcItem.left = rc.left + (rc.right - rc.left) * (1.0 - a - b);
          rcItem.right = rcItem.left + (rc.right - rc.left) * b;
          rcItem.top = rc.top;
          rcItem.bottom = rc.bottom;
      }
      pChild->m_Info.rc = rcItem;
      a += b;
   }
}

DOUBLE CTreeMapSliceLayout::_GetChildrenSum(const CSimpleValArray<CTreeMapItem*>& aItems, INT iStart, INT iEnd) const
{
   DOUBLE dblSum = 0.0;
   for( INT i = iStart; i <= iEnd; i++ ) dblSum += aItems[i]->m_Info.dblSize;
   return dblSum;
}


////////////////////////////////////////////////////////////////////
// CTreeMapSquaredLayout
//

static int CompareTreeMapItemSize(const void* a, const void* b)
{
  return (int)( (*(CTreeMapItem**)b)->m_Info.dblSize - (*(CTreeMapItem**)a)->m_Info.dblSize );
}

VOID CTreeMapSquaredLayout::DoLayout(CTreeMapItem* pItem, RECT rc)
{
   // Sort items according to size value
   CSimpleValArray<CTreeMapItem*> aSortedList;
   CSimpleValArray_Copy(aSortedList, pItem->m_aChildren);
   qsort(aSortedList.GetData(), aSortedList.GetSize(), sizeof(CTreeMapItem*), CompareTreeMapItemSize);

   _DoSquaredLayout(aSortedList, 0, aSortedList.GetSize() - 1, rc);
}

VOID CTreeMapSquaredLayout::_DoSquaredLayout(CSimpleValArray<CTreeMapItem*>& aItems, INT iStart, INT iEnd, RECT rc)
{
   if( iStart > iEnd ) return;
   if( iEnd - iStart < 2 ) {
      _DoSliceLayout(aItems, iStart, iEnd, rc);
      return;
   }
   DOUBLE x = rc.left;
   DOUBLE y = rc.top;
   DOUBLE w = rc.right - rc.left;
   DOUBLE h = rc.bottom - rc.top;
   DOUBLE dblTotal = _GetChildrenSum(aItems, iStart, iEnd);
   if( dblTotal == 0.0 ) return;
   INT iMid = iStart;
   DOUBLE a = aItems[iStart]->m_Info.dblSize / dblTotal;
   DOUBLE b = a;
   if( w < h )
   {
      // height/width
      while( iMid < iEnd ) {
         DOUBLE dblAspect = _GetNormAspect(h, w, a, b);
         DOUBLE q = aItems[iMid + 1]->m_Info.dblSize / dblTotal;
         if( _GetNormAspect(h, w, a, b + q) > dblAspect ) break;
         b += q;
         ++iMid;
      }
      RECT rcSliced = { x, y, x + w, y + (h * b) };
      RECT rcSquared = { x, y + (h * b), x + w, y + h };
      _DoSliceLayout(aItems, iStart, iMid, rcSliced);
      _DoSquaredLayout(aItems, iMid + 1, iEnd, rcSquared);
   }
   else 
   {
      // width/height
      while( iMid < iEnd ) {
         DOUBLE dblAspect = _GetNormAspect(w, h, a, b);
         DOUBLE q = aItems[iMid + 1]->m_Info.dblSize / dblTotal;
         if( _GetNormAspect(w, h, a, b + q) > dblAspect ) break;
         b += q;
         ++iMid;
      }
      RECT rcSlice = { x, y, x + (w * b), y + h };
      RECT rcSquared = { x + (w * b), y, x + w, y + h };
      _DoSliceLayout(aItems, iStart, iMid, rcSlice);
      _DoSquaredLayout(aItems, iMid + 1, iEnd, rcSquared);
   }
}

DOUBLE CTreeMapSquaredLayout::_GetNormAspect(DOUBLE big, DOUBLE smal, DOUBLE a, DOUBLE b) const
{
   DOUBLE x = (big * b) / (smal * a / b);
   if( x < 1.0 ) return 1.0 / x;
   return x;
}


////////////////////////////////////////////////////////////////////
// CTreeMapStripLayout
//

VOID CTreeMapStripLayout::DoLayout(CTreeMapItem* pItem, RECT rc)
{
   _DoStripLayout(pItem->m_aChildren, 0, pItem->m_aChildren.GetSize() - 1, rc);
}

VOID CTreeMapStripLayout::_DoStripLayout(CSimpleValArray<CTreeMapItem*>& aItems, INT iStart, INT iEnd, RECT rc)
{
   DOUBLE dblTotal = _GetChildrenSum(aItems, iStart, iEnd);
   if( dblTotal == 0.0 ) return;

   bool bLookAhead = true;

   if( rc.bottom < rc.top ) rc.bottom = rc.top;
   if( rc.right < rc.left ) rc.right = rc.left;

   DOUBLE dblArea = (rc.right - rc.left) * (rc.bottom - rc.top);
   DOUBLE dblScaleFactor = sqrt(dblArea / dblTotal);
   if( dblScaleFactor == 0.0 ) return;

   INT iFinish = 0;
   DOUBLE yoffset = 0;
   RECT rcBox = { rc.left / dblScaleFactor, rc.top / dblScaleFactor, rc.right / dblScaleFactor, rc.bottom / dblScaleFactor };

   while( iFinish <= iEnd ) {
      INT nItems = _DoOneLayoutStrip(aItems, rcBox, iFinish, iEnd);
      if( bLookAhead && iFinish + nItems <= iEnd ) {
         // Layout 2nd strip and compute AR of first strip plus 2nd strip
         INT nItems2 = _DoOneLayoutStrip(aItems, rcBox, iFinish + nItems, iEnd);
         DOUBLE ar2a = _ComputeAverageAspectRatio(aItems, iFinish, nItems + nItems2);
         // Layout 1st and 2nd strips together
         _ComputeHorizontalBoxLayout(aItems, rcBox, iFinish, nItems + nItems2);
         DOUBLE ar2b = _ComputeAverageAspectRatio(aItems, iFinish, nItems + nItems2);
         if( ar2b < ar2a ) nItems += nItems2;
         else _ComputeHorizontalBoxLayout(aItems, rcBox, iFinish, nItems);
      }
      for( int i = iFinish; i < iFinish + nItems; i++ ) {
         aItems[i]->m_Info.rc.top += yoffset;
         aItems[i]->m_Info.rc.bottom += yoffset;
      }
      DOUBLE cy = aItems[iFinish]->m_Info.rc.bottom - aItems[iFinish]->m_Info.rc.top;
      yoffset += cy;
      rcBox.top += cy;
      iFinish += nItems;
   }

   for( int i = iStart; i <= iEnd; i++ ) {
      RECT& rcItem = aItems[i]->m_Info.rc;
      rcItem.left *= dblScaleFactor;
      rcItem.top *= dblScaleFactor;
      rcItem.right *= dblScaleFactor;
      rcItem.bottom *= dblScaleFactor;
      rcItem.left += rc.left;
      rcItem.top += rc.top;
      rcItem.right += rc.left;
      rcItem.bottom += rc.top;
   }
}

int CTreeMapStripLayout::_DoOneLayoutStrip(CSimpleValArray<CTreeMapItem*>& aItems, RECT rcBox, int iStart, int iEnd)
{
   INT nItems = 0;
   DOUBLE ar = 99999999999999999.0;
   DOUBLE cy, prevAR;
   do {
      prevAR = ar;
      ++nItems;
      cy = _ComputeHorizontalBoxLayout(aItems, rcBox, iStart, nItems);
      ar = _ComputeAverageAspectRatio(aItems, iStart, nItems);
   } while( (ar < prevAR ) && (iStart + nItems <= iEnd) );
   if( ar >= prevAR ) {
      --nItems;
      cy = _ComputeHorizontalBoxLayout(aItems, rcBox, iStart, nItems);
      ar = _ComputeAverageAspectRatio(aItems, iStart, nItems);
   }
   return nItems;
}

DOUBLE CTreeMapStripLayout::_ComputeHorizontalBoxLayout(CSimpleValArray<CTreeMapItem*>& aItems, RECT rcBox, int iStart, int nItems) 
{
   DOUBLE dblSize = _GetChildrenSum(aItems, iStart, iStart + nItems - 1);
   if( rcBox.right - rcBox.left == 0 ) return 0.0;
   DOUBLE cy = dblSize / (rcBox.right - rcBox.left);
   DOUBLE x = 0.0;
   for( int i = 0; i < nItems; i++ ) {
      DOUBLE cx = aItems[iStart + i]->m_Info.dblSize / cy;
      RECT rc = { x, 0, x + cx, 0 + cy };
      aItems[iStart + i]->m_Info.rc = rc;;
      x += cx;
   }
   return cy;
}

DOUBLE CTreeMapStripLayout::_ComputeAverageAspectRatio(CSimpleValArray<CTreeMapItem*>& aItems, int iStart, int nItems) 
{
   DOUBLE tar = 0.0;
   if( nItems == 0 ) return tar;
   for( int i = 0; i < nItems; i++ ) {
      const RECT& rcItem = aItems[iStart + i]->m_Info.rc;
      DOUBLE cx = rcItem.right - rcItem.left;
      DOUBLE cy = rcItem.bottom - rcItem.top;
      if( cx == 0.0 || cy == 0.0 ) continue;
      DOUBLE ar = (cx / cy) > (cy / cx) ? (cx / cy) : (cy / cx);
      tar += ar;
   }
   tar /= nItems;
   return tar;
}


////////////////////////////////////////////////////////////////////
// TMapItemInfo
//

TMapItemInfo::TMapItemInfo()
{
   szName[0] = '\0';
   uDepth = 0;
   dwOrder = 0;
   dblSize = 0.0;
   rc.left = rc.right = rc.top = rc.bottom = 0;
}

TMapItemInfo::TMapItemInfo(LPCTSTR pstrName, DWORD dwOrder, UINT uDepth, DOUBLE dblSize)
{
   _tcsncpy(szName, pstrName, (sizeof(szName) / sizeof(szName[0])));
   szName[ (sizeof(szName) / sizeof(szName[0])) - 1 ] = '\0';
   this->uDepth = uDepth;
   this->dwOrder = dwOrder;
   this->dblSize = dblSize;
   rc.left = rc.right = rc.top = rc.bottom = 0;
}


////////////////////////////////////////////////////////////////////
// CTreeMapItem
//

CTreeMapItem::CTreeMapItem()
{
}

CTreeMapItem::CTreeMapItem(LPCTSTR pstrName, DWORD dwOrder, UINT uDepth, DOUBLE dblSize) :
   m_Info(pstrName, dwOrder, uDepth, dblSize)
{
}

CTreeMapItem::CTreeMapItem(const CTreeMapItem& /*src*/)
{
   ATLASSERT(false);
}

CTreeMapItem::~CTreeMapItem()
{
   for( int i = 0; i < m_aChildren.GetSize(); i++ ) delete m_aChildren[i];
}

void CTreeMapItem::Copy(CTreeMapItem* pSrc)
{
   this->m_Info = pSrc->m_Info;

   // Copy file items
   CSimpleValArray_Copy(this->m_aFiles, pSrc->m_aFiles);

   // Copy folder items
   int nChildren = pSrc->m_aChildren.GetSize();
   CSimpleValArray_Resize(this->m_aChildren, nChildren);
   for( int i = 0; i < nChildren; i++ ) {
      CTreeMapItem* pDstItem = new CTreeMapItem;
      pDstItem->Copy(pSrc->m_aChildren[i]);
      this->m_aChildren.SetAtIndex(i, pDstItem);
   }
}


////////////////////////////////////////////////////////////////////
// CTreeMapModel
//

CTreeMapModel::CTreeMapModel() : 
   m_pLayout(NULL), 
   m_pRoot(NULL), 
   m_uHueStart(0), 
   m_uHueEnd(240),
   m_uMaxDepth(0),
   m_cxDrawMinRect(0), 
   m_cyDrawMinRect(0), 
   m_uDrawMaxDepth(99), 
   m_hFont(NULL)
{
}

CTreeMapModel::~CTreeMapModel()
{
   Destroy();
   delete m_pLayout;
}

CTreeMapModel::CTreeMapModel(const CTreeMapModel& /*src*/)
{
   ATLASSERT(false);
}

void CTreeMapModel::Destroy()
{
   delete m_pRoot;
   m_pRoot = NULL;
}

void CTreeMapModel::Attach(CTreeMapModel* pSrc)
{
   ATLASSERT(pSrc);
   Destroy();
   m_pRoot = pSrc->m_pRoot;
   pSrc->m_pRoot = NULL;
}

void CTreeMapModel::Copy(CTreeMapModel* pSrc)
{
   ATLASSERT(pSrc);
   Destroy();
   m_pRoot = new CTreeMapItem();
   m_pRoot->Copy(pSrc->m_pRoot);
}

void CTreeMapModel::SetLayout(CTreeMapLayout* pLayout)
{
   delete m_pLayout;
   m_pLayout = pLayout;
}

void CTreeMapModel::SetMinRectangle(INT cx, INT cy)
{
   m_cxDrawMinRect = cx;
   m_cyDrawMinRect = cy;
}

void CTreeMapModel::SetMaxDepth(UINT uDepth)
{
   ATLASSERT(uDepth<=99);
   m_uDrawMaxDepth = uDepth;
}

void CTreeMapModel::SetHueRange(WORD uStart, WORD uEnd)
{
   m_uHueStart = uStart;
   m_uHueEnd = uEnd;
}

void CTreeMapModel::Prepare()
{
   if( m_pRoot == NULL ) return;

   m_uMaxDepth = 0;

   _DoClearCalculation(m_pRoot);

   MINMAXPAIR pair = { 999999999999.0, 0.0 };
   m_MinMaxSize = pair;
   _DoSizeCalculation(m_pRoot);

   m_aMinMaxDepth.RemoveAll();
   CSimpleArray_Resize(m_aMinMaxDepth, 99);
   for( int i = 0; i < 99; i++ ) m_aMinMaxDepth.SetAtIndex(i, pair);
   _DoColorCalculation(m_pRoot);
}

void CTreeMapModel::DoLayout(RECT rc)
{
   if( m_pRoot == NULL ) return;
   
   // Create a layout algorithm if needed
   if( m_pLayout == NULL ) m_pLayout = new CTreeMapSliceLayout();

   // Create the sub-title font is needed
   if( m_hFont == NULL ) {
      CLogFont lf = AtlGetDefaultGuiFont();
      lf.lfHeight = -10;
      m_fontSmallTitle.CreateFontIndirect(&lf);
      m_hFont = m_fontSmallTitle;
      CWindowDC dcWindow = HWND_DESKTOP;
      HFONT hOldFont = dcWindow.SelectFont(m_hFont);
      dcWindow.GetTextMetrics(&m_tmFont);
      dcWindow.SelectFont(hOldFont);
   }

   m_pRoot->m_Info.rc = rc;
   _DoLayout(m_pRoot, rc);
}

void CTreeMapModel::DoPaint(HDC hDC, RECT rc)
{
   //ATLTRACE(_T("DoPaint\n"));
   CDCHandle dc = hDC;
   if( m_pRoot == NULL ) {
      dc.FillSolidRect(&rc, ::GetSysColor(COLOR_WINDOW));
      return;
   }
   int iSaveDC = ::SaveDC(hDC);
   _DoPaint(m_pRoot, dc);
   ::RestoreDC(hDC, iSaveDC);
}

void CTreeMapModel::_DoClearCalculation(CTreeMapItem* pItem)
{
   // Reset all structures that are calculated by Prepare() or DoLayout()
   static const RECT rcEmpty = { 0 };
   pItem->m_Info.rc = rcEmpty;
   pItem->m_Info.dblSize = 0.0;
   // Process children...
   for( int i = 0; i < pItem->m_aChildren.GetSize(); i++ ) _DoClearCalculation(pItem->m_aChildren[i]);
}

double CTreeMapModel::_DoSizeCalculation(CTreeMapItem* pItem)
{
   // Determine the sum of size of files and folders for each node.
   double dblSize = 0.0;
   for( int i1 = 0; i1 < pItem->m_aFiles.GetSize(); i1++ ) dblSize += pItem->m_aFiles[i1].dblSize;
   for( int i2 = 0; i2 < pItem->m_aChildren.GetSize(); i2++ ) dblSize += _DoSizeCalculation(pItem->m_aChildren[i2]);
   pItem->m_Info.dblSize = dblSize;
   if( dblSize < m_MinMaxSize.dblMin ) m_MinMaxSize.dblMin = dblSize;
   if( dblSize > m_MinMaxSize.dblMax ) m_MinMaxSize.dblMax = dblSize;
   return dblSize;
}

void CTreeMapModel::_DoColorCalculation(CTreeMapItem* pItem)
{
   // To paint in nice gradient colors we need to record the mininmum and maximum
   // size sums for nodes at each tree depth.
   if( _FilterItem(pItem) ) return;
   MINMAXPAIR& pair = m_aMinMaxDepth[pItem->m_Info.uDepth];
   if( pItem->m_Info.dblSize < pair.dblMin ) pair.dblMin = pItem->m_Info.dblSize;
   if( pItem->m_Info.dblSize > pair.dblMax ) pair.dblMax = pItem->m_Info.dblSize;
   // Determine maximum depth we can reach
   if( pItem->m_Info.uDepth > m_uMaxDepth ) m_uMaxDepth = pItem->m_Info.uDepth;
   // Process children...
   for( int i = 0; i < pItem->m_aChildren.GetSize(); i++ ) _DoColorCalculation(pItem->m_aChildren[i]);
}

void CTreeMapModel::_DoLayout(CTreeMapItem* pItem, RECT rc)
{
   // Create bounds (with indent for subtitle text)
   const INT cxyBorder = 6;
   ::InflateRect(&rc, -cxyBorder, -cxyBorder);
   rc.top += m_tmFont.tmHeight + 1;
   // Do layout of this item
   m_pLayout->DoLayout(pItem, rc);
   // Do layout of children...
   for( int i = 0; i < pItem->m_aChildren.GetSize(); i++ ) {
      _DoLayout(pItem->m_aChildren[i], pItem->m_aChildren[i]->m_Info.rc);
   }
}

bool CTreeMapModel::_FilterItem(const CTreeMapItem* pItem) const
{
   // Filter items beyond a certain tree depth
   if( pItem->m_Info.uDepth > m_uDrawMaxDepth ) return true;

   // Filter items when their rectangles are too small
   if( pItem->m_Info.uDepth > 1 ) {
      if( pItem->m_Info.rc.right - pItem->m_Info.rc.left < m_cxDrawMinRect ) return true;
      if( pItem->m_Info.rc.bottom - pItem->m_Info.rc.top < m_cyDrawMinRect ) return true;
   }

   return false;
}

void CTreeMapModel::_DoPaint(const CTreeMapItem* pItem, CDCHandle& dc)
{
   if( _FilterItem(pItem) ) return;

   // Calculate a color for this rectangle.
   // Use its relative size for all folders on this depth to determine the HUE
   const MINMAXPAIR& minmax = m_aMinMaxDepth[pItem->m_Info.uDepth];
   double dblMaxDepth = (double) (m_uMaxDepth + 1);
   double dblHueRange = (m_uHueEnd - m_uHueStart) / dblMaxDepth;
   double dblHueStart = pItem->m_Info.uDepth * dblHueRange;
   double dblHue = ((pItem->m_Info.dblSize - minmax.dblMin) / (minmax.dblMax - minmax.dblMin)) * dblHueRange;
   WORD wHue = (double) m_uHueStart + dblHueStart + dblHue;

   const RECT& rc = pItem->m_Info.rc;

   // Fill rectangle
   COLORREF clrBack = CColors::RGBQUAD2RGB( CColors::HSLtoRGB(RGB(255,255,wHue)) );
   dc.FillSolidRect(&rc, clrBack);

   // Draw 3D border
   RECT rcBorder = rc;
   COLORREF clrTL1 = CColors::BlendRGB(clrBack, RGB(255,255,255), 40);
   COLORREF clrTL2 = CColors::BlendRGB(clrBack, RGB(255,255,255), 10);
   COLORREF clrBR1 = CColors::BlendRGB(clrBack, RGB(0,0,0), 40);
   COLORREF clrBR2 = CColors::BlendRGB(clrBack, RGB(0,0,0), 10);
   dc.Draw3dRect(&rcBorder, clrTL1, clrBR1);
   ::InflateRect(&rcBorder, -1, -1);
   dc.Draw3dRect(&rcBorder, clrTL2, clrBR2);

   // Draw text
   RECT rcText = { rc.left + 6, rc.top + 4, rc.right - 3, rc.bottom - 3 };
   dc.SelectFont(m_hFont);
   dc.SetBkMode(TRANSPARENT);
   dc.SetTextColor(RGB(0,0,0));
   dc.DrawText(pItem->m_Info.szName, -1, &rcText, DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX);
   if( pItem->m_Info.uDepth == 0 ) {
      ::OffsetRect(&rcText, -1, -1);
      dc.SetTextColor(RGB(205,205,205));
      dc.DrawText(pItem->m_Info.szName, -1, &rcText, DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX);
   }

   //ATLTRACE(_T("rect: (%3ld,%3ld)-(%3ld,%3ld) size=%-12.1f hue=%-3u depth=%-2u clr=0x%08X  dir=%s\n"), rc.left, rc.top, rc.right, rc.bottom, pItem->m_Info.dblSize, wHue, pItem->m_Info.uDepth, clrBack, pItem->m_Info.szName);

   // Draw childen as well.
   // Children all have rectangles enclosed in their parent.
   for( int i = 0; i < pItem->m_aChildren.GetSize(); i++ ) _DoPaint(pItem->m_aChildren[i], dc);
}


#pragma warning(default : 4244)

